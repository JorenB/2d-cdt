// Copyright 2023 Google LLC
#include "ricci_cuda_kernels.hpp"
#include "../universe.hpp" 
#include <vector>
#include <numeric> 
#include <algorithm> 

#include <cuda_runtime.h>
#include <iostream> 

#define CUDA_CHECK(err)     do {         cudaError_t err_ = (err);         if (err_ != cudaSuccess) {             std::cerr << "CUDA error in " << __FILE__ << " at line " << __LINE__                       << ": " << cudaGetErrorString(err_) << std::endl;             /* Potentially throw an exception or exit to prevent further errors */             /* For a library, re-throwing as a C++ exception might be better */             /* For this context, printing and continuing might be okay for initial tests, */             /* but robust error handling should be considered. */             return; /* Simple return on error for now */         }     } while(0)

namespace RicciCUDATask {

// ... (keep existing __device__ and __global__ functions as implemented previously) ...
const int LOCAL_BFS_MAX_QUEUE_SIZE = 256; 
const int LOCAL_BFS_MAX_VISITED_SIZE = 256;

__device__ int calculate_distance_bfs_device(
    Vertex::Label u_start, Vertex::Label v_target,
    const int* d_adj_list, const int* d_adj_offsets,
    int num_universe_vertices, int max_depth) {

    if (u_start == v_target) return 0;

    Vertex::Label queue[LOCAL_BFS_MAX_QUEUE_SIZE];
    int q_head = 0;
    int q_tail = 0;

    Vertex::Label visited_in_bfs[LOCAL_BFS_MAX_VISITED_SIZE];
    int visited_count = 0;

    if (q_tail < LOCAL_BFS_MAX_QUEUE_SIZE) {
        queue[q_tail++] = u_start;
    } else { return -1; }
    if (visited_count < LOCAL_BFS_MAX_VISITED_SIZE) {
        visited_in_bfs[visited_count++] = u_start;
    } else { return -1; }

    for (int current_depth = 0; current_depth < max_depth; ++current_depth) {
        int current_level_size = q_tail - q_head;
        if (current_level_size == 0) break; 

        for (int i = 0; i < current_level_size; ++i) {
            if (q_head >= q_tail) { return -1; }
            Vertex::Label curr_v = queue[q_head++];
            
            if (curr_v >= num_universe_vertices || curr_v < 0) { /* Safety check */ return -1;}

            int neighbors_start_idx = d_adj_offsets[curr_v];
            int neighbors_end_idx = d_adj_offsets[curr_v + 1];

            for (int neighbor_idx = neighbors_start_idx; neighbor_idx < neighbors_end_idx; ++neighbor_idx) {
                Vertex::Label neighbor = d_adj_list[neighbor_idx];
                 if (neighbor >= num_universe_vertices || neighbor < 0) { /* Safety check */ continue;}


                if (neighbor == v_target) {
                    return current_depth + 1;
                }

                bool already_visited = false;
                for (int j = 0; j < visited_count; ++j) {
                    if (visited_in_bfs[j] == neighbor) {
                        already_visited = true;
                        break;
                    }
                }

                if (!already_visited) {
                    if (q_tail >= LOCAL_BFS_MAX_QUEUE_SIZE) { return -1; }
                    queue[q_tail++] = neighbor; 
                    
                    if (visited_count >= LOCAL_BFS_MAX_VISITED_SIZE) { return -1; }
                    visited_in_bfs[visited_count++] = neighbor;
                }
            }
        }
    }
    return -1; 
}

__global__ void pairwise_bfs_kernel(
    const Vertex::Label* d_s1_vertices, int s1_size,
    const Vertex::Label* d_s2_vertices, int s2_size,
    const int* d_adj_list, const int* d_adj_offsets, 
    int num_universe_vertices, int max_depth,         
    long long int* d_total_dist_sum,
    int* d_total_dist_count) {

    int s1_glob_idx = blockIdx.x * blockDim.x + threadIdx.x;
    int s2_glob_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if (s1_glob_idx >= s1_size || s2_glob_idx >= s2_size) {
        return;
    }

    Vertex::Label u = d_s1_vertices[s1_glob_idx];
    Vertex::Label v_target = d_s2_vertices[s2_glob_idx];
    
    // Add bounds check for u and v_target before calling device function,
    // if Vertex::Label can be outside [0, num_universe_vertices-1]
    if (u >= num_universe_vertices || u < 0 || v_target >= num_universe_vertices || v_target < 0) {
        // Or handle this error more explicitly if possible
        return; 
    }

    int dist = calculate_distance_bfs_device(u, v_target, d_adj_list, d_adj_offsets, num_universe_vertices, max_depth);

    if (dist != -1) {
        atomicAdd(d_total_dist_sum, (long long int)dist);
        atomicAdd(d_total_dist_count, 1);
    }
}

// Host wrapper function - Full Implementation
void calculate_sum_and_count_distances_cuda(
    const std::vector<Vertex::Label>& s1_vertices,
    const std::vector<Vertex::Label>& s2_vertices,
    const std::vector<std::vector<Vertex::Label>>& adj_list_vector_cpu, // Renamed for clarity
    int num_total_vertices_from_cpu, 
    int epsilon,
    long long int& host_total_distance_sum, // Renamed for clarity
    int& host_total_distance_count) {     // Renamed for clarity

    host_total_distance_sum = 0;
    host_total_distance_count = 0;

    if (s1_vertices.empty() || s2_vertices.empty() || num_total_vertices_from_cpu == 0) {
        return;
    }
    if (adj_list_vector_cpu.size() < num_total_vertices_from_cpu) {
         // This check might be too strict if num_total_vertices_from_cpu is just larger
         // but adj_list_vector_cpu is still valid for all actual vertex labels used.
         // However, for CSR conversion, offsets array needs to be num_total_vertices_from_cpu + 1.
         // If adj_list_vector_cpu.size() is less, it means some vertices at the end
         // implicitly have no neighbors, which the CSR conversion below handles.
         // The primary concern is if adj_list_vector_cpu.size() is 0 and num_total_vertices_from_cpu > 0.
         // The loop for CSR conversion handles adj_list_vector_cpu.size() < num_total_vertices_from_cpu.
         // So, specific error for adj_list_vector_cpu.empty() && num_total_vertices_from_cpu > 0 might be better.
         // For now, the existing CSR loop logic seems to handle this.
         // Let's ensure adj_list_vector_cpu.size() is at least checked against relevant vertex indices if they could exceed it.
         // The current CSR construction logic assumes adj_list_vector_cpu[i] is safe up to adj_list_vector_cpu.size()
         // and then handles the rest up to num_total_vertices_from_cpu. This is fine.
    }


    // 1. Convert adj_list_vector_cpu to CSR format on CPU
    std::vector<int> h_adj_list_csr; // Flat list of neighbors
    std::vector<int> h_adj_offsets_csr(num_total_vertices_from_cpu + 1);
    
    h_adj_offsets_csr[0] = 0;
    for (int i = 0; i < num_total_vertices_from_cpu; ++i) {
        if (i < adj_list_vector_cpu.size()) { // Check if vertex i exists in adj_list_vector_cpu
            // Before accessing adj_list_vector_cpu[i], ensure all vertex labels within it are valid
            // and < num_total_vertices_from_cpu. This check is implicit in problem context.
            for (Vertex::Label neighbor : adj_list_vector_cpu[i]) {
                h_adj_list_csr.push_back(static_cast<int>(neighbor));
            }
            h_adj_offsets_csr[i+1] = static_cast<int>(h_adj_list_csr.size());
        } else { // Vertex i has no entry in adj_list_vector_cpu (e.g. isolated vertex at end of range)
             h_adj_offsets_csr[i+1] = h_adj_offsets_csr[i]; // No new neighbors, offset remains same
        }
    }

    // 2. GPU Memory Allocation
    Vertex::Label *d_s1_vertices = nullptr, *d_s2_vertices = nullptr;
    int *d_adj_list_csr = nullptr, *d_adj_offsets_csr = nullptr;
    long long int *d_total_dist_sum_gpu = nullptr;
    int *d_total_dist_count_gpu = nullptr;

    CUDA_CHECK(cudaMalloc(&d_s1_vertices, s1_vertices.size() * sizeof(Vertex::Label)));
    CUDA_CHECK(cudaMalloc(&d_s2_vertices, s2_vertices.size() * sizeof(Vertex::Label)));
    // Handle empty h_adj_list_csr case for cudaMalloc/cudaMemcpy
    if (!h_adj_list_csr.empty()) {
      CUDA_CHECK(cudaMalloc(&d_adj_list_csr, h_adj_list_csr.size() * sizeof(int)));
    } else {
      // d_adj_list_csr remains nullptr if there are no edges in the graph.
      // The kernel should handle this (e.g. neighbors_end_idx == neighbors_start_idx).
    }
    CUDA_CHECK(cudaMalloc(&d_adj_offsets_csr, h_adj_offsets_csr.size() * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_total_dist_sum_gpu, sizeof(long long int)));
    CUDA_CHECK(cudaMalloc(&d_total_dist_count_gpu, sizeof(int)));

    // 3. Copy data from Host to Device
    CUDA_CHECK(cudaMemcpy(d_s1_vertices, s1_vertices.data(), s1_vertices.size() * sizeof(Vertex::Label), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_s2_vertices, s2_vertices.data(), s2_vertices.size() * sizeof(Vertex::Label), cudaMemcpyHostToDevice));
    if (!h_adj_list_csr.empty()) {
      CUDA_CHECK(cudaMemcpy(d_adj_list_csr, h_adj_list_csr.data(), h_adj_list_csr.size() * sizeof(int), cudaMemcpyHostToDevice));
    }
    CUDA_CHECK(cudaMemcpy(d_adj_offsets_csr, h_adj_offsets_csr.data(), h_adj_offsets_csr.size() * sizeof(int), cudaMemcpyHostToDevice));
    
    long long int initial_sum = 0;
    int initial_count = 0;
    CUDA_CHECK(cudaMemcpy(d_total_dist_sum_gpu, &initial_sum, sizeof(long long int), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_total_dist_count_gpu, &initial_count, sizeof(int), cudaMemcpyHostToDevice));

    // 4. Kernel Launch Configuration
    dim3 blockDim(16, 16); // 256 threads per block
    dim3 gridDim(
        (s1_vertices.size() + blockDim.x - 1) / blockDim.x,
        (s2_vertices.size() + blockDim.y - 1) / blockDim.y
    );
    int max_depth = 3 * epsilon;

    // 5. Launch Kernel
    pairwise_bfs_kernel<<<gridDim, blockDim>>>(
        d_s1_vertices, static_cast<int>(s1_vertices.size()),
        d_s2_vertices, static_cast<int>(s2_vertices.size()),
        d_adj_list_csr, d_adj_offsets_csr,
        num_total_vertices_from_cpu, max_depth,
        d_total_dist_sum_gpu, d_total_dist_count_gpu
    );
    CUDA_CHECK(cudaPeekAtLastError()); 
    CUDA_CHECK(cudaDeviceSynchronize()); 

    // 6. Copy results from Device to Host
    CUDA_CHECK(cudaMemcpy(&host_total_distance_sum, d_total_dist_sum_gpu, sizeof(long long int), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(&host_total_distance_count, d_total_dist_count_gpu, sizeof(int), cudaMemcpyDeviceToHost));

    // 7. Free GPU Memory
    CUDA_CHECK(cudaFree(d_s1_vertices));
    CUDA_CHECK(cudaFree(d_s2_vertices));
    if (d_adj_list_csr != nullptr) { // Only free if allocated
        CUDA_CHECK(cudaFree(d_adj_list_csr));
    }
    CUDA_CHECK(cudaFree(d_adj_offsets_csr));
    CUDA_CHECK(cudaFree(d_total_dist_sum_gpu));
    CUDA_CHECK(cudaFree(d_total_dist_count_gpu));
}

} // namespace RicciCUDATask

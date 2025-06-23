#include "bfs.cuh"
#include <cuda_runtime.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>
#include <unordered_map>

namespace {
struct DeviceGraph {
    int* offsets;
    int* edges;
    int  numVertices;
};

DeviceGraph d_graph;

__global__ void bfs_kernel(const DeviceGraph g,
                           const int* s1,
                           int s1_size,
                           const int* targetMap,
                           int s2_size,
                           int epsilon,
                           int* distanceOut)
{
    int idx = blockIdx.x;
    if (idx >= s1_size) return;

    extern __shared__ int shared[]; // dynamic shared memory
    int* frontier = shared;
    int* nextFrontier = shared + g.numVertices;
    unsigned char* visited = (unsigned char*)(nextFrontier + g.numVertices);

    for (int i = threadIdx.x; i < g.numVertices; i += blockDim.x) {
        visited[i] = 0;
    }
    __syncthreads();

    int start = s1[idx];
    if (threadIdx.x == 0) {
        frontier[0] = start;
        visited[start] = 1;
    }
    __syncthreads();
    int frontierSize = 1;
    int nextSize = 0;
    int depth = 0;
    while (frontierSize > 0 && depth < 3 * epsilon) {
        for (int f = threadIdx.x; f < frontierSize; f += blockDim.x) {
            int v = frontier[f];
            if (v < g.numVertices) {
                // check if v is target
                if (v < g.numVertices && v < s2_size && targetMap[v]) {
                    atomicAdd(&distanceOut[idx], depth);
                }
                int startOff = g.offsets[v];
                int endOff = g.offsets[v + 1];
                for (int e = startOff; e < endOff; ++e) {
                    int nb = g.edges[e];
                    if (!visited[nb]) {
                        int pos = atomicAdd(&nextSize, 1);
                        nextFrontier[pos] = nb;
                        visited[nb] = 1;
                    }
                }
            }
        }
        __syncthreads();
        // move to next level
        if (threadIdx.x == 0) {
            frontierSize = nextSize;
            nextSize = 0;
            for (int i = 0; i < frontierSize; ++i)
                frontier[i] = nextFrontier[i];
        }
        __syncthreads();
        depth++;
    }
}
} // namespace

void cudaInitGraph(const std::vector<std::vector<Vertex::Label>>& neighbors)
{
    int numVertices = neighbors.size();
    thrust::host_vector<int> h_offsets(numVertices + 1);
    thrust::host_vector<int> h_edges;
    h_edges.reserve(numVertices * 6); // rough

    int off = 0;
    for (int v = 0; v < numVertices; ++v) {
        h_offsets[v] = off;
        for (auto n : neighbors[v]) {
            h_edges.push_back(n);
            off++;
        }
    }
    h_offsets[numVertices] = off;

    d_graph.numVertices = numVertices;
    cudaMalloc(&d_graph.offsets, sizeof(int) * (numVertices + 1));
    cudaMalloc(&d_graph.edges, sizeof(int) * off);

    cudaMemcpy(d_graph.offsets, h_offsets.data(), sizeof(int) * (numVertices + 1), cudaMemcpyHostToDevice);
    cudaMemcpy(d_graph.edges, h_edges.data(), sizeof(int) * off, cudaMemcpyHostToDevice);
}

void cudaFreeGraph()
{
    cudaFree(d_graph.offsets);
    cudaFree(d_graph.edges);
    d_graph.offsets = nullptr;
    d_graph.edges = nullptr;
}

void cudaAverageSphereDistances(const std::vector<Vertex::Label>& s1,
                                const std::vector<Vertex::Label>& s2,
                                int epsilon,
                                std::vector<int>& distanceList)
{
    thrust::device_vector<int> d_s1(s1.begin(), s1.end());
    thrust::device_vector<int> d_targets(d_graph.numVertices, 0);
    for (auto t : s2) d_targets[t] = 1;

    thrust::device_vector<int> d_dist(s1.size(), 0);

    size_t shared = (d_graph.numVertices * 2 + d_graph.numVertices) * sizeof(int);

    bfs_kernel<<<s1.size(), 128, shared>>>(d_graph,
                                          thrust::raw_pointer_cast(d_s1.data()),
                                          s1.size(),
                                          thrust::raw_pointer_cast(d_targets.data()),
                                          d_targets.size(),
                                          epsilon,
                                          thrust::raw_pointer_cast(d_dist.data()));
    thrust::host_vector<int> h_dist = d_dist;
    distanceList.assign(h_dist.begin(), h_dist.end());
}

// Copyright 2023 Google LLC
#pragma once

#include <vector>
#include "../vertex.hpp" // For Vertex::Label

// Forward declaration of Universe_GPU_Data if its definition is complex
// and only pointers/references are used in this header.
// For now, assume it's not needed here, or will be defined elsewhere.
// struct UniverseGPUData; 

namespace RicciCUDATask {

// Wrapper function to be called from Ricci::averageSphereDistance
// Calculates the sum of distances and the count of found pairs using CUDA.
// The final averaging (division by epsilon * count) is done by the caller.
void calculate_sum_and_count_distances_cuda(
    const std::vector<Vertex::Label>& s1_vertices,
    const std::vector<Vertex::Label>& s2_vertices,
    // Universe graph data - consider passing a struct or individual pointers
    // For now, let's assume the graph data (adjacencies) is managed by a separate
    // class or is globally accessible in a GPU-friendly format by the .cu file.
    // This will be refined when implementing the .cu file.
    // For simplicity in this declaration, we might omit direct graph data parameters
    // and assume the .cu file has a way to get it (e.g., from a singleton managing GPU data).
    // However, it's better to be explicit.
    // Let's pass the essential graph structure:
    const std::vector<std::vector<Vertex::Label>>& adj_list_vector, // The CPU version of adjacencies
    int num_total_vertices, // Total number of vertices in the graph
    int epsilon,            // For the 3*epsilon max depth calculation
    long long int& total_distance_sum, // Output: sum of all distances found
    int& total_distance_count      // Output: count of all pairs for which distance was found
);

} // namespace RicciCUDATask

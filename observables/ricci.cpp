// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include <vector>
#include <numeric> // For std::accumulate in original, not needed for CUDA path directly for sum
// #include <unordered_map> // No longer needed for averageSphereDistance
#include "ricci.hpp"
#include "ricci_cuda_kernels.hpp" // New include for CUDA version
#include "../universe.hpp"        // For Universe::vertexNeighbors, Universe::vertices

void Ricci::process() {
    std::vector<double> epsilonDistanceList;
    // The original code created one random origin per epsilon.
    // This seems reasonable to keep.
    std::vector<Vertex::Label> origins;
    for (size_t i = 0; i < epsilons.size(); ++i) {
        if (Universe::vertices.empty()) {
             // Handle case with no vertices to prevent crash in randomVertex()
            epsilonDistanceList.push_back(0.0); // Or some other indicator of error/no data
            continue;
        }
        origins.push_back(randomVertex());
    }
    
    if (Universe::vertices.empty() && !epsilons.empty()) {
        // Fill output with 0.0 if no vertices but epsilons requested
        // This check combined with the loop above means epsilonDistanceList might already be populated.
        // If origins is empty, the main loop below won't run if !epsilons.empty().
        // Let's ensure output is correctly formed if origins couldn't be populated.
        std::string tmp = "";
        for (size_t i = 0; i < epsilons.size(); ++i) {
            // If origins is empty, epsilonDistanceList should have 0.0 for each epsilon from the loop above.
            // If it's not empty, this block is skipped.
            if (origins.empty()) { // This condition means Universe::vertices was empty.
                 tmp += std::to_string(0.0); // Default value consistent with loop above
            } else {
                // This path should not be taken if origins is not empty.
                // The logic flow means if origins is populated, the main calculation loop runs.
                // This block is specifically for the case where Universe::vertices was empty initially.
                // The epsilonDistanceList is already populated by the first loop.
                // So, this block might be redundant if the first loop correctly populates epsilonDistanceList.
                // Let's refine: if origins is empty, the main computation loop won't run correctly.
                // The first loop *conditionally* adds to epsilonDistanceList. If Universe::vertices is empty,
                // it *does* add 0.0 for each epsilon.
                // So the main loop for averageSphereDistance won't run if origins is empty.
                // The formatting of output based on pre-filled epsilonDistanceList is the key.
            }
            // The above logic is a bit tangled. Let's simplify:
            // If Universe::vertices is empty, the first loop populates epsilonDistanceList with 0.0s.
            // The `origins` vector remains empty.
            // The second loop (calculating average distances) will be skipped or needs guarding for empty `origins`.
            // The provided code has a guard `if (i >= origins.size())` inside the second loop.
            // If `origins` is empty, this guard prevents issues.
            // Thus, this specific block for `Universe::vertices.empty() && !epsilons.empty()`
            // is primarily to ensure the `output` string is formed correctly if the main calculation loop
            // doesn't run (because origins is empty).
            // The existing code structure seems to handle this by pre-filling epsilonDistanceList.
        }
        // The main logic for output string generation is at the end of `process`.
        // This block can be removed if the end-of-function formatting loop handles empty epsilonDistanceList
        // or pre-filled epsilonDistanceList correctly.
        // The prompt version of `process` has this block, let's keep it for now.
        // However, if `origins` is empty, the `epsilonDistanceList` *is* populated by the first loop.
        // So, the final string formatting loop will use these 0.0 values.
        // This means this explicit `if (Universe::vertices.empty() && !epsilons.empty())` block to build `tmp`
        // might be redundant or could conflict if not careful.
        // Re-evaluating the prompt's version: it seems this block tries to directly set `output`.
        // If `origins` is empty (because Universe was empty), the first loop *does* fill `epsilonDistanceList`.
        // The second loop is guarded. The final loop *will* build `output` from `epsilonDistanceList`.
        // So this block is indeed redundant. I will remove it for cleaner logic.
        // The first loop handles populating epsilonDistanceList with 0.0 if Universe empty.
        // The final loop handles converting epsilonDistanceList to string.
    }


    for (size_t i = 0; i < epsilons.size(); i++) {
        int epsilon = epsilons[i];
        
        if (i >= origins.size()) { // Guard for when Universe::vertices was empty
            if (epsilonDistanceList.size() <= i) { // Ensure list is large enough if not pre-filled
                 epsilonDistanceList.push_back(0.0); // Should be pre-filled by the first loop
            } else {
                epsilonDistanceList[i] = 0.0; // Ensure it's 0.0
            }
            continue;
        }
        auto origin_p1 = origins[i];

        double averageDistance = averageSphereDistance(origin_p1, epsilon);
        // If origins was populated, epsilonDistanceList might not be.
        // We need to add the calculated distance.
        if (epsilonDistanceList.size() <= i) {
            epsilonDistanceList.push_back(averageDistance);
        } else {
            epsilonDistanceList[i] = averageDistance; // Overwrite if it was pre-filled (e.g. with 0.0)
        }
    }

    std::string tmp_output_str = ""; // Renamed to avoid conflict with member `output`
    for (double dst : epsilonDistanceList) {
        tmp_output_str += std::to_string(dst); // Consider std::fixed and std::setprecision for consistent output
        tmp_output_str += " ";
    }
    if (!tmp_output_str.empty()) {
        tmp_output_str.pop_back();
    }
    output = tmp_output_str; // Assign to member variable
}

double Ricci::averageSphereDistance(Vertex::Label p1, int epsilon) {
    if (Universe::vertices.empty() || Universe::vertexNeighbors.empty()) {
        return 0.0; 
    }
    // Check if p1 is a valid label given the graph size.
    // Universe::vertexNeighbors.size() is num_graph_vertices. Labels should be < this.
    if (p1 >= Universe::vertexNeighbors.size()) {
        // p1 is out of bounds for the graph representation.
        return 0.0; 
    }

    if (epsilon == 0) { 
        return 0.0;
    }

    auto s1 = sphere(p1, epsilon); 
    if (s1.empty()) {
        return 0.0; 
    }

    // Observable::rng is `static std::default_random_engine rng;` (protected)
    std::uniform_int_distribution<> s1_idx_dist(0, s1.size() - 1);
    auto p2 = s1.at(s1_idx_dist(rng)); 

    // Check if p2 is a valid label. sphere() should ensure this.
    if (p2 >= Universe::vertexNeighbors.size()) {
        // p2 selected from s1 is out of bounds. This implies s1 contained an invalid label.
        // sphere() should not return invalid labels relative to Universe::vertexNeighbors.
        return 0.0; 
    }

    auto s2 = sphere(p2, epsilon); 
    if (s2.empty()) {
        return 0.0; 
    }

    long long int total_distance_sum = 0;
    int total_distance_count = 0;

    int num_graph_vertices = static_cast<int>(Universe::vertexNeighbors.size());
    // num_graph_vertices is already implicitly checked by Universe::vertexNeighbors.empty()
    // and the p1/p2 bounds checks.

    RicciCUDATask::calculate_sum_and_count_distances_cuda(
        s1,                            
        s2,                            
        Universe::vertexNeighbors,     
        num_graph_vertices,            
        epsilon,                       
        total_distance_sum,            
        total_distance_count           
    );

    if (total_distance_count == 0) { // Epsilon is non-zero here due to earlier check
        return 0.0; 
    }

    double averageDistance = static_cast<double>(total_distance_sum) / static_cast<double>(epsilon * total_distance_count);

    return averageDistance;
}

Ricci::Ricci(std::string ID, std::vector<int> eps) : Observable(ID), epsilons(eps) {
	name = "ricci";
}

#pragma once
#include <vector>
#include "../vertex.hpp"

// Initialize GPU adjacency structures from CPU neighbor lists
void cudaInitGraph(const std::vector<std::vector<Vertex::Label>>& neighbors);
void cudaFreeGraph();

// Compute BFS distances for the Ricci observable
// 's1' and 's2' are vertex lists residing on the host. Distances from each
// vertex in s1 to any vertex in s2 (up to 3*epsilon) are written into
// distanceList, matching the CPU implementation.
void cudaAverageSphereDistances(const std::vector<Vertex::Label>& s1,
                                const std::vector<Vertex::Label>& s2,
                                int epsilon,
                                std::vector<int>& distanceList);

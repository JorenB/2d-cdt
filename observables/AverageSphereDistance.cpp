// Copyright 2023 Google LLC
#include "AverageSphereDistance.hpp"

#include <string>
#include <vector>
#include <numeric> // For std::accumulate
#include <iomanip> // For std::fixed and std::setprecision
#include <sstream> // For std::ostringstream

// Required for Observable::sphere, Observable::distance, Observable::randomVertex
#include "../universe.hpp" 


AverageSphereDistance::AverageSphereDistance(std::string id, int radius, int num_origins)
    : Observable(id), radius(radius), num_origins(num_origins) {
    name = "average_sphere_distance";
}

void AverageSphereDistance::process() {
    double total_average_distance = 0.0;
    int valid_origins_count = 0;

    if (Universe::vertices.empty()) {
        output = "No vertices in universe.";
        return;
    }

    for (int i = 0; i < num_origins; ++i) {
        Vertex::Label origin = Observable::randomVertex();
        std::vector<Vertex::Label> sphere_vertices = Observable::sphere(origin, radius);

        if (sphere_vertices.empty()) {
            continue;
        }

        double current_sum_of_distances = 0.0;
        for (Vertex::Label v_label : sphere_vertices) {
            // Ensure distance is not -1 (no path) before adding
            int dist = Observable::distance(origin, v_label);
            if (dist != -1) {
                 current_sum_of_distances += dist;
            } else {
                // If any vertex in the sphere is unreachable from the origin,
                // this sphere calculation might be considered invalid.
                // For now, we just skip this distance, but this could be a point of refinement.
            }
        }
        
        // Recalculate sphere_vertices.size() in case some distances were -1 and skipped.
        // However, the current logic means we average over those reachable.
        // A stricter interpretation might require all vertices in the initially found sphere to be reachable.
        // For now, we stick to the simpler average over reachable ones within the sphere.

        if (!sphere_vertices.empty()) { // ensure sphere is not empty after potential skips if we were to filter
            double current_origin_average = current_sum_of_distances / sphere_vertices.size();
            total_average_distance += current_origin_average;
            valid_origins_count++;
        }
    }

    if (valid_origins_count > 0) {
        double final_average = total_average_distance / valid_origins_count;
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(10) << final_average;
        output = stream.str();
    } else {
        // Adjusted message to be more specific if num_origins > 0
        if (num_origins > 0) {
            output = "No non-empty spheres found for the given parameters.";
        } else {
            output = "Number of origins is zero.";
        }
    }
}

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include <vector>
#include <unordered_map>
#include "ricci.hpp"

void Ricci::process() {
    std::vector<double> epsilonDistanceList;
	std::vector<Vertex::Label> origins;

	for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
		origins.push_back(randomVertex());
	}

	for (int i = 0; i < epsilons.size(); i++) {
		int epsilon = epsilons[i];
		// printf("%d - ", epsilon);

		auto origin = origins[i];

        double averageDistance = averageSphereDistance(origin, epsilon);
        epsilonDistanceList.push_back(averageDistance);

		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";
    for (double dst : epsilonDistanceList) {
        tmp += std::to_string(dst);
        tmp += " ";
    }
	tmp.pop_back();
    output = tmp;
}

double Ricci::averageSphereDistance(Vertex::Label p1, int epsilon) {
    auto s1 = sphere(p1, epsilon);
	std::uniform_int_distribution<> rv(0, s1.size()-1);
    auto p2 = s1.at(rv(rng));
    auto s2 = sphere(p2, epsilon);
    std::unordered_map<int, Vertex::Label> vertexMap;

    std::vector<int> distanceList;
    for (auto b : s1) {
        vertexMap.clear();
        for (auto v : s2) {
            vertexMap[v] = v;
        }
        std::vector<Vertex::Label> done;
        std::vector<Vertex::Label> thisDepth;
        std::vector<Vertex::Label> nextDepth;

        done.push_back(b);
        thisDepth.push_back(b);


        for (int currentDepth = 0; currentDepth < 3 * epsilon; currentDepth++) {
            for (auto v : thisDepth) {
                if (vertexMap.find(v) != vertexMap.end()) {
                    distanceList.push_back(0);
                    vertexMap.erase(v);
                }
                for (auto neighbor : Universe::vertexNeighbors[v]) {
                    if (std::find(done.begin(), done.end(), neighbor) == done.end()) {
                        nextDepth.push_back(neighbor);
                        done.push_back(neighbor);

                        if (vertexMap.find(neighbor) != vertexMap.end()) {
                            distanceList.push_back(currentDepth+1);
                            vertexMap.erase(neighbor);
                        }
                    }
                    if (vertexMap.size() == 0) break;
                }
                if (vertexMap.size() == 0) break;
            }
            thisDepth = nextDepth;
            nextDepth.clear();
            if (vertexMap.size() == 0) break;
        }
    }

    int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
    double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size());

    return averageDistance;
}

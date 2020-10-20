// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <unordered_map>
#include <string>
#include <vector>
#include "ricci_dual.hpp"

void RicciDual::process() {
    std::vector<double> epsilonDistanceList;
	std::vector<Triangle::Label> origins;

	for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
		origins.push_back(Universe::trianglesAll.pick());
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

double RicciDual::averageSphereDistance(Triangle::Label t1, int epsilon) {
    auto s1 = sphereDual(t1, epsilon);
	std::uniform_int_distribution<> rv(0, s1.size() - 1);
    auto t2 = s1.at(rv(rng));
    auto s2 = sphereDual(t2, epsilon);
    std::unordered_map<int, Triangle::Label> triangleMap;

    std::vector<int> distanceList;
    for (auto b : s1) {
        triangleMap.clear();
        for (auto v : s2) {
            triangleMap[v] = v;
        }
        std::vector<Triangle::Label> done;
        std::vector<Triangle::Label> thisDepth;
        std::vector<Triangle::Label> nextDepth;

        done.push_back(b);
        thisDepth.push_back(b);

        for (int currentDepth = 0; currentDepth < 3*epsilon; currentDepth++) {
            for (auto v : thisDepth) {
                if (triangleMap.find(v) != triangleMap.end()) {
                    distanceList.push_back(0);
                    triangleMap.erase(v);
                }
                for (auto neighbor : Universe::triangleNeighbors[v]) {
                    if (std::find(done.begin(), done.end(), neighbor) == done.end()) {
                        nextDepth.push_back(neighbor);
                        done.push_back(neighbor);

                        if (triangleMap.find(neighbor) != triangleMap.end()) {
                            distanceList.push_back(currentDepth+1);
                            triangleMap.erase(neighbor);
                        }
                    }
                    if (triangleMap.size() == 0) break;
                }
                if (triangleMap.size() == 0) break;
            }
            thisDepth = nextDepth;
            nextDepth.clear();
            if (triangleMap.size() == 0) break;
        }
    }

    int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
    double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size());

    return averageDistance;
}

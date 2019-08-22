#include <iostream>
#include <fstream>
#include "observable.hpp"

std::default_random_engine Observable::rng(0);  // TODO: seed properly

void Observable::write() {
    std::string filename = data_dir + name + "-" + identifier + extension;

	std::ifstream infile(filename);
	if (!infile.good()) { printf("output file deleted\n"); exit(1); }
	infile.close();

    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::app);

	assert(file.is_open());

	file << output << "\n";
	file.close();

    std::cout << filename << "\n";
}

void Observable::clear() {
    std::string filename = data_dir + name + "-" + identifier + extension;

    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::trunc);
	
	assert(file.is_open());

	file.close();
}

std::vector<Vertex::Label> Observable::sphere(Vertex::Label origin, int radius) {
	std::vector<Vertex::Label> done;
    std::vector<Vertex::Label> thisDepth;
    std::vector<Vertex::Label> nextDepth;

    done.push_back(origin);
    thisDepth.push_back(origin);

    std::vector<Vertex::Label> vertexList;

    for (int currentDepth = 0; currentDepth < radius; currentDepth++) {
        for (auto v : thisDepth) {
            for (auto neighbor : Universe::vertexNeighbors[v]) {
               if (std::find(done.begin(), done.end(), neighbor) == done.end()) {
                   nextDepth.push_back(neighbor);
                   done.push_back(neighbor);
                   if(currentDepth == radius-1) vertexList.push_back(neighbor);
               }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
    }

    return vertexList;

}

std::vector<Triangle::Label> Observable::sphereDual(Triangle::Label origin, int radius) {
	std::vector<Triangle::Label> done;
    std::vector<Triangle::Label> thisDepth;
    std::vector<Triangle::Label> nextDepth;

    done.push_back(origin);
    thisDepth.push_back(origin);

    std::vector<Triangle::Label> triangleList;

    for (int currentDepth = 0; currentDepth < radius; currentDepth++) {
        for (auto t : thisDepth) {
            for (auto neighbor : Universe::triangleNeighbors[t]) {
               if (std::find(done.begin(), done.end(), neighbor) == done.end()) {
                   nextDepth.push_back(neighbor);
                   done.push_back(neighbor);
                   if(currentDepth == radius-1) triangleList.push_back(neighbor);
               }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
    }

    return triangleList;

}

int Observable::distance(Vertex::Label v1, Vertex::Label v2) {
    if(v1 == v2) return 0;

    std::vector<Vertex::Label> done;
    std::vector<Vertex::Label> thisDepth;
    std::vector<Vertex::Label> nextDepth;

    done.push_back(v1);
    thisDepth.push_back(v1);

    int currentDepth = 0;
    while(true) {
        for(auto v : thisDepth) {
            for(auto neighbor : Universe::vertexNeighbors[v]) {
                if(neighbor == v2) return currentDepth+1;

                if(std::find(done.begin(), done.end(), neighbor) == done.end()) {
                    nextDepth.push_back(neighbor);
                    done.push_back(neighbor);
                }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
        currentDepth++;
    }
}

int Observable::distanceDual(Triangle::Label t1, Triangle::Label t2) {
    if(t1 == t2) return 0;

    std::vector<Triangle::Label> done;
    std::vector<Triangle::Label> thisDepth;
    std::vector<Triangle::Label> nextDepth;

    done.push_back(t1);
    thisDepth.push_back(t1);

    int currentDepth = 0;
    while(true) {
        for(auto t : thisDepth) {
            for(auto neighbor : Universe::triangleNeighbors[t]) {
                if(neighbor == t2) return currentDepth+1;

                if(std::find(done.begin(), done.end(), neighbor) == done.end()) {
                    nextDepth.push_back(neighbor);
                    done.push_back(neighbor);
                }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
        currentDepth++;
    }
}

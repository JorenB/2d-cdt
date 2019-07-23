// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef UNIVERSE_HPP_
#define UNIVERSE_HPP_

#include <vector>
#include <random>
#include "vertex.hpp"
#include "triangle.hpp"
#include "pool.hpp"
#include "bag.hpp"

#define N_TRIANGLES 2000000
#define N_VERTICES 1000000

class Universe {
public:
	int nSlices;
	std::vector<int> sliceSizes;

	Pool<Triangle, N_TRIANGLES> triangles;
	Pool<Vertex, N_VERTICES> vertices;

	Bag<Triangle, N_TRIANGLES> trianglesAll;  // All triangles. These are candidates for the add move
	Bag<Vertex, N_VERTICES> verticesFour;  // Vertices with coordination number 4. These are candidates for the delete move
	Bag<Vertex, N_VERTICES> verticesPlus;  // Vertices with more than two triangles above. These are candidates for the flip move

	Universe(int n_slices, int seed);

	void initialize();

	static Universe* create(int n_slices, int seed);

	~Universe() {
		printf("Destructing Universe with %d vertices and %d triangles\n", vertices.size(), triangles.size());
	}

	// moves
	void insertVertex(Triangle& t);
	void removeVertex(Vertex& v);
	enum flipSide { LEFT, RIGHT };
	void flipLink(Vertex& v, flipSide side);

	// bag consistency
	void updateVertexCoord(Vertex &v, int up, int down);

private:
	std::default_random_engine rng;
};
#endif  // UNIVERSE_HPP_

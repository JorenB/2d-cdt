// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef UNIVERSE_HPP_
#define UNIVERSE_HPP_

#include <vector>
#include <random>
#include "vertex.hpp"
#include "triangle.hpp"
//#include "pool.hpp"
//#include "bag.hpp"

class Universe {
public:
	static int nSlices;
	static std::vector<int> sliceSizes;

	/*
	Bag<Triangle, N_TRIANGLES> trianglesAll;  // All triangles. These are candidates for the add move
	Bag<Vertex, N_VERTICES> verticesFour;  // Vertices with coordination number 4. These are candidates for the delete move
	Bag<Vertex, N_VERTICES> verticesPlus;  // Vertices with more than two triangles above. These are candidates for the flip move
	*/

	static void initialize();

	static void create(int n_slices, int seed);

	/*
	// moves
	void insertVertex(Triangle& t);
	void removeVertex(Vertex& v);
	enum flipSide { LEFT, RIGHT };
	void flipLink(Vertex& v, flipSide side);

	// bag consistency
	void updateVertexCoord(Vertex &v, int up, int down);
	*/

private:
	Universe() {}
	static std::default_random_engine rng;
};
#endif  // UNIVERSE_HPP_

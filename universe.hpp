// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <vector>
#include <random>
#include "vertex.hpp"
#include "triangle.hpp"
#include "pool.hpp"
#include "bag.hpp"

class Universe {
public:
	static int nSlices;
	static std::vector<int> sliceSizes;
	static bool sphere;

	static Bag<Triangle, Triangle::pool_size> trianglesAll;  // All triangles. These are candidates for the add move
	static Bag<Vertex, Vertex::pool_size> verticesFour;  // Vertices with coordination number 4. These are candidates for the delete move
	static Bag<Vertex, Vertex::pool_size> verticesPlus;  // Vertices with more than two triangles above. These are candidates for the flip move

	static void initialize();

	static void create(int n_slices);

	// moves
	static void insertVertex(Triangle::Label t);
	static void removeVertex(Vertex::Label v);
	enum flipSide { LEFT, RIGHT };
	static void flipLink(Vertex::Label v, flipSide side);

	// bag consistency
	static void updateVertexCoord(Vertex::Label v, int up, int down);

	static void check();

	static void updateVertexData();
	static void updateTriangleData();

	static std::vector<Vertex::Label> vertices;
	static std::vector<Triangle::Label> triangles;
	static std::vector<std::vector<Vertex::Label>> vertexNeighbors;
	static std::vector<std::vector<Triangle::Label>> triangleNeighbors;

private:
	Universe() {}
	static std::default_random_engine rng;
};

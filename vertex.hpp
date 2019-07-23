// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <vector>
#include <unordered_map>
#include "simplex.hpp"

class Triangle;
class Vertex : public Simplex {
public:
	int time;  // Slice number
	int nUp;  // No. shared triangles in upper hemisphere
	int nDown;  // No. shared triangles in lower hemisphere

	void changeCoord(int up, int down) {
		nUp += up;
		nDown += down;
	}

	void setCoord(int up, int down) {
		nUp = up;
		nDown = down;
	}

	Triangle& getTriangleLeft() { return *tl; }
	Triangle& getTriangleRight() { return *tr; }

	void setTriangleLeft(Triangle &t)  { tl = &t; }
	void setTriangleRight(Triangle &t) { tr = &t; }

private:
	Triangle *tl, *tr;
};

#endif  // VERTEX_HPP_

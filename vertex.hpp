// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef vertex_hpp
#define vertex_hpp

#include "simplex.hpp"

class Triangle;
class Vertex : public Simplex {
public:
	int time;

	int neighborsUp, neighborsDown;  // Number of triangles above and below sharing this vertex

	void changeNeighborNumber(int up, int down) {
		neighborsUp += up;
		neighborsDown += down;
	}

	void setNeighborNumber(int up, int down) {
		neighborsUp = up;
		neighborsDown = down;
	}

	Triangle& getTriangleLeft() { return *tl; }
	Triangle& getTriangleRight() { return *tr; }

	void setTriangleLeft(Triangle &t)  { tl = &t; }
	void setTriangleRight(Triangle &t) { tr = &t; }

private:
	Triangle *tl, *tr;
};

#endif

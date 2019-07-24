// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include "pool.hpp"

class Triangle;
class Vertex : public Pool<Vertex> {
public:
	static const unsigned pool_size = 1000;
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

	Triangle& getTriangleLeft();
	Triangle& getTriangleRight();

	void setTriangleLeft(Triangle &t);
	void setTriangleRight(Triangle &t);

private:
	int tl, tr;
};

#endif  // VERTEX_HPP_

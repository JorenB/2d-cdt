// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
//#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"

int main(int argc, const char * argv[]) {
	Triangle t1 = Triangle::at(Triangle::create());
	Triangle t2 = Triangle::at(Triangle::create());
	Triangle t3 = Triangle::at(Triangle::create());
	Triangle t4 = Triangle::at(Triangle::create());
	t1.setTriangles(t2, t3, t4);

	Vertex& v1 = Vertex::at(Vertex::create());
	Vertex& v2 = Vertex::at(Vertex::create());
	Vertex& v3 = Vertex::at(Vertex::create());
	Vertex& v4 = Vertex::at(Vertex::create());
	v1.time = 1;
	v2.time = 1;
	v3.time = 2;
	v3.time = 2;

	t1.setVertices(v1,v2,v3);
	t2.setVertices(v3,v4,v1);
		
	printf("t1: %c\n", t1.isUpwards() ? 'u' : 'd');
	printf("t2: %c\n", t2.isUpwards() ? 'u' : 'd');

	Universe::create(10,1);

	
	return 0;
}

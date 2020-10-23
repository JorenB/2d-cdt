// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#ifndef LINK_HPP_
#define LINK_HPP_

#include "pool.hpp"

class Vertex;
class Triangle;
class Link : public Pool<Link> {
public:
	static const unsigned pool_size = 10000000;

	Pool<Vertex>::Label getVertexFinal();
	Pool<Vertex>::Label getVertexInitial();

	Pool<Triangle>::Label getTrianglePlus();
	Pool<Triangle>::Label getTriangleMinus();

	void setVertices(Pool<Vertex>::Label vi, Pool<Vertex>::Label vf);
	void setTriangles(Pool<Triangle>::Label tp, Pool<Triangle>::Label tm);

	bool isTimelike();
	bool isSpacelike();

private:
	Pool<Vertex>::Label vi, vf;  // vertices
	Pool<Triangle>::Label tp, tm;  // triangles
};

#endif  // LINK_HPP_

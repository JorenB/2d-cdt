// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"

int main(int argc, const char * argv[]) {
	Universe::create(10, 1);

	/*Triangle::Label t = Pool<Triangle>::at(1);
	printf("t: %d, vr: %d\n", (int) t, (int) t->getVertexRight());

	Universe::insertVertex(t);
	
	printf("t: %d, vr: %d\n", (int) t, (int) t->getVertexRight());

	Universe::removeVertex(t->getVertexRight());

	printf("t: %d, vr: %d\n", (int) t, (int) t->getVertexRight());

	auto v = t->getVertexLeft();
	auto vl = v->getTriangleLeft()->getVertexLeft();
	printf("v: %d, nUp: %d, nDown: %d\n", (int) v, v->nUp, v->nDown);
	Universe::flipLink(v, Universe::LEFT);
	printf("v: %d, nUp: %d, nDown: %d\n", (int) v, v->nUp, v->nDown);
	Universe::flipLink(vl, Universe::RIGHT);
	printf("v: %d, nUp: %d, nDown: %d\n", (int) v, v->nUp, v->nDown);

	auto t1 = Universe::trianglesAll.pick();
	printf("t1: %d, vr: %d\n", (int) t1, (int) t1->getVertexRight());
*/
	for (int i = 0; i < 1000000; i++) {
		if (Universe::verticesFour.size() > 0) {
			auto v = Universe::verticesFour.pick();
			if (Universe::sliceSizes[v->time] >= 4) Universe::removeVertex(v);
		}
		auto v = Universe::verticesPlus.pick();
		printf("v: %d, nUp: %d, nDown: %d\n", (int) v, v->nUp, v->nDown);
		Universe::flipLink(v, i%2 == 0 ? Universe::LEFT : Universe::RIGHT);
		printf("v: %d, nUp: %d, nDown: %d\n", (int) v, v->nUp, v->nDown);
		auto t = Universe::trianglesAll.pick();
		Universe::insertVertex(t);
		printf("i: %d\n", i);
	}

	printf("universe now has %d vertices\n", Vertex::size());

	return 0;
}

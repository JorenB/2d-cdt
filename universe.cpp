// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "universe.hpp"
#include <stdio.h>

Universe::Universe(int nSlices, int seed) 
	: nSlices(nSlices), sliceSizes(nSlices), rng(seed),
	verticesDelete(rng), verticesFlip(rng) {} 

Universe* Universe::create(int n_slices, int seed) {
	Universe *u = new Universe(n_slices, seed);
	u->initialize();
	return u;
}

void Universe::initialize() {
	int w = 10;  // width of the initial strip. Can be adjusted for thermalization purposes - unclear what the 'optimal' value is.
	int t = nSlices;

	std::vector<Vertex*> initialVertices(w*t);

	for (int i = 0; i < w*t; i++) {
		Vertex *v = &vertices.create();
		v->time = i / w;
		v->setNeighborNumber(2, 2);
		initialVertices[i] = v;
		verticesFlip.add(*v);
	}

	for (int i = 0; i < t; i++) {
		sliceSizes.push_back(w);
	}

	// create triangles
	std::vector<Triangle*> initialTriangles(2*w*t);
	for(int i = 0; i < t; i++) {
		for(int j = 0; j < w; j++) {
			Triangle& tl = triangles.create();
			tl.setVertices(
					*initialVertices[i*w+j], 
					*initialVertices[i*w+(j+1)%w], 
					*initialVertices[((i+1)%t)*w+j]
					);
			initialTriangles[2*(i*w+j)] = &tl;

			Triangle& tr = triangles.create();
			tr.setVertices(
					*initialVertices[((i+1)%t)*w+j],
					*initialVertices[((i+1)%t)*w+(j+1)%w],
					*initialVertices[i*w+(j+1)%w]
					);
			initialTriangles[2*(i*w+j)+1] = &tr;
		}
	}

	// set triangle connectivity
	int row = 0, column = 0;
	for(int i = 0; i < t; ++i) {
		for(int j = 0; j < w; ++j) {
			row = 2*i*w;
			column = 2*j;
			initialTriangles[row + column]->setTriangles(
					*initialTriangles[row + (column-1+2*w)%(2*w)],
					*initialTriangles[row + column + 1],
					*initialTriangles[(row + column -2*w+1+2*t*w)%(2*t*w)]
					);

			initialTriangles[row + column +1]->setTriangles(
					*initialTriangles[row + column],
					*initialTriangles[row + (column+2)%(2*w)],
					*initialTriangles[(row + column + 2*w)%(2*t*w)]
					);
		}
	}

	/*Triangle& tr = triangles[10];
	// useful logging string for triangle
	printf("tri: %d; time: %d, type: %s, tl: %d, tr: %d, tc: %d, vl: %d, vr: %d, vc: %d\n", tr.key(),
	  tr.time, 
	  tr.type == Triangle::Type::UP ? "up" : "down",
	  tr.getTriangleLeft().key(),
	  tr.getTriangleRight().key(),
	  tr.getTriangleCenter().key(),
	  tr.getVertexLeft().key(), 
	  tr.getVertexRight().key(), 
	  tr.getVertexCenter().key());
	  */

	// useful logging string for vertex
	//printf("v: %d; tl: %d, tr: %d, nu: %d, nd: %d\n", v.getKey(), v.getTriangleLeft().getKey(), v.getTriangleRight().getKey(), v.neighborsUp, v.neighborsDown);
}


void Universe::moveAdd(Triangle& t) {
	Triangle& tc = t.getTriangleCenter();

	Vertex& vr = t.getVertexRight();

	int time = t.time;

	Vertex& v = vertices.create();
	v.time = time;
	v.setNeighborNumber(1, 1);
	verticesDelete.add(v);  // replace by updateVertexBags(v)?
	sliceSizes[time] += 1;

	t.setVertexRight(v);
	tc.setVertexRight(v);

	Triangle& t1 = triangles.create();  // right neighbor of t
	Triangle& t2 = triangles.create();  // right neighbor of tc

	t1.setVertices(v, vr, t.getVertexCenter());
	t2.setVertices(v, vr, tc.getVertexCenter());

	t1.setTriangles(t, t.getTriangleRight(), t2);
	t2.setTriangles(tc, tc.getTriangleRight(), t1);

	if (t.isUpwards()) {
		t.getVertexCenter().changeNeighborNumber(0, 1);
		tc.getVertexCenter().changeNeighborNumber(1, 0);
	} else if (t.isDownwards()) {
		t.getVertexCenter().changeNeighborNumber(1, 0);
		tc.getVertexCenter().changeNeighborNumber(0, 1);
	}

	updateVertexBags(t.getVertexCenter());
	updateVertexBags(tc.getVertexCenter());
}

void Universe::moveDelete(Vertex& v) {
	Triangle& tl = v.getTriangleLeft();
	Triangle& tr = v.getTriangleRight();
	Triangle& tlc = tl.getTriangleCenter();
	Triangle& trc = tr.getTriangleCenter();

	Triangle& trn = tr.getTriangleLeft();
	Triangle& trcn = trc.getTriangleLeft();

	tl.setTriangleRight(trn);
	tlc.setTriangleRight(trcn);

	tl.setVertexRight(tr.getVertexRight());
	tlc.setVertexRight(tr.getVertexRight());

	tr.getVertexRight().setTriangleLeft(tl);

	sliceSizes[v.time] -= 1;

	triangles.destroy(tr);
	triangles.destroy(trc);

	verticesDelete.remove(v);  // replace by updateVertexBags(v)?
	vertices.destroy(v);
}

void Universe::moveFlip(Vertex& v, flipSide side) {
	if (side == LEFT) {
		Triangle& tl = v.getTriangleLeft();
		Triangle& tr = tl.getTriangleRight();

		Vertex& vl = tl.getVertexLeft();
		Vertex& vlu = tr.getVertexLeft();
		Vertex& vru = tr.getVertexRight();

		triangles.destroy(tl);
		triangles.destroy(tr);

		Triangle& tln = triangles.create();
		Triangle& trn = triangles.create();

		tln.setVertices(vlu, vru, vl);
		trn.setVertices(vl, v, vru);

		// v.changeNeighborNumber(-1, 0);
		// vl.changeNeighborNumber(1, 0);
		// vlu.changeNeighborNumber(0, 1);
		// vru.changeNeighborNumber(0, -1);

		// updateVertexBags(v);
		// updateVertexBags(vl);
		// updateVertexBags(vlu);
		// updateVertexBags(vru);

		updateNeighborNumber(v, -1, 0);
		updateNeighborNumber(vl, 1, 0);
		updateNeighborNumber(vlu, 0, 1);
		updateNeighborNumber(vru, 0, -1);
	} else if (side == RIGHT) {
		Triangle& tr = v.getTriangleRight();
		Triangle& tl = tr.getTriangleLeft();

		Vertex& vr = tr.getVertexRight();
		Vertex& vlu = tl.getVertexLeft();
		Vertex& vru = tl.getVertexRight();

		triangles.destroy(tl);
		triangles.destroy(tr);

		Triangle& tln = triangles.create();
		Triangle& trn = triangles.create();

		tln.setVertices(v, vr, vlu);
		trn.setVertices(vlu, vru, vr);

		v.changeNeighborNumber(-1, 0);
		vr.changeNeighborNumber(1, 0);
		vlu.changeNeighborNumber(0, 1);
		vru.changeNeighborNumber(0, -1);

		updateVertexBags(v);
		updateVertexBags(vr);
		updateVertexBags(vlu);
		updateVertexBags(vru);
	}
}

void Universe::updateNeighborNumber(Vertex &v, int up, int down) {
	if(v.neighborsUp + v.neighborsDown == 2)  // Since up != 0 or down !=, no need to check if contains
		verticesDelete.remove(v);

	if ((v.neighborsUp > 2) && (v.neighborsUp + up == 2))
		verticesFlip.remove(v);

	if ((up > 0) && (v.neighborsUp == 2))
		verticesFlip.add(v);

	v.changeNeighborNumber(up, down);

	if (v.neighborsUp + v.neighborsDown == 2)  // Since up != 0 or down !=, no need to check if does not contain
		verticesDelete.add(v);
}


void Universe::updateVertexBags(Vertex& v) {
	if (v.neighborsUp < 2 && verticesFlip.contains(v)) { verticesFlip.remove(v); }
	if (v.neighborsUp >= 2 && !verticesFlip.contains(v)) { verticesFlip.add(v); }

	if ((v.neighborsUp + v.neighborsDown == 2) && !verticesDelete.contains(v)) { verticesDelete.add(v); }
	if ((v.neighborsUp + v.neighborsDown != 2) && verticesDelete.contains(v)) { verticesDelete.remove(v); }
}

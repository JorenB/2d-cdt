// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "universe.hpp"

int Universe::nSlices = 0;
std::vector<int> Universe::sliceSizes;
std::default_random_engine Universe::rng(0);  // TODO: set seed somewhere else
Bag<Triangle, Triangle::pool_size> Universe::trianglesAll(rng);
Bag<Vertex, Vertex::pool_size> Universe::verticesFour(rng);
Bag<Vertex, Vertex::pool_size> Universe::verticesPlus(rng);

void Universe::create(int nSlices_, int seed_) {
	nSlices = nSlices_;
	initialize();
}

void Universe::initialize() {
	int w = 10;  // width of the initial strip. Can be adjusted for thermalization purposes - unclear what the 'optimal' value is.
	int t = nSlices;

	std::vector<Vertex::Label> initialVertices(w*t);

	for (int i = 0; i < w*t; i++) {
		auto v = Vertex::create();
		v->time = i / w;
		v->setCoord(3, 3);
		initialVertices[i] = v;
		verticesPlus.add(v);
	}

	for (int i = 0; i < t; i++) {
		sliceSizes.push_back(w);
	}

	printf("u: %d\n", nSlices);

	// create triangles
	std::vector<Triangle::Label> initialTriangles(2*w*t);
	for(int i = 0; i < t; i++) {
		for(int j = 0; j < w; j++) {
			auto tl = Triangle::create();
			tl->setVertices(
					initialVertices[i*w+j],
					initialVertices[i*w+(j+1)%w],
					initialVertices[((i+1)%t)*w+j]);
			initialTriangles[2*(i*w+j)] = tl;

			auto tr = Triangle::create();
			tr->setVertices(
					initialVertices[((i+1)%t)*w+j],
					initialVertices[((i+1)%t)*w+(j+1)%w],
					initialVertices[i*w+(j+1)%w]);
			initialTriangles[2*(i*w+j)+1] = tr;
			trianglesAll.add(tl);
			trianglesAll.add(tr);
		}
	}
	
	// set triangle connectivity
	int row = 0, column = 0;
	for(int i = 0; i < t; ++i) {
		for(int j = 0; j < w; ++j) {
			row = 2*i*w;
			column = 2*j;
			initialTriangles[row + column]->setTriangles(
					initialTriangles[row + (column-1+2*w)%(2*w)],
					initialTriangles[row + column + 1],
					initialTriangles[(row + column -2*w+1+2*t*w)%(2*t*w)]);

			initialTriangles[row + column +1]->setTriangles(
					initialTriangles[row + column],
					initialTriangles[row + (column+2)%(2*w)],
					initialTriangles[(row + column + 2*w)%(2*t*w)]);
		}
	}
}


void Universe::insertVertex(Triangle::Label t) {
	Triangle::Label tc = t->getTriangleCenter();

	Vertex::Label vr = t->getVertexRight();

	int time = t->time;

	Vertex::Label v = Vertex::create();
	v->time = time;
	v->setCoord(2, 2);
	verticesFour.add(v);
	sliceSizes[time] += 1;

	t->setVertexRight(v);
	tc->setVertexRight(v);

	Triangle::Label t1 = Triangle::create();  // right neighbor of t
	Triangle::Label t2 = Triangle::create();  // right neighbor of tc
	trianglesAll.add(t1);
	trianglesAll.add(t2);

	t1->setVertices(v, vr, t->getVertexCenter());
	t2->setVertices(v, vr, tc->getVertexCenter());

	t1->setTriangles(t, t->getTriangleRight(), t2);
	t2->setTriangles(tc, tc->getTriangleRight(), t1);

	if (t->isUpwards()) {
		updateVertexCoord(t->getVertexCenter(), 0, 1);
		updateVertexCoord(tc->getVertexCenter(), 1, 0);
	} else if (t->isDownwards()) {
		updateVertexCoord(t->getVertexCenter(), 1, 0);
		updateVertexCoord(tc->getVertexCenter(), 0, 1);
	}
}

void Universe::removeVertex(Vertex::Label v) {
	Triangle::Label tl = v->getTriangleLeft();
	Triangle::Label tr = v->getTriangleRight();
	Triangle::Label tlc = tl->getTriangleCenter();
	Triangle::Label trc = tr->getTriangleCenter();

	Triangle::Label trn = tr->getTriangleRight();
	Triangle::Label trcn = trc->getTriangleRight();

	tl->setTriangleRight(trn);
	tlc->setTriangleRight(trcn);

	tl->setVertexRight(tr->getVertexRight());
	tlc->setVertexRight(tr->getVertexRight());

	tr->getVertexRight()->setTriangleLeft(tl);

	sliceSizes[v->time] -= 1;

	trianglesAll.remove(tr);
	trianglesAll.remove(trc);
	Triangle::destroy(tr);
	Triangle::destroy(trc);

	Vertex::Label vu = tl->getVertexCenter();
	Vertex::Label vd = tlc->getVertexCenter();

	updateVertexCoord(vu, 0, -1);
	updateVertexCoord(vd, -1, 0);

	verticesFour.remove(v);
	Vertex::destroy(v);
}

void Universe::flipLink(Vertex::Label v, flipSide side) {
	std::bernoulli_distribution d(0.5);

	// simplify and add check
	if (side == LEFT) {
		Triangle::Label tl = v->getTriangleLeft();
		Triangle::Label tr = tl->getTriangleRight();
		Triangle::Label tll = tl->getTriangleLeft();
		Triangle::Label trr = tr->getTriangleRight();
		Triangle::Label tlc = tl->getTriangleCenter();
		Triangle::Label trc = tr->getTriangleCenter();

		Vertex::Label vl = tl->getVertexLeft();
		Vertex::Label vlu = tr->getVertexLeft();
		Vertex::Label vru = tr->getVertexRight();

		trianglesAll.remove(tl);
		trianglesAll.remove(tr);
		Triangle::destroy(tl);
		Triangle::destroy(tr);

		Triangle::Label tln = Triangle::create();
		Triangle::Label trn = Triangle::create();
		trianglesAll.add(tln);
		trianglesAll.add(trn);

		tln->setVertices(vlu, vru, vl);
		trn->setVertices(vl, v, vru);

		tln->setTriangleLeft(tll);
		tln->setTriangleRight(trn);
		trn->setTriangleRight(trr);
		tln->setTriangleCenter(trc);
		trn->setTriangleCenter(tlc);

		updateVertexCoord(v, -1, 0);
		updateVertexCoord(vl, 1, 0);
		updateVertexCoord(vlu, 0, -1);
		updateVertexCoord(vru, 0, 1);
	} else if (side == RIGHT) {
		Triangle::Label tr = v->getTriangleRight();
		Triangle::Label tl = tr->getTriangleLeft();
		Triangle::Label tll = tl->getTriangleLeft();
		Triangle::Label trr = tr->getTriangleRight();
		Triangle::Label tlc = tl->getTriangleCenter();
		Triangle::Label trc = tr->getTriangleCenter();

		Vertex::Label vr = tr->getVertexRight();
		Vertex::Label vlu = tl->getVertexLeft();
		Vertex::Label vru = tl->getVertexRight();

		trianglesAll.remove(tl);
		trianglesAll.remove(tr);
		Triangle::destroy(tl);
		Triangle::destroy(tr);

		Triangle::Label tln = Triangle::create();
		Triangle::Label trn = Triangle::create();
		trianglesAll.add(tln);
		trianglesAll.add(trn);

		tln->setVertices(v, vr, vlu);
		trn->setVertices(vlu, vru, vr);

		tln->setTriangleLeft(tll);
		tln->setTriangleRight(trn);
		trn->setTriangleRight(trr);
		tln->setTriangleCenter(trc);
		trn->setTriangleCenter(tlc);

		updateVertexCoord(v, -1, 0);
		updateVertexCoord(vr, 1, 0);
		updateVertexCoord(vlu, 0, 1);
		updateVertexCoord(vru, 0, -1);
	}
}

void Universe::updateVertexCoord(Vertex::Label v, int up, int down) {
	if(v->nUp + v->nDown == 4)  // Since up != 0 or down != 0, no need to check if contains
		verticesFour.remove(v);

	if ((v->nUp > 2) && (v->nUp + up == 2))
		verticesPlus.remove(v);

	if ((up > 0) && (v->nUp == 2))  // If nUp == 2, then up >= 0
		verticesPlus.add(v);

	v->changeCoord(up, down);

	if (v->nUp + v->nDown == 4)  // Since up != 0 or down !=, no need to check if does not contain
		verticesFour.add(v);
		
}


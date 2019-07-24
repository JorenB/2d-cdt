// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "universe.hpp"

//Universe::Universe(int nSlices, int seed) : nSlices(nSlices)/*, trianglesAll(rng), verticesFour(rng), verticesPlus(rng)*/, rng(seed) {} 
int Universe::nSlices = 0;
std::vector<int> Universe::sliceSizes;

void Universe::create(int nSlices_, int seed_) {
	nSlices = nSlices_;
	initialize();
}

void Universe::initialize() {
	int w = 10;  // width of the initial strip. Can be adjusted for thermalization purposes - unclear what the 'optimal' value is.
	int t = nSlices;

	std::vector<Vertex*> initialVertices(w*t);

	for (int i = 0; i < w*t; i++) {
		Vertex& v = Vertex::at(Vertex::create());
		v.time = i / w;
		v.setCoord(3, 3);
		initialVertices[i] = &v;
		//verticesPlus.add(v);
	}

	for (int i = 0; i < t; i++) {
		sliceSizes.push_back(w);
	}

	// create triangles
	std::vector<Triangle*> initialTriangles(2*w*t);
	for(int i = 0; i < t; i++) {
		for(int j = 0; j < w; j++) {
			Triangle& tl = Triangle::at(Triangle::create());
			tl.setVertices(
					*initialVertices[i*w+j],
					*initialVertices[i*w+(j+1)%w],
					*initialVertices[((i+1)%t)*w+j]);
			initialTriangles[2*(i*w+j)] = &tl;

			Triangle& tr = Triangle::at(Triangle::create());
			tr.setVertices(
					*initialVertices[((i+1)%t)*w+j],
					*initialVertices[((i+1)%t)*w+(j+1)%w],
					*initialVertices[i*w+(j+1)%w]);
			initialTriangles[2*(i*w+j)+1] = &tr;
			//trianglesAll.add(tl);
			//trianglesAll.add(tr);
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
					*initialTriangles[(row + column -2*w+1+2*t*w)%(2*t*w)]);

			initialTriangles[row + column +1]->setTriangles(
					*initialTriangles[row + column],
					*initialTriangles[row + (column+2)%(2*w)],
					*initialTriangles[(row + column + 2*w)%(2*t*w)]);
		}
	}
}


/*void Universe::insertVertex(Triangle& t) {
	Triangle& tc = t.getTriangleCenter();

	Vertex& vr = t.getVertexRight();

	int time = t.time;

	Vertex& v = vertices.create();
	v.time = time;
	v.setCoord(2, 2);
	verticesFour.add(v);
	sliceSizes[time] += 1;

	t.setVertexRight(v);
	tc.setVertexRight(v);

	Triangle& t1 = triangles.create();  // right neighbor of t
	Triangle& t2 = triangles.create();  // right neighbor of tc
	trianglesAll.add(t1);
	trianglesAll.add(t2);

	t1.setVertices(v, vr, t.getVertexCenter());
	t2.setVertices(v, vr, tc.getVertexCenter());

	t1.setTriangles(t, t.getTriangleRight(), t2);
	t2.setTriangles(tc, tc.getTriangleRight(), t1);

	if (t.isUpwards()) {
		updateVertexCoord(t.getVertexCenter(), 0, 1);
		updateVertexCoord(tc.getVertexCenter(), 1, 0);
	} else if (t.isDownwards()) {
		updateVertexCoord(t.getVertexCenter(), 1, 0);
		updateVertexCoord(tc.getVertexCenter(), 0, 1);
	}
}

void Universe::removeVertex(Vertex& v) {
	Triangle& tl = v.getTriangleLeft();
	Triangle& tr = v.getTriangleRight();
	Triangle& tlc = tl.getTriangleCenter();
	Triangle& trc = tr.getTriangleCenter();

	Triangle& trn = tr.getTriangleRight();
	Triangle& trcn = trc.getTriangleRight();

	tl.setTriangleRight(trn);
	tlc.setTriangleRight(trcn);

	tl.setVertexRight(tr.getVertexRight());
	tlc.setVertexRight(tr.getVertexRight());

	tr.getVertexRight().setTriangleLeft(tl);

	sliceSizes[v.time] -= 1;

	trianglesAll.remove(tr);
	trianglesAll.remove(trc);
	triangles.destroy(tr);
	triangles.destroy(trc);

	Vertex& vu = tl.getVertexCenter();
	Vertex& vd = tlc.getVertexCenter();

	updateVertexCoord(vu, 0, -1);
	updateVertexCoord(vd, -1, 0);

	verticesFour.remove(v);
	vertices.destroy(v);
}

void Universe::flipLink(Vertex& v, flipSide side) {
	std::bernoulli_distribution d(0.5);

	if (side == LEFT) {
		Triangle& tl = v.getTriangleLeft();
		Triangle& tr = tl.getTriangleRight();
		Triangle& tll = tl.getTriangleLeft();
		Triangle& trr = tr.getTriangleRight();
		Triangle& tlc = tl.getTriangleCenter();
		Triangle& trc = tr.getTriangleCenter();

		Vertex& vl = tl.getVertexLeft();
		Vertex& vlu = tr.getVertexLeft();
		Vertex& vru = tr.getVertexRight();

		trianglesAll.remove(tl);
		trianglesAll.remove(tr);
		triangles.destroy(tl);
		triangles.destroy(tr);

		Triangle& tln = triangles.create();
		Triangle& trn = triangles.create();
		trianglesAll.add(tln);
		trianglesAll.add(trn);

		tln.setVertices(vlu, vru, vl);
		trn.setVertices(vl, v, vru);

		tln.setTriangleLeft(tll);
		tln.setTriangleRight(trn);
		trn.setTriangleRight(trr);
		tln.setTriangleCenter(trc);
		trn.setTriangleCenter(tlc);

		updateVertexCoord(v, -1, 0);
		updateVertexCoord(vl, 1, 0);
		updateVertexCoord(vlu, 0, -1);
		updateVertexCoord(vru, 0, 1);
	} else if (side == RIGHT) {
		Triangle& tr = v.getTriangleRight();
		Triangle& tl = tr.getTriangleLeft();
		Triangle& tll = tl.getTriangleLeft();
		Triangle& trr = tr.getTriangleRight();
		Triangle& tlc = tl.getTriangleCenter();
		Triangle& trc = tr.getTriangleCenter();

		Vertex& vr = tr.getVertexRight();
		Vertex& vlu = tl.getVertexLeft();
		Vertex& vru = tl.getVertexRight();

		trianglesAll.remove(tl);
		trianglesAll.remove(tr);
		triangles.destroy(tl);
		triangles.destroy(tr);

		Triangle& tln = triangles.create();
		Triangle& trn = triangles.create();
		trianglesAll.add(tln);
		trianglesAll.add(trn);

		tln.setVertices(v, vr, vlu);
		trn.setVertices(vlu, vru, vr);

		tln.setTriangleLeft(tll);
		tln.setTriangleRight(trn);
		trn.setTriangleRight(trr);
		tln.setTriangleCenter(trc);
		trn.setTriangleCenter(tlc);

		updateVertexCoord(v, -1, 0);
		updateVertexCoord(vr, 1, 0);
		updateVertexCoord(vlu, 0, 1);
		updateVertexCoord(vru, 0, -1);
	}
}

void Universe::updateVertexCoord(Vertex &v, int up, int down) {
	if(v.nUp + v.nDown == 4)  // Since up != 0 or down != 0, no need to check if contains
		verticesFour.remove(v);

	if ((v.nUp > 2) && (v.nUp + up == 2))
		verticesPlus.remove(v);

	if ((up > 0) && (v.nUp == 2))  // If nUp == 2, then up >= 0
		verticesPlus.add(v);

	v.changeCoord(up, down);

	if (v.nUp + v.nDown == 4)  // Since up != 0 or down !=, no need to check if does not contain
		verticesFour.add(v);
}
*/

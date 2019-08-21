// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "simulation.hpp"
#include "observables/volume.hpp"

std::default_random_engine Simulation::rng(0);  // TODO: seed properly
int Simulation::sweepSize = 0;
double Simulation::lambda = 0;
double Simulation::gsq = 0;
std::vector<Observable*> Simulation::observables;

void Simulation::start(int sweeps, int sweepSize_, double lambda_) {
	sweepSize = sweepSize_;
	lambda = lambda_;
	gsq = exp(-2.0*lambda);


	for (unsigned int i = 0; i < sweeps; i++) {
		sweep();
		printf("sweep %d\n", i);
		prepare();
		for (auto o : observables) {
			o->measure();
		}
	}

}

void Simulation::sweep() {
	std::uniform_int_distribution<> uniform_int(0, 3);

	int move;
	for (unsigned int i = 0; i < sweepSize; i++) {
		move = uniform_int(rng);
		switch (move) {
			case 0:
				moveAdd();
				break;
			case 1:
				moveDelete();
				break;
			case 2:
			case 3:
				moveFlip();
				break;
		}

	}
}

bool Simulation::moveAdd() {
	double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	double ar = n2 / (2.0*(n0_four + 1.0)) * gsq;

	Triangle::Label t = Universe::trianglesAll.pick();

	if (ar < 1.0) { 
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Universe::insertVertex(t);
	return true;
}

bool Simulation::moveDelete() {
	if (Universe::verticesFour.size() == 0) return false;

	double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	double ar = n0_four*2.0/(gsq * (n2-2.0));

	Vertex::Label v = Universe::verticesFour.pick();
	if (Universe::sliceSizes[v->time] < 4) return false;  // reject moves that shrink slices below size 3

	if (ar < 1.0) {
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}	

	Universe::removeVertex(v);

	return true;
}

bool Simulation::moveFlip() {
	if (Universe::verticesPlus.size() == 0) return false;

	Vertex::Label v = Universe::verticesPlus.pick();
	std::bernoulli_distribution bernoulli(0.5);
	Universe::flipSide side;
	int wa = Universe::verticesPlus.size();
	int wb = wa;
	if (bernoulli(rng)) {
		side = Universe::flipSide::LEFT;
		wb = v->getTriangleLeft()->getVertexLeft()->nUp == 2 ? wb + 1 : wb;
	}
	else {
		side = Universe::flipSide::RIGHT;
		wb = v->getTriangleRight()->getVertexRight()->nUp == 2 ? wb + 1 : wb;
	}

	if (v->nUp == 3)
		wb -= 1;

	double ar = 1.0*wa/wb;

	if (ar < 1.0) { 
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Universe::flipLink(v, side);

	assert(wb == Universe::verticesPlus.size());

	return true;
}

void Simulation::prepare() {
	Universe::updateVertexData();
	Universe::updateTriangleData();

	/*for (auto t : Universe::triangles) {
		printf("t: %d\n", (int) t);
		for (auto tn : Universe::triangleNeighbors[t]) {
			printf("\ttn: %d\n", (int) tn);
		}
	}
	printf("tsize: %d\n", (int) Universe::triangles.size());
	for (auto v : Universe::vertices) {
		printf("v: %d\n", (int) v);
		for (auto vn : Universe::vertexNeighbors[v]) {
			printf("\tvn: %d\n", (int) vn);
		}
	}
	printf("vsize: %d\n", (int) Universe::vertices.size());*/
}

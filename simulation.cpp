// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "simulation.hpp"
#include "observables/volume.hpp"

std::default_random_engine Simulation::rng(0);  // TODO: seed properly
int Simulation::targetVolume = 0;
int Simulation::sweepSize = 0;
double Simulation::lambda = 0;
double Simulation::gsq = 0;
bool Simulation::measuring = false;
std::vector<Observable*> Simulation::observables;

void Simulation::start(int sweeps, int sweepSize_, double lambda_, int targetVolume_) {
	sweepSize = sweepSize_;
	lambda = lambda_;
	gsq = exp(-2.0*lambda);
	targetVolume = targetVolume_;

	gsq = 0.25;

	for (auto o : observables) {
		o->clear();
	}

	prepare();
	printf("thermalizing");
	fflush(stdout);
	for (int i = 0; i < 50; i++) {
		sweep();
		printf(".");
		fflush(stdout);
	}
	printf("\n");

	measuring = true;

	for (int i = 0; i < sweeps; i++) {
		sweep();
		printf("sweep %d\n", i);
		printf("vol: %d\n", Triangle::size());
	}

}

void Simulation::sweep() {
	std::uniform_int_distribution<> uniform_int(0, 3);
	bool measured = false;

	int move;
	for (int i = 0; i < sweepSize; i++) {
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
		if (measuring && !measured) {
			if (targetVolume > 0 && Triangle::size() != targetVolume) continue;
			prepare();
			for (auto o : observables) {
				o->measure();
			}
			measured = true;
		}
	}
}

bool Simulation::moveAdd() {
	double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	double ar = n2 / (2.0*(n0_four + 1.0)) * gsq;
	if (targetVolume > 0) ar *= exp(-epsilon*(1-(targetVolume-n2)));

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
	if (targetVolume > 0) ar *= exp(epsilon * (1-(targetVolume-n2)));

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
		printf("t: %d, %c, t=%d\n", (int) t, t->isUpwards() ? 'u' : 'd', t->time);
		for (auto tn : Universe::triangleNeighbors[t]) {
			printf("\ttn: %d\n", (int) tn);
		}
	}
	printf("tsize: %d\n", (int) Universe::triangles.size());
	for (auto v : Universe::vertices) {
		printf("v: %d, tl: %d, tr: %d, nUp: %d, nDown: %d\n", (int) v, (int) v->getTriangleLeft(), (int) v->getTriangleRight(), v->nUp, v->nDown);
		for (auto vn : Universe::vertexNeighbors[v]) {
			printf("\tvn: %d\n", (int) vn);
		}
	}
	printf("vsize: %d\n", (int) Universe::vertices.size());*/
}

// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "simulation.hpp"

std::default_random_engine Simulation::rng(0);  // TODO: seed properly
int Simulation::sweepSize = 0;
double Simulation::lambda = 0;
double Simulation::gsq = 0;

void Simulation::start(int sweeps, int sweepSize_, double lambda_) {
	sweepSize = sweepSize_;
	lambda = lambda_;
	gsq = exp(-2.0*lambda);

	for (unsigned int i = 0; i < sweeps; i++) {
		sweep();
		printf("sweep %d\n", i);
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
	if (bernoulli(rng))
		side = Universe::flipSide::LEFT;
	else
		side = Universe::flipSide::RIGHT;

	Universe::flipLink(v, side);

	return true;
}
/*void Simulation::start(int totalSweeps) {
	std::uniform_int_distribution<> uniform_int(0, 3);
	for (auto o : observables) {
		o->clear();
	}

	int move;
	int sweeps = 0;
	double bias = 0;
	long unsigned int accepted = 0;
	for (long unsigned int i = 0; true; i++) {
		move = uniform_int(rng);
		switch (move) {
			case 0:
				if (moveAdd()) accepted++;
				break;
			case 1:
				if (moveDelete()) accepted++;
				break;
			case 2:
			case 3:
				if (moveFlip()) accepted++;
				break;
		}
		if (i%sweepSize == 0) {
			printf("sweep %d\n", sweeps);
			printf("torus %d\n", u->nSlices);
			printf("ar: %f, a: %lu, i: %lu\n", (double) accepted/ (double) i, accepted, i);
			for (auto v : u->vertices.elements) {
				v.neighborsFresh = false;
			}
			if (startMeasurements) {
				for (auto o : observables) {
					o->measure();
				}
			}
			sweeps++;
			if (sweeps >= totalSweeps) break;
			if (sweeps >= 150 && lambda > 0.69) startMeasurements = true; // TODO: more intelligent solution
		}

		bias += (double) u->vertices.size() - (double) targetSize;

		if (i % tuneLambdaSteps == 0) {
			bias *= 1.0*1.0/(double) tuneLambdaSteps/(double) targetSize;


			printf("n0: %d\n", u->vertices.size());
			printf("n0_four: %d\n", u->verticesFour.size());
			printf("bias: %f\n", bias);
			printf("lambda: %f\n", lambda);

			bias = 0;
		}
	}
}*/

/**/

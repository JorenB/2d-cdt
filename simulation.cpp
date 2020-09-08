// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "simulation.hpp"
#include "observables/volume.hpp"

std::default_random_engine Simulation::rng(0);  // TODO: seed properly
int Simulation::targetVolume = 0;
double Simulation::gsq = 0;
std::vector<Observable*> Simulation::observables;

std::array<int, 2> Simulation::moveFreqs = {1, 1};

void Simulation::start(int measurements, int targetVolume_) {
	targetVolume = targetVolume_;

	gsq = 0.25;

	for (auto o : observables) {
		o->clear();
	}
	
	grow();

	thermalize();

	for (int i = 0; i < measurements; i++) {
		sweep();
		printf("sweep %d\n", i);
		printf("vol: %d\n", Triangle::size());
	}

}

int Simulation::attemptMove() {
	std::array<int, 2> cumFreqs = {0, 0};
	int freqTotal = 0;
	int prevCumFreq = 0;
	for (int i = 0; i < moveFreqs.size(); i++) {
		freqTotal += moveFreqs[i];
		cumFreqs[i] = prevCumFreq + moveFreqs[i];
		prevCumFreq = cumFreqs[i];
	}

	std::uniform_int_distribution<> moveGen(0, freqTotal-1);
	std::uniform_int_distribution<> binGen(0, 1);

	int move = moveGen(rng);

	if (move < cumFreqs[0]) {
		if (binGen(rng) == 0) {	
			if (moveAdd()) return 1;
		} else {
			if (moveDelete()) return 2;
		}
	} else if (cumFreqs[0] <= move) {
		if (moveFlip()) return 3;
	} 

	return 0;
}

void Simulation::sweep() {
	std::uniform_int_distribution<> uniform_int(0, 3);
	bool measured = false;

	std::array<int, 4> moves = {0, 0, 0, 0};
	for (int i = 0; i < 100*targetVolume; i++) {
		moves[attemptMove()]++;
	}
	
	do {
		attemptMove();
	} while (Triangle::size() != targetVolume);

	prepare();
	for (auto o : observables) {
		o->measure();
	}
}

bool Simulation::moveAdd() {
	double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	double ar = n2 / (2.0*(n0_four + 1.0)) * gsq;
	if (targetVolume > 0) ar *= exp(epsilon * (targetVolume-n2));

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
	if (targetVolume > 0) ar *= exp(-epsilon * (targetVolume-n2));

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
}

void Simulation::grow() {
	int growSteps = 0;
	printf("growing");
	do {
		for (int i = 0; i < targetVolume; i++) attemptMove();
		printf(".");
		fflush(stdout);
		growSteps++;
	} while (Triangle::size() < targetVolume);
	printf("\n");
	printf("grown in %d sweeps\n", growSteps);
}

void Simulation::thermalize() {
	int thermSteps = 0;
	printf("thermalizing");
	fflush(stdout);
	//for (int i = 0; i < 50; i++) {
	double coordBound = log(2*targetVolume)/(double)log(2);
	int maxUp, maxDown;
	do {
		for (int i = 0; i < 100*targetVolume; i++) attemptMove();
		printf(".");
		fflush(stdout);

		prepare();
		maxUp = 0;
		maxDown = 0;
		for (auto v : Universe::vertices) {
			if (v->nUp > maxUp) maxUp = v->nUp;
			if (v->nDown > maxDown) maxDown = v->nDown;
		}
		//printf("up: %d, down %d, bound: %f\n", maxUp, maxDown, coordBound);

		thermSteps++;
	} while (maxUp > coordBound || maxDown > coordBound);
	printf("\n");
	printf("thermalized in %d sweeps\n", thermSteps);
}

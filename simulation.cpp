// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include "simulation.hpp"
#include <vector>

std::default_random_engine Simulation::rng(0);  // TODO(JorenB): seed properly
int Simulation::targetVolume = 0;
double Simulation::lambda = 0;
double Simulation::epsilon = 0.02;
std::vector<Observable*> Simulation::observables;

std::array<int, 2> Simulation::moveFreqs = {1, 1};

void Simulation::start(int measurements, double lambda_, int targetVolume_, int seed) {
	targetVolume = targetVolume_;

	lambda = lambda_;

	for (auto o : observables) {
		o->clear();
	}

	rng.seed(seed);
	// tune();

	if (!Universe::imported) {
		grow();
		thermalize();
		std::string expFn = "geom/geometry-V"+std::to_string(targetVolume)+"-sl"+std::to_string(Universe::nSlices)+"-s"+std::to_string(seed);
		if (Universe::sphere) expFn += "-sphere";
		expFn += ".txt";
		Universe::exportGeometry(expFn);
	}

	for (int i = 0; i < measurements; i++) {
		sweep();
		printf("m %d\n", i);
		fflush(stdout);
	}
}

int Simulation::attemptMove() {
	std::array<int, 2> cumFreqs = {0, 0};
	int freqTotal = 0;
	int prevCumFreq = 0;
	for (auto i = 0u; i < moveFreqs.size(); i++) {
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

	std::array<int, 4> moves = {0, 0, 0, 0};
	for (int i = 0; i < 500 * targetVolume; i++) {
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
	double n0 = Vertex::size();
	// double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	// double edS = exp(- 2 * lambda);
	// double rg = n2 / (n2 + 2.0);
	// double ar = edS * rg;

	double ar = n0 / (n0_four + 1.0) * exp(- 2 * lambda);
	if (targetVolume > 0) {
		double expesp = exp(2 * epsilon);
		ar *= Triangle::size() < targetVolume ? expesp : 1 / expesp;
	}

	Triangle::Label t = Universe::trianglesAll.pick();

	if (Universe::sphere) {
		if (t->time == 0) return false;
	}

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

	double n0 = Vertex::size();
	// double n2 = Triangle::size();
	double n0_four = Universe::verticesFour.size();

	// double edS = exp(2 * lambda);
	// double rg = n2 / (n2 - 2.0);
	// double ar = edS * rg;

	double ar = n0_four / (n0 - 1.0) * exp(2 * lambda);
	if (targetVolume > 0) {
		double expesp = exp(2 * epsilon);
		ar *= Triangle::size() < targetVolume ? 1 / expesp : expesp;
	}

	if (ar < 1.0) {
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Vertex::Label v = Universe::verticesFour.pick();
	// auto t = Universe::trianglesAll.pick();
	// auto v = t->getVertexLeft();
	// if (v->nUp + v->nDown != 4) return false;

	if (Universe::sliceSizes[v->time] < 4) return false;  // reject moves that shrink slices below size 3

	Universe::removeVertex(v);

	return true;
}

bool Simulation::moveFlip() {
	if (Universe::trianglesFlip.size() == 0) return false;

	auto t = Universe::trianglesFlip.pick();

	int wa = Universe::trianglesFlip.size();
	int wb = wa;
	if (t->type == t->getTriangleLeft()->type) {
		wb++;
	} else {
		wb--;
	}

	if (t->getTriangleRight()->type == t->getTriangleRight()->getTriangleRight()->type) {
	   	wb++;
	} else {
		wb--;
	}

	double ar = 1.0*wa/wb;

	if (ar < 1.0) {
		std::uniform_real_distribution<> uniform(0.0, 1.0);
		double r = uniform(rng);
		if (r > ar) return false;
	}

	Universe::flipLink(t);

	return true;
}

void Simulation::prepare() {
	Universe::updateVertexData();
	Universe::updateTriangleData();
	Universe::updateLinkData();
}

#if 0  // tuning isn't used in the current setup
void Simulation::tune() {
	printf("start tune..\n");
	fflush(stdout);
	std::vector<int> volumes;
	epsilon = 0.02;

	bool done = false;
	int tuneSteps = 50;
	for (int k = 0; k < tuneSteps && !done; k++) {
		for (int i = 0; i < targetVolume; i++) {
			for (int j = 0; j < 100; j++) attemptMove();

			volumes.push_back(Triangle::size());
		}

		double avg = 0.0;
		for (auto v : volumes) avg += static_cast<double>(v);
		avg /= volumes.size();

		double sd = 0.0;
		for (auto v : volumes) sd += (static_cast<double>(v) - avg)*(static_cast<double>(v) - avg);
		sd /= volumes.size();

		if ((targetVolume - avg)*(targetVolume - avg) < 2*sd) {
			epsilon *= 0.7;
			if (epsilon < 0.02) {
				epsilon = 0.02;
				lambda -= 0.003 * (targetVolume - avg) / sqrt(sd);
			}
		} else if ((targetVolume - avg)*(targetVolume - avg) > 8*sd) {
			epsilon *= 1.2;
			if (epsilon > 5.0) epsilon = 5.0;
		} else if ((targetVolume - avg)*(targetVolume - avg) < 0.04*targetVolume*targetVolume) {
			lambda += 0.6*(avg - targetVolume)/abs((avg-targetVolume)) * epsilon;
		}
		volumes.clear();
		if (k >= tuneSteps && abs(avg-targetVolume) < 0.1*targetVolume && epsilon < 0.021) done = true;

		printf("step %d - epsilon: %f, lambda: %f, avg: %f, sd: %f\n", k, epsilon, lambda, avg, sd);
	}
}
#endif

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
	double coordBound = log(2 * targetVolume) / static_cast<double>(log(2));
	int maxUp, maxDown;
	do {
		for (int i = 0; i < 100 * targetVolume; i++) attemptMove();
		printf(".");
		fflush(stdout);

		prepare();
		maxUp = 0;
		maxDown = 0;
		for (auto v : Universe::vertices) {
			int nup = 0, ndown = 0;
			for (auto vn : Universe::vertexNeighbors.at(v)) {
				if (vn->time > v->time || (v->time == Universe::nSlices-1 && vn->time == 0)) nup++;
				if (vn->time < v->time || (v->time == 0 && vn->time == Universe::nSlices-1)) ndown++;
			}
			if (nup > maxUp) maxUp = nup;
			if (ndown > maxDown) maxDown = ndown;
		}

		thermSteps++;
	} while (maxUp > coordBound || maxDown > coordBound);
	printf("\n");
	printf("thermalized in %d sweeps\n", thermSteps);
}

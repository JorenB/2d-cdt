// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <random>
#include "universe.hpp"
#include "observable.hpp"

class Simulation {
public:
	static double lambda;

	static void start(int sweeps,int targetVolume_, int seed = 0);

	static void addObservable(Observable& o) {
		observables.push_back(&o);
	}

	static bool pinch;

	static std::array<int, 2> moveFreqs;
	static int attemptMove();
private:
	static std::default_random_engine rng;

	static int targetVolume;
	static double epsilon;
	static bool measuring;

	static std::vector<Observable*> observables;

	static void sweep();

	static bool moveAdd();
	static bool moveDelete();
	static bool moveFlip();

	static void prepare();

	static void tune();
	static void grow();
	static void thermalize();
};
#endif  // SIMULATION_HPP_

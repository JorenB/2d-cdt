// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <random>
#include "universe.hpp"

class Simulation {
public:
	static int sweepSize;
	static double lambda;
	static double gsq;

	static void start(int sweeps, int sweepSize_, double lambda_);

	static void sweep();

	static bool moveAdd();
	static bool moveDelete();
	static bool moveFlip();

private:
	static std::default_random_engine rng;
};
#endif  // SIMULATION_HPP_

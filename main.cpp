// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"
#include "simulation.hpp"

int main(int argc, const char * argv[]) {
	Universe::create(3);

	Simulation::start(50000, 10000, 0.699);

	return 0;
}

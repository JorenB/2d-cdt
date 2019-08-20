// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"
#include "simulation.hpp"
#include "observable.hpp"
#include "observables/volume.hpp"

int main(int argc, const char * argv[]) {
	Universe::create(3);

	Volume o1;
	Simulation::addObservable(o1);

	Simulation::start(100, 100000, 0.699);


	return 0;
}

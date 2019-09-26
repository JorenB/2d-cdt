// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"
#include "simulation.hpp"
#include "observable.hpp"
#include "observables/volume_profile.hpp"
#include "observables/coord.hpp"

int main(int argc, const char * argv[]) {
	Universe::create(200);

	VolumeProfile vp;
	Simulation::addObservable(vp);
	Coord coord;
	Simulation::addObservable(coord);

	Simulation::start(100, 1000000, 0.699, 250*250);

	return 0;
}

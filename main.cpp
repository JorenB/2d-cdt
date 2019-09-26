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

int main(int argc, const char * argv[]) {
	Universe::create(120);

	VolumeProfile vp;
	Simulation::addObservable(vp);

	Simulation::start(10, 100000, 0.699, 300*120);

	return 0;
}

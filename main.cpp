// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "include/INIReader.h"

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
	INIReader ir("in/conf.ini");

	if (ir.ParseError() != 0) return 1;

	int targetVolume = ir.GetInteger("geometry", "targetVolume", 0);

	std::string fID = ir.Get("simulation", "fileID", "geen");
	int measurements = ir.GetInteger("simulation", "measurements", 0);

	Universe::create(10);

	VolumeProfile vp;
	Simulation::addObservable(vp);

	
	Simulation::start(100, targetVolume);

	return 0;
}

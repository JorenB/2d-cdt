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
#include "observables/volume_profile.hpp"
#include "observables/hausdorff.hpp"
#include "observables/hausdorff_dual.hpp"

int main(int argc, const char * argv[]) {
	Universe::create(100);

	Hausdorff o1;
	Simulation::addObservable(o1);
	VolumeProfile o2;
	Simulation::addObservable(o2);
	HausdorffDual o3;
	Simulation::addObservable(o3);

	Simulation::start(10, 1000000, 0.699);

	
	return 0;
}

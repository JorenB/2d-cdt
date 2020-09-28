// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>
#include <chrono>

#include "include/INIReader.h"

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"
#include "simulation.hpp"
#include "observable.hpp"
#include "observables/volume_profile.hpp"
#include "observables/hausdorff.hpp"
#include "observables/hausdorff_dual.hpp"

int main(int argc, const char * argv[]) {
	std::string fname;
	if (argc > 1) {
		fname = std::string(argv[1]);
		printf("%s\n", fname.c_str());
	}
	INIReader ir(fname);

	if (ir.ParseError() != 0) return 1;

	int targetVolume = ir.GetInteger("geometry", "targetVolume", 0);
	int slices = ir.GetInteger("geometry", "slices", 0);
	std::string sphereString = ir.Get("geometry", "sphere", "false");
	bool sphere = false;
	if (sphereString == "true") sphere = true;


	int seed = ir.GetInteger("simulation", "seed", 0);
	std::string fID = ir.Get("simulation", "fileID", "geen");
	int measurements = ir.GetInteger("simulation", "measurements", 0);

	Universe::create(slices);
	if (sphere) {
		Universe::sphere = true;
		printf("sphere\n");
	}

	//VolumeProfile vp(fID);
	//Simulation::addObservable(vp);

	//Hausdorff haus(fID);
	//Simulation::addObservable(haus);

	//HausdorffDual hausd(fID);
	//Simulation::addObservable(hausd);






	//int seed = std::chrono::system_clock::now().time_since_epoch().count();
	printf("seed: %d\n", seed);

	Simulation::start(measurements, targetVolume, seed);

	printf("end\n");
	return 0;
}

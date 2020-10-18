// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "config.hpp"
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
	ConfigReader cfr;
	cfr.read(fname);

	double lambda = cfr.getDouble("lambda");
	int targetVolume = cfr.getInt("targetVolume");
	int slices = cfr.getInt("slices");
	std::string sphereString = cfr.getString("sphere");
	bool sphere = false;
	if (sphereString == "true") sphere = true;

	int seed = cfr.getInt("seed");
	std::string fID = cfr.getString("fileID");
	int measurements = cfr.getInt("measurements");

	Universe::create(slices);
	if (sphere) {
		Universe::sphere = true;
		printf("sphere\n");
	}

	VolumeProfile vp(fID);
	Simulation::addObservable(vp);

	Hausdorff haus(fID);
	Simulation::addObservable(haus);


	printf("seed: %d\n", seed);

	Simulation::start(measurements, lambda, targetVolume, seed);
	printf("end\n");
	return 0;
}

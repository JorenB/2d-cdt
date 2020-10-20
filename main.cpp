// Copyright 2020 Joren Brunekreef and Andrzej Görlich
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
#include "observables/ricci.hpp"
#include "observables/ricci_dual.hpp"
#include "observables/riccih.hpp"
#include "observables/ricciv.hpp"

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
	if (sphereString == "true") {
		Universe::sphere = true;
		printf("sphere\n");
	}

	int seed = cfr.getInt("seed");
	std::string fID = cfr.getString("fileID");
	int measurements = cfr.getInt("measurements");
	std::string impGeomString = cfr.getString("importGeom");
	bool impGeom = false;
	if (impGeomString == "true") impGeom = true;

	if (impGeom) {
		std::string geomFn = Universe::getGeometryFilename(targetVolume, slices, seed);
		if (geomFn != "") {
			Universe::importGeometry(geomFn);
		} else {
			printf("No suitable geometry file found. Creating new Universe...\n");
		}
	}

	if (Universe::imported == false) {
		Universe::create(slices);
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

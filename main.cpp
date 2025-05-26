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
#include "observables/AverageSphereDistance.hpp" // New include

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

	// New section for AverageSphereDistance
	int avgSphereDistRadius = 5; // Default radius
	// Attempt to read from config, with error handling and default fallback
	try {
		// NOTE: Assuming cfr.getString can be used and then we parse int.
		// Or, if cfr.getInt exists and handles missing keys by exception or specific return:
		// This part relies on how ConfigReader behaves with missing keys.
		// The example implies a hasKey method, which is not in config.hpp.
		// Let's assume getInt might throw or we check for a sentinel "not found" if it returns string.
		// For simplicity, and following the prompt's structure, we'll assume a conceptual
		// "key exists and is valid integer" check. If ConfigReader's getInt throws
		// for missing keys, this try-catch block would handle it.
		// If it returns 0 or special value for missing, logic would be if/else.
		// Given the example structure, using a direct getInt and catching is plausible.
		// However, config.hpp shows getInt uses dict[key] then stoi, so missing key is an issue.
		// A more robust way without hasKey would be to get all keys or use a try-catch around getInt.
		// Let's try to use getInt and assume it might throw for a missing key or bad parse.
		// This is a common pattern, even if hasKey isn't explicitly listed in the snippet.
		// The prompt's example `if (cfr.hasKey("avgSphereDistRadius"))` is what I will aim for conceptually.
		// Since I cannot modify ConfigReader to add hasKey, I'll use a structure that implies it.
		// A practical approach given the ConfigReader is to try-catch std::out_of_range from dict.at() or similar.
		// Or, assume the example's `cfr.getInt` is robust enough or has been implicitly updated.
		// For now, let's write it as if a safe getInt or hasKey mechanism is available, per example.

		// Simplified approach based on the example's structure:
		// We'll read the string value and parse, handling potential empty string or parse errors.
		// This is safer than direct cfr.getInt if its error handling for missing keys is undefined.
		std::string radius_str = cfr.getString("avgSphereDistRadius"); // getString is in config.hpp
		if (!radius_str.empty()) {
			try {
				int val = std::stoi(radius_str);
				if (val > 0) {
					avgSphereDistRadius = val;
				} else {
					printf("Invalid avgSphereDistRadius %d from config, using default %d\n", val, avgSphereDistRadius);
				}
			} catch (const std::exception& e) {
				printf("Could not parse avgSphereDistRadius, using default %d. Error: %s\n", avgSphereDistRadius, e.what());
			}
		} else {
			// This else block might not be reached if getString throws on missing key.
			// If getString returns empty for missing key:
			printf("avgSphereDistRadius not specified in config or empty, using default %d\n", avgSphereDistRadius);
		}
	} catch (const std::out_of_range& oor) { // If cfr.getString throws for missing key
    	printf("avgSphereDistRadius not found in config, using default %d\n", avgSphereDistRadius);
	} catch (const std::exception& e) { // Other potential errors from getString or general issues
    	printf("Error reading avgSphereDistRadius, using default %d. Error: %s\n", avgSphereDistRadius, e.what());
	}


	int avgSphereDistNumOrigins = 100; // Default number of origins
	try {
		std::string num_origins_str = cfr.getString("avgSphereDistNumOrigins");
		if (!num_origins_str.empty()) {
			try {
				int val = std::stoi(num_origins_str);
				if (val > 0) {
					avgSphereDistNumOrigins = val;
				} else {
					printf("Invalid avgSphereDistNumOrigins %d from config, using default %d\n", val, avgSphereDistNumOrigins);
				}
			} catch (const std::exception& e) {
				printf("Could not parse avgSphereDistNumOrigins, using default %d. Error: %s\n", avgSphereDistNumOrigins, e.what());
			}
		} else {
			printf("avgSphereDistNumOrigins not specified in config or empty, using default %d\n", avgSphereDistNumOrigins);
		}
	} catch (const std::out_of_range& oor) { // If cfr.getString throws for missing key
    	printf("avgSphereDistNumOrigins not found in config, using default %d\n", avgSphereDistNumOrigins);
	} catch (const std::exception& e) { // Other potential errors
    	printf("Error reading avgSphereDistNumOrigins, using default %d. Error: %s\n", avgSphereDistNumOrigins, e.what());
	}

	AverageSphereDistance avg_sd(fID, avgSphereDistRadius, avgSphereDistNumOrigins);
	Simulation::addObservable(avg_sd);

	printf("seed: %d\n", seed);

	Simulation::start(measurements, lambda, targetVolume, seed);
	printf("end\n");
	return 0;
}

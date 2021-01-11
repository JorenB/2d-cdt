# Monte Carlo Simulation of Two-Dimensional Causal Dynamical Triangulations

This codebase provides a simple framework for sampling the partition sum of two-dimensional Causal Dynamical Triangulations (CDT), primarily as a model of two-dimensional lattice quantum gravity. It is straightforward to implement custom observables for which one can subsequently compute ensemble averages, corresponding to its quantum expectation value. Simulation parameters can be set using a config file. 

## Usage
The code can be built using GNU Make:
```bash
make
```
The directory `example` shows how to start a simple simulation of a small CDT system. Issuing the command (one may have to fix execute permissions)
```bash
./run.sh
```
will build the project and run the simulation with a standard config file. The template for such a config file is as follows:
```
lambda          0.693147
targetVolume    16000
slices          100
seed            1
fileID          collab-16000-1
measurements    100
sphere          false
```
All parameters are required. 

- `lambda` is the cosmological constant and should generally be set to the critical value of `ln(2) = 0.693147...`.
- `targetVolume` is the target number of triangles of the system. Measurements are only performed at exactly this volume. Only even-numbered volumes are possible in the current setup, since triangles are always created in pairs.
- `slices` is the number of time slices in the geometry.
- `seed` is the seed for the random number generator. There is no further seeding using e.g. timestamps, so using the same seed in different simulations (while keeping all other parameters constant) ensures identical output.
- `fileID` is the identifier used to mark output files of observables.
- `measurements` is the number of measurements that should be performed for each observable before execution terminates.
- `sphere` instructs the simulation whether to force the configuration into an effective spherical topology or not. More comments on the spherical topology follow below.

## Observables
Several standard observables are supplied in the directory `observables`. These can be added to the simulation in `main.cpp`. In the standard `main.cpp`, it is shown how to do this for the volume profile and Hausdorff dimension observables.

Custom observables can be created based on the standard observables supplied here. The class `Universe` offers access to all `Vertex`, `Link`, and `Triangle` objects, and lists containing the neighbors of `Vertex` and `Triangle` objects. Furthermore, the class `Observable` provides methods for constructing metric spheres (circles) on both the direct and dual lattices, and methods for computing the (dual) distance between arbitrary `Vertex` and `Triangle` objects.

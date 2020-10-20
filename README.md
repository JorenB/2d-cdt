# Monte Carlo Simulation of Two-Dimensional Causal Dynamical Triangulations

This codebase provides a simple framework for sampling the partition sum of two-dimensional Causal Dynamical Triangulations (CDT), primarily as a model of two-dimensional lattice quantum gravity. It is straightforward to implement custom observables for which one can subsequently compute ensemble averages, corresponding to its quantum expectation value. Simulation parameters can be set using a config file. 

## Usage
The code can be built using GNU Make:
```
make all
```
The simplest method for running the executable is to pass a config file as an argument (one may have to create the directory `out` first, if it does not exist yet):
```
./main.x conf.dat
```
The template for a config file is as follows:
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

Another method of running the simulation is to specify several parameters in the file `params.sh`, and subsequently use
```
./run.sh <seed>
```
If this method is used to run the simulation directly (instead of through a slurm job), make sure to comment the `srun -u main.x $FILE` line, and uncomment `./main.x $FILE`. The bash script `run.sh` reads the parameters and generates a config file in the directory `in/` using `<seed>`, while also setting the `fileID` to `<dirname>-<targetVolume>-<seed>`. Subsequently, it runs the simulation. If the simulation is completed successfully, all output of the measured observables (possibly including output of simulations with a different seed run in the same directory, see below) will be copied to `$TARGET_DIR`.

### Batch execution
For large system sizes or time-consuming observables, one generally wants to run several simulations in parallel (with different seeds) on a computing cluster to collect data faster. This can be achieved using `batch.sh`. It submits a number of jobs to a `slurm` queue, where every job executes `run.sh` with a distinct seed. It should not be difficult to adapt `batch.sh` to interact with job schedulers other than `slurm`.

The basic usage is
```
./batch.sh <start> <end>
```
where `<start>` and `end` are integers. This will submit the jobs 
```
./run.sh <i>
```
to the job scheduler for all `<i>` in the range `<start>..<end>`. Every time a single job finishes execution, _all_ measurement output for the `$TARGET_VOLUME` set in `params.sh` is concatenated into a file (distinct files for distinct observables) in `$TARGET_DIR`. Therefore, the folder `out` in the executable directory may contain many measurement output files of the form `<observable>-<dirname>-<$TARGET_VOLUME>-<i>.dat`, but `$TARGET_DIR` will contain only a single file named `agg-<observable>-<dirname>-<$TARGET_VOLUME>.dat` for every `<observable>`. This helps maintain order in directories where the data will be analyzed.

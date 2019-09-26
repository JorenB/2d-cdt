CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++14 -O2 -Wall

.PHONY: all clean

vpath %.cpp observables
vpath %.hpp observables

all: main.x

main.x: main.o vertex.o triangle.o universe.o simulation.o observable.o volume.o hausdorff.o volume_profile.o hausdorff_dual.o 

main.o: main.cpp pool.hpp bag.hpp 

vertex.o: vertex.hpp pool.hpp triangle.hpp

triangle.o: triangle.hpp pool.hpp vertex.hpp

universe.o: universe.hpp pool.hpp bag.hpp vertex.hpp triangle.hpp

simulation.o: simulation.hpp universe.hpp pool.hpp bag.hpp vertex.hpp triangle.hpp

observable.o: observable.hpp simulation.hpp universe.hpp

volume.o: observables/volume.hpp observable.hpp

hausdorff.o: observables/hausdorff.hpp observable.hpp

hausdorff_dual.o: observables/hausdorff_dual.hpp observable.hpp

volume_profile.o: observables/volume_profile.hpp observable.hpp


%.x: %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

clean:
	$(RM) *.o *.x

CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++14 -O2 -Wall

.PHONY: all clean

all: main.x

main.x: main.o vertex.o triangle.o universe.o simulation.o observable.o observables/volume.o

#main.o: main.cpp pool.hpp bag.hpp vertex.hpp triangle.hpp universe.hpp simulation.hpp observable.hpp
main.o: main.cpp pool.hpp bag.hpp 

vertex.o: vertex.hpp pool.hpp triangle.hpp

triangle.o: triangle.hpp pool.hpp vertex.hpp

universe.o: universe.hpp pool.hpp bag.hpp vertex.hpp triangle.hpp

simulation.o: simulation.hpp universe.hpp pool.hpp bag.hpp vertex.hpp triangle.hpp

observable.o: observable.hpp simulation.hpp universe.hpp

observables/volume.o: observables/volume.hpp observable.hpp

%.x: %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

clean:
	$(RM) *.o *.x

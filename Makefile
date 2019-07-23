CXXFLAGS = -std=c++11 -O2 -Wall

.PHONY: all clean

all: main.x

main.x: main.o vertex.o triangle.o universe.o 

main.o: main.cpp pool.hpp bag.hpp vertex.hpp simplex.hpp triangle.hpp universe.hpp

vertex.o: vertex.hpp simplex.hpp

triangle.o: triangle.hpp simplex.hpp

universe.o: universe.hpp vertex.hpp triangle.hpp pool.hpp bag.hpp

%.x: %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

clean:
	$(RM) *.o *.x

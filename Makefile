CXX = /usr/local/opt/llvm/bin/clang++
CXXFLAGS = -std=c++11 -O2 -Wall

.PHONY: all clean

all: main.x

main.x: main.o vertex.o triangle.o universe.o

main.o: main.cpp pool.hpp vertex.hpp triangle.hpp universe.hpp

vertex.o: vertex.hpp pool.hpp triangle.hpp

triangle.o: triangle.hpp pool.hpp vertex.hpp

universe.o: universe.hpp vertex.hpp triangle.hpp

%.x: %.o
	$(CXX) $(LDFLAGS) $(LDLIBS) -o $@ $^

clean:
	$(RM) *.o *.x

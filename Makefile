#CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
LDFLAGS = 

main: main.o vertex.o triangle.o universe.o bag.hpp
	$(CXX) $(LDFLAGS) -o 2d-cdt main.o vertex.o triangle.o universe.o

main.o: main.cpp vertex.hpp triangle.hpp 

vertex.o: vertex.hpp simplex.hpp

triangle.o: triangle.hpp simplex.hpp

universe.o: universe.hpp vertex.hpp triangle.hpp pool.hpp bag.hpp

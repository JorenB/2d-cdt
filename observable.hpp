// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef OBSERVABLE_HPP_
#define OBSERVABLE_HPP_

#include <chrono>
#include <string>
#include <algorithm>
#include "universe.hpp"

class Observable {
public:
	std::string name;

	Observable(std::string identifier_) {
		identifier = identifier_;
		//identifier = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}

	void measure() {
		process();
		write();
	}

	void clear();

private:
	std::string identifier;

protected:
	static std::default_random_engine rng;
	virtual void process() = 0;

	void write();

	// toolbox
	static std::vector<Vertex::Label> sphere(Vertex::Label origin, int radius);
	static std::vector<Triangle::Label> sphereDual(Triangle::Label origin, int radius);

	static int distance(Vertex::Label v1, Vertex::Label v2);
	static int distanceDual(Triangle::Label t1, Triangle::Label t2);

	static Vertex::Label randomVertex() {
		std::uniform_int_distribution<> rv(0, Universe::vertices.size()-1);
		return Universe::vertices.at(rv(rng));
	}
	static Triangle::Label randomTriangle() {
		std::uniform_int_distribution<> rt(0, Universe::triangles.size()-1);
		return Universe::triangles.at(rt(rng));
	}

	std::string data_dir = "out/";
	std::string extension = ".dat";
	std::string output;
};
#endif  // OBSERVABLE_HPP_

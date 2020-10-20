// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class Ricci : public Observable {
public:
	Ricci(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "ricci"; }

	void process();

private:
	std::vector<int> epsilons;

	double averageSphereDistance(Vertex::Label p1, int epsilon);
};

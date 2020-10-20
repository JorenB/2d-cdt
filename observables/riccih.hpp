// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciH : public Observable {
    public:
        RicciH(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "riccih"; }

        void process();

	private:
		std::vector<int> epsilons;

        double averageSphereDistance(Vertex::Label p1, int epsilon);
};

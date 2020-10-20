// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciDual : public Observable {
    public:
        RicciDual(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "ricci_dual"; }

        void process();

	private:
		std::vector<int> epsilons;

        double averageSphereDistance(Triangle::Label t1, int epsilon);
};

// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include "../observable.hpp"
#include "../universe.hpp"

class HausdorffDual : public Observable {
    public:
        HausdorffDual(std::string id) : Observable(id) { name = "hausdorff_dual"; }

        void process();

	private:
		int max_epsilon;
};

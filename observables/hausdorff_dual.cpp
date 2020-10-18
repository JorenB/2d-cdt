// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <string>
#include <vector>
#include "hausdorff_dual.hpp"

void HausdorffDual::process() {
    std::string tmp = "";
	max_epsilon = Universe::nSlices;
    for (int i = 1; i < max_epsilon; i++) {
		auto t = randomTriangle();

        std::vector<Triangle::Label> s1 = sphereDual(t, i);
        tmp += std::to_string(s1.size());
        tmp += " ";
    }
	tmp.pop_back();

    output = tmp;
}

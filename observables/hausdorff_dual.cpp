#include "hausdorff_dual.hpp"

void HausdorffDual::process() {
	std::uniform_int_distribution<> rt(0, Universe::triangles.size());
    std::string tmp = "";
	max_epsilon = 15;
    for(int i = 1; i < max_epsilon; i++) {
		auto t = Universe::triangles[rt(rng)];

        std::vector<Triangle::Label> s1 = sphereDual(t, i);
        tmp += std::to_string(s1.size());
        tmp += " ";
    }
	tmp.pop_back();

    output = tmp; 
}

   

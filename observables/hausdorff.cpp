#include "hausdorff.hpp"

void Hausdorff::process() {
	std::uniform_int_distribution<> rv(0, Universe::vertices.size());
    std::string tmp = "";
	max_epsilon = 15;
    for(int i = 1; i < max_epsilon; i++) {
		auto v = Universe::vertices[rv(rng)];

        std::vector<Vertex::Label> s1 = sphere(v, i);
        tmp += std::to_string(s1.size());
        tmp += " ";
    }
	tmp.pop_back();

    output = tmp; 
}

   

#include "hausdorff.hpp"

void Hausdorff::process() {
    std::string tmp = "";
	max_epsilon = Universe::nSlices;
    for(int i = 1; i < max_epsilon; i++) {
		auto v = randomVertex();

        std::vector<Vertex::Label> s1 = sphere(v, i);
        tmp += std::to_string(s1.size());
        tmp += " ";
    }
	tmp.pop_back();

    output = tmp; 
}

   

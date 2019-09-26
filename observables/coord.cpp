// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "coord.hpp"

void Coord::process() {
	std::string tmp = "";
	for (auto v : Universe::vertices) {
		//tmp += std::to_string(Universe::vertexNeighbors.at(v).size());
		tmp += std::to_string(v->nUp);
		tmp += " ";
	}
	tmp.pop_back();
	
	output = tmp;
}

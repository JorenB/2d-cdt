// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include "volume_profile.hpp"

void VolumeProfile::process() {
	std::string tmp = "";
	for (auto l : Universe::sliceSizes) {
		tmp += std::to_string(l);
		tmp += " ";
	}
	tmp.pop_back();

	output = tmp;
}

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include "../observable.hpp"
#include "../universe.hpp"

class VolumeProfile : public Observable {
public:
	VolumeProfile(std::string id) : Observable(id) { name = "volume_profile"; }

	void process();
};

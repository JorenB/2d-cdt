// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef VOLUME_PROFILE_HPP_
#define VOLUME_PROFILE_HPP_

#include "../observable.hpp"
#include "../universe.hpp"

class VolumeProfile : public Observable {
public:
	VolumeProfile() : Observable() { name = "volume_profile"; }

	void process();
};
#endif  // VOLUME_PROFILE_HPP_

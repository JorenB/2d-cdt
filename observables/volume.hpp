// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef VOLUME_HPP_
#define VOLUME_HPP_

#include "../observable.hpp"
#include "../universe.hpp"

class Volume : public Observable {
public:
	Volume() : Observable() { name = "volume"; }

	void process();
};
#endif  // VOLUME_HPP_

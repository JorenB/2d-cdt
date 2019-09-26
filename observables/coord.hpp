// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef COORD_HPP_
#define COORD_HPP_

#include "../observable.hpp"
#include "../universe.hpp"

class Coord : public Observable {
public:
	Coord() : Observable() { name = "coord"; }

	void process();
};
#endif  // COORD_HPP_

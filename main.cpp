// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"

int main(int argc, const char * argv[]) {
	int slices = 300;
	Universe *u	= Universe::create(slices, 2);

	delete u;

	return 0;
}

// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"

int main(int argc, const char * argv[]) {
	Universe *u	= Universe::create(10, 1234);

	delete u;

	return 0;
}


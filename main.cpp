// Copyright 2018 Joren Brunekreef
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "universe.hpp"

int main(int argc, const char * argv[]) {
    // Universe u(10);

    // u.initialize();

	Universe u	= Universe::create(10);

    return 0;
}


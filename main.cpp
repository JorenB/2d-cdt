// Copyright 2018 Joren Brunekreef
#include <iostream>

#include "pool.hpp"
#include "bag.hpp"
#include "vertex.hpp"
#include "triangle.hpp"

int main(int argc, const char * argv[]) {
    Pool<Vertex, 10> vertices;
    Bag<Vertex, 10> special;

    Vertex &v1 = vertices.create();
    Vertex &v2 = vertices.create();
    Vertex &v3 = vertices.create();
    Vertex &v4 = vertices.create();
    Vertex &v5 = vertices.create();

    special.add(v1);

    return 0;
}


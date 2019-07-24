// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "vertex.hpp"
#include "triangle.hpp"

Triangle& Vertex::getTriangleLeft() { return Triangle::at(tl); }
Triangle& Vertex::getTriangleRight() { return Triangle::at(tr); }

void Vertex::setTriangleLeft(Triangle& t) { tl = t.key(); }
void Vertex::setTriangleRight(Triangle& t) { tr = t.key(); }

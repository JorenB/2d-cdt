// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#include "triangle.hpp"

void Triangle::setVertexLeft(Vertex& v) {
	vl = &v;
	time = vl->time;
	if (type == UP) {
		v.setTriangleRight(*this);
	}
}

void Triangle::setVertexRight(Vertex& v) { 
	vr = &v;
	if (type == UP) {
		v.setTriangleLeft(*this);
	}
}

void Triangle::setVertices(Vertex &vl_, Vertex &vr_, Vertex &vc_) {
	vl	= &vl_;
	vr	= &vr_;
	vc	= &vc_;

	time = vl->time;
	updateType();

	if (type == UP) {
		vl_.setTriangleRight(*this);
		vr_.setTriangleLeft(*this);
	}
}

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include "link.hpp"
#include "vertex.hpp"
#include "triangle.hpp"

Vertex::Label Link::getVertexFinal() {
	return vf;
}

Vertex::Label Link::getVertexInitial() {
	return vi;
}

Triangle::Label Link::getTrianglePlus() {
	return tp;
}

Triangle::Label Link::getTriangleMinus() {
	return tm;
}

void Link::setVertices(Vertex::Label vi_, Vertex::Label vf_) {
	vi = vi_;
	vf = vf_;
}

void Link::setTriangles(Triangle::Label tp_, Triangle::Label tm_) {
	tp = tp_;
	tm = tm_;
}

bool Link::isTimelike() {
	return vf->time != vi->time;
}

bool Link::isSpacelike() {
	return vf->time == vi->time;
}

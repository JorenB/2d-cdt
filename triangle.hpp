// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <stdio.h>
#include "pool.hpp"
#include "vertex.hpp"

class Triangle : public Pool<Triangle> {
public:
	static const unsigned pool_size = 1000;
	enum Type { UP, DOWN };

	int time;  // proper time at base of triangle
	Type type;

	Triangle& getTriangleLeft() const noexcept { return Triangle::at(tl); }
	Triangle& getTriangleRight() const noexcept { return Triangle::at(tr); }
	Triangle& getTriangleCenter() const noexcept { return Triangle::at(tc); }

	void setTriangleLeft(Triangle& t) {
		tl = t.key();
		t.tr = key();
	}

	void setTriangleRight(Triangle& t) {
		tr = t.key();
		t.tl = key();
	}

	void setTriangleCenter(Triangle& t) {
		tc = t.key();
		t.tc = key();
	}

	void setTriangles(Triangle &tl_, Triangle &tr_, Triangle &tc_) {
		tl = tl_.key();
		tr = tr_.key();
		tc = tc_.key();

		tl_.tr = key();
		tr_.tl = key();
		tc_.tc = key();
	}

	
	Vertex& getVertexLeft() const noexcept { return Vertex::at(vl); }
	Vertex& getVertexRight() const noexcept { return Vertex::at(vr); }
	Vertex& getVertexCenter() const noexcept { return Vertex::at(vc); }

	void setVertexLeft(Vertex& v) {
		vl = v.key();
		time = Vertex::at(vl).time;
		if (type == UP) {
			v.setTriangleRight(*this);
		}
	}

	void setVertexRight(Vertex& v) {
		vr = v.key();
		if (type == UP) {
			v.setTriangleLeft(*this);
		}
	}

	void setVertices(Vertex &vl_, Vertex &vr_, Vertex &vc_) {
		vl	= vl_.key();
		vr	= vr_.key();
		vc	= vc_.key();

		time = Vertex::at(vl).time;
		updateType();

		if (type == UP) {
			vl_.setTriangleRight(*this);
			vr_.setTriangleLeft(*this);
		}
	}

	void setVertexCenter(Vertex& v) { vc = v.key(); }

	bool isUpwards() {
		return type == UP;
	}

	bool isDownwards() {
		return type == DOWN;
	}

private:
	int	tl, tr, tc;
	int	vl, vr, vc;
	
	void updateType() {
		printf("vl.t: %d, vc.t: %d\n", Vertex::at(vl).time, Vertex::at(vc).time);
		if (Vertex::at(vl).time > Vertex::at(vc).time) {
			type = UP;
		} else {
			type = DOWN;
		}

		if (Vertex::at(vl).time == 0 && Vertex::at(vc).time > 1) {
			type = UP;
		}
		if (Vertex::at(vc).time == 0 && Vertex::at(vl).time > 1) {
			type = DOWN;
		}
	}
};

#endif  // TRIANGLE_HPP_

// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef triangle_hpp
#define triangle_hpp

#include <iostream>
#include "simplex.hpp"
#include "vertex.hpp"

class Triangle : public Simplex {
public:
	enum Type { UP, DOWN };

	int time;  // proper time at base of triangle
	Type type;

	Triangle& getTriangleLeft() const noexcept { return *tl; }
	Triangle& getTriangleRight() const noexcept { return *tr; }
	Triangle& getTriangleCenter() const noexcept { return *tc; }

	void setTriangleRight(Triangle& t) {
		tr		= &t;
		t.tl	= this;
	}

	void setTriangleLeft(Triangle& t) {
		tl		= &t;
		t.tr	= this;
	}

	void setTriangleCenter(Triangle& t) {
		tc		= &t;
		t.tc	= this;
	}

	void setTriangles(Triangle &tl_, Triangle &tr_, Triangle &tc_) {
		tl	= &tl_;
		tr	= &tr_;
		tc	= &tc_;

		tl_.tr = this;
		tr_.tl = this;
		tc_.tc = this;
	}

	Vertex& getVertexLeft() const noexcept { return *vl; }
	Vertex& getVertexRight() const noexcept { return *vr; }
	Vertex& getVertexCenter() const noexcept { return *vc; }

	void setVertexLeft(Vertex& v);
	void setVertexRight(Vertex& v);
	void setVertexCenter(Vertex& v) { vc = &v; }
	void setVertices(Vertex &vl_, Vertex &vr_, Vertex &vc_);

	bool isUpwards() {
		return type == UP;
	}

	bool isDownwards() {
		return type == DOWN;
	}

private:
	Triangle	*tl, *tr, *tc;
	Vertex		*vl, *vr, *vc;

	void updateType() {
		if (vl->time > vc->time) {
			type = UP;
		} else {
			type = DOWN;
		}

		if (vl->time == 0 && vc->time > 1) {
			type = UP;
		}
		if (vc->time == 0 && vl->time > 1) {
			type = DOWN;
		}
	}
};

#endif

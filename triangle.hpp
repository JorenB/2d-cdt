// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include <stdio.h>
#include "pool.hpp"
#include "vertex.hpp"

class Triangle : public Pool<Triangle> {
public:
	static const unsigned pool_size = 2*Vertex::pool_size;
	enum Type { UP, DOWN };

	int time;  // proper time at base of triangle
	Type type;

	Triangle::Label getTriangleLeft() const noexcept { return tl; }
	Triangle::Label getTriangleRight() const noexcept { return tr; }
	Triangle::Label getTriangleCenter() const noexcept { return tc; }

	void setTriangleLeft(Triangle::Label t) {
		tl = t;
		t->tr = *this;
	}

	void setTriangleRight(Triangle::Label t) {
		tr = t;
		t->tl = *this;
	}

	void setTriangleCenter(Triangle::Label t) {
		tc = t;
		t->tc = *this;
	}

	void setTriangles(Triangle::Label tl_, Triangle::Label tr_, Triangle::Label tc_) {
		tl = tl_;
		tr = tr_;
		tc = tc_;

		tl_->tr = *this;
		tr_->tl = *this;
		tc_->tc = *this;
	}

	
	Vertex::Label getVertexLeft() const noexcept { return vl; }
	Vertex::Label getVertexRight() const noexcept { return vr; }
	Vertex::Label getVertexCenter() const noexcept { return vc; }

	void setVertexLeft(Vertex::Label v) {
		vl = v;
		time = v->time;
		if (type == UP) {
			v->setTriangleRight(*this);
		}
	}

	void setVertexRight(Vertex::Label v) {
		vr = v;
		if (type == UP) {
			v->setTriangleLeft(*this);
		}
	}

	void setVertices(Vertex::Label vl_, Vertex::Label vr_, Vertex::Label vc_) {
		vl	= vl_;
		vr	= vr_;
		vc	= vc_;

		time = vl_->time;
		updateType();

		if (type == UP) {
			vl_->setTriangleRight(*this);
			vr_->setTriangleLeft(*this);
		}
	}

	void setVertexCenter(Vertex::Label v) { vc = v; }

	bool isUpwards() {
		return type == UP;
	}

	bool isDownwards() {
		return type == DOWN;
	}

private:
	Pool<Triangle>::Label tl, tr, tc;
	Pool<Vertex>::Label vl, vr, vc;
	
	void updateType() {
		if (vl->time < vc->time) {
			type = UP;
		} else {
			type = DOWN;
		}

		if (vc->time == 0 && vl->time > 1) {
			type = UP;
		}
		if (vl->time == 0 && vc->time > 1) {
			type = DOWN;
		}
	}
};
#endif  // TRIANGLE_HPP_

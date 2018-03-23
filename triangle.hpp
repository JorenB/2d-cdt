#ifndef triangle_hpp
#define triangle_hpp

#include "simplex.hpp"
#include "vertex.hpp"

class Triangle : public Simplex {
    public:
        enum { UP, DOWN };

        int time;

        Triangle	*tl, *tr, *tc;
        Vertex		*vl, *vr, *vc;

        Triangle& left() const noexcept { return *tl; }
        Triangle& right() const noexcept { return *tr; }
        Triangle& center() const noexcept { return *tc; }

        void right(Triangle& b) {
            tr		= &b;
            b.tl	= this;
        }

        void left(Triangle& b) {
             tl		= &b;
             b.tr	= this;
        }

        void center(Triangle& b) {
            tc		= &b;
            b.tc	= this;
        }

        Vertex& vertex_left() const noexcept { return *vl; }
        Vertex& vertex_right() const noexcept { return *vr; }
        Vertex& vertex_center() const noexcept { return *vc; }

        void set_vertices(Vertex &vl_, Vertex &vr_, Vertex &vc_) {
            vl	= &vl_;
            vr	= &vr_;
            vc	= &vc_;

            vl_.rightmost(*this);
            vr_.leftmost(*this);
        }
        // type up/down ...
};

#endif

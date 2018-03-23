#ifndef vertex_hpp
#define vertex_hpp

#include "simplex.hpp"

class Triangle; 
class Vertex : public Simplex {
    public:
        int time;

        Triangle *tl, *tr;

        Triangle& leftmost() { return *tl; }
        Triangle& rightmost() { return *tr; }
        
        void leftmost(Triangle &t)  { tl = &t; }
        void rightmost(Triangle &t) { tr = &t; }

        /*int nu, nd;													//
        void coord(int xu, int xd) {								// Change coord
            nu	+= xu;
            nd	+= xd;
        }

        void set_n(int nu_, int nd_) {
            nu	= nu_;
            nd	= nd_;
        }
*/
};

#endif

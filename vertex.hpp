#ifndef vertex_hpp
#define vertex_hpp

#include "simplex.hpp"

class Triangle; 
class Vertex : public Simplex {
    public:
        int time;

        int neighboursUp, neighboursDown;                       // No. triangles above and below sharing this vertex

        void changeNeighbourNumber(int up, int down) {
            neighboursUp += up;
            neighboursDown += down;
        }

        void setNeighbourNumber(int up, int down) {
            neighboursUp = up;
            neighboursDown = down;
        }

        Triangle& getTriangleLeft() { return *tl; }
        Triangle& getTriangleRight() { return *tr; }
        
        void setTriangleLeft(Triangle &t)  { tl = &t; }
        void setTriangleRight(Triangle &t) { tr = &t; }

        int key() const { return key_; }

        int key(int key__) {
            auto tmp = key_;
            key_    = key__;
            return tmp;
        }


    private:
        Triangle *tl, *tr;
        int key_;

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

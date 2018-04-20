#ifndef universe_hpp
#define universe_hpp

#include <vector>
#include "vertex.hpp"
#include "triangle.hpp"
#include "pool.hpp"
#include "bag.hpp"

#define N_TRIANGLES 1000000
#define N_VERTICES 1000000

class Universe {
    public:
        Pool<Triangle, N_TRIANGLES> triangles;
        Pool<Vertex, N_VERTICES> vertices;

        Bag<Vertex, N_VERTICES> verticesDelete;  // vertices with coordination number 4. These are candidates for the delete move
        Bag<Vertex, N_VERTICES> verticesFlip;   // vertices with more than two upwards pointing links. These can be used in a flip move

        std::vector<int> sliceSizes;
        int nSlices;

        Universe(int n_slices);

        void initialize();

        static Universe* create(int n_slices);

        // moves
        void moveAdd(Triangle& t);
        void moveDelete(Vertex& v);
        enum flipSide { LEFT, RIGHT };
        void moveFlip(Vertex& v, flipSide side);

        // bag consistency
        void updateVertexBags(Vertex& v);
        void updateNeighbourNumber(Vertex &v, int up, int down);
}; 
#endif

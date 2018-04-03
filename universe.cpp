#include "universe.hpp"
#include <stdio.h>

Universe::Universe(int nSlices) : nSlices(nSlices) {
    std::vector<int> sliceSizes(nSlices);
} 

void Universe::initialize() {
    int w = 10; //  width of the initial strip. Can be adjusted for thermalization purposes - unclear what the ``optimal'' value is.
    int t = nSlices;

    std::vector<Vertex*> initialVertices(w*t);

    for (int i = 0; i < w*t; i++) {
        initialVertices[i] = &vertices.create();
        initialVertices[i]->time = i/w;
    }

    for (int i = 0; i < t; i++) {
        sliceSizes.push_back(w);
    }

    //  create triangles
    std::vector<Triangle*> initialTriangles(2*w*t);
    for(int i = 0; i < t; i++) {
        for(int j = 0; j < w; j++) {
            Triangle& tl = triangles.create();
            tl.setVertices(
                    *initialVertices[i*w+j], 
                    *initialVertices[i*w+(j+1)%w], 
                    *initialVertices[((i+1)%t)*w+j]
                    );
            initialTriangles[2*(i*w+j)] = &tl;

            Triangle& tr = triangles.create();
            tr.setVertices(
                    *initialVertices[((i+1)%t)*w+j],
                    *initialVertices[((i+1)%t)*w+(j+1)%w],
                    *initialVertices[i*w+(j+1)%w]
                    );
            initialTriangles[2*(i*w+j)+1] = &tr;
        }
    }

    //  set triangle connectivity
    int row = 0, column = 0;
    for(int i = 0; i < t; ++i) {
        for(int j = 0; j < w; ++j) {
            row = 2*i*w;
            column = 2*j;
            initialTriangles[row + column]->setTriangles(
                    *initialTriangles[row + (column-1+2*w)%(2*w)],
                    *initialTriangles[row + column + 1],
                    *initialTriangles[(row + column -2*w+1+2*t*w)%(2*t*w)]
                    );

            initialTriangles[row + column +1]->setTriangles(
                    *initialTriangles[row + column],
                    *initialTriangles[row + (column+2)%(2*w)],
                    *initialTriangles[(row + column + 2*w)%(2*t*w)]
                    );
        }
    }

    Triangle *tri = initialTriangles[3];
    moveAdd(*tri);

    for (int i = 1; i < triangles.size()+1; i++) {
        Triangle tr = triangles[i];
        printf("tri: %d; time: %d, type: %s, tl: %d, tr: %d, tc: %d, vl: %d, vr: %d, vc: %d\n", tr.getKey(),
                tr.time, 
                tr.type == Triangle::Type::UP ? "up" : "down",
                tr.getTriangleLeft().getKey(),
                tr.getTriangleRight().getKey(),
                tr.getTriangleCenter().getKey(),
                tr.getVertexLeft().getKey(), 
                tr.getVertexRight().getKey(), 
                tr.getVertexCenter().getKey());
    }


    for (int i = 1; i < vertices.size()+1; i++) {
        Vertex v = vertices[i];
        printf("v: %d; tl: %d, tr: %d\n", v.getKey(), v.getTriangleLeft().getKey(), v.getTriangleRight().getKey());
    }
    
}

void Universe::moveAdd(Triangle& t) {
    Triangle& tc = t.getTriangleCenter();

    Vertex& vr = t.getVertexRight();

    int time = t.time;

    Vertex& v = vertices.create();
    v.time = time;
    sliceSizes[time] += 1;

    t.setVertexRight(v);
    tc.setVertexRight(v);

    Triangle& t1 = triangles.create(); //  right neighbour of t
    Triangle& t2 = triangles.create(); //  right neighbour of tc

    t1.setVertices(v, vr, t.getVertexCenter());
    t2.setVertices(v, vr, tc.getVertexCenter());

    t1.setTriangles(t, t.getTriangleRight(), t2);
    t2.setTriangles(tc, tc.getTriangleRight(), t1);
}

/****
 *
 * Bag is an implementation of a set-like data structure.
 * It provides fast (O(1)) add, remove and pick operations.
 * It stores integer values less than N.
 *
 ****/
#ifndef bag_hpp
#define bag_hpp

#include <cassert>

template <class T, unsigned int N>  // or size_t or int
class Bag {
    private:
        int* indices = new int[N+1];
        //std::array<int, N+1>	indices;  // with holes, indexed by labels, holds indices of obj[]
        int* elements = new int[N];
        //std::array<int, N+1>	elements;  // continuous, holds labes
        unsigned int containerSize;
        unsigned int elementsSize;

enum : int {
           EMPTY = -1	 // or constexpr
       };

    public:

       Bag() : containerSize(N+1), elementsSize(0) {
           for(int i = 0; i < containerSize; i++) {
               indices[i] = EMPTY;  // initialize indices with EMPTY
           }

           for(int i = 0; i < containerSize; i++) {
               elements[i] = EMPTY;  // initialize elements with EMPTY
           }
       }

       bool contains(int key) const {
           return indices[key] != EMPTY;
       }

       bool contains(T& obj) { return contains(obj.getKey()); }

       void add(T& obj) { add(obj.getKey()); }
       void remove(T& obj) { remove(obj.getKey()); }

       void add(int key) {
           //assert(!contains(key));  // check is currently performed in Universe

           indices[key]		= elementsSize;
           elements[elementsSize]	= key;
           elementsSize++;
       }


       void remove(int key) {
           //assert(contains(key));  // check is currently performed in Universe
           elementsSize--;

           auto index	= indices[key];
           auto last	= elements[elementsSize];

           elements[index]		= last;
           elements[elementsSize] = EMPTY;
           indices[last]		= index;
           indices[key]		= EMPTY;
       }

       void log() {
           //printf("indices\n");
           /*for(int i = 0; i < containerSize; i++) {
               //printf("%d: %d\n", i, indices[i]);
           }*/
           printf("elements\n");
           for(int i = 0; i < elementsSize; i++) {
               printf("%d: %d\n", i, elements[i]);
           }

           printf("--\n");
       }

       // fix the pick function - think about desired behavior
       int pick(uint64_t r) const {
           /*uint64_t x	= (r >> 32) * elementsSize;
           unsigned int i = (x >> 32);*/
           assert(r < elementsSize);
           return elements[r];
       }

       int size() const {
           return elementsSize;
       }
};
#endif

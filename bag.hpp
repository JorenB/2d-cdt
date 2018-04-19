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
        unsigned int capacity_;                             // Maybe call capacity_
        unsigned int size_;                              // size_

enum : int {
           EMPTY = -1	 // or constexpr
       };

    public:

       Bag() : capacity_(N+1), size_(0) {
           for(unsigned int i = 0; i < capacity_; i++) {
               indices[i] = EMPTY;  // initialize indices with EMPTY
           }

           for(unsigned int i = 0; i < capacity_ - 1; i++) {
               elements[i] = EMPTY;  // initialize elements with EMPTY
           }
       }

       void add(T& obj) { add(obj.key()); }
       void remove(T& obj) { remove(obj.key()); }
       bool contains(T& obj) { return contains(obj.key()); }

       void add(int key) {
           //assert(!contains(key));  // check is currently performed in Universe

           indices[key]		= size_;
           elements[size_]	= key;
           size_++;
       }

       void remove(int key) {
           //assert(contains(key));  // check is currently performed in Universe
           size_--;

           auto index	= indices[key];
           auto last	= elements[size_];

           elements[index]		= last;
           elements[size_] = EMPTY;
           indices[last]		= index;
           indices[key]		= EMPTY;
       }

       bool contains(int key) const {
           return indices[key] != EMPTY;
       }

       void log() {
           //printf("indices\n");
           /*for(int i = 0; i < capacity_; i++) {
               //printf("%d: %d\n", i, indices[i]);
           }*/
           printf("elements\n");
           for(int i = 0; i < size_; i++) {
               printf("%d: %d\n", i, elements[i]);
           }

           printf("--\n");
       }

       // fix the pick function - think about desired behavior
       int pick(uint64_t r) const {
           /*uint64_t x	= (r >> 32) * size_;
           unsigned int i = (x >> 32);*/
           assert(r < size_);
           return elements[r];
       }

       int size() const {
           return size_;
       }
};
#endif

/****
 *
 * Bag is an implementation of a set-like data structure.
 * It provides fast (O(1)) add, remove and pick operations.
 * It stores integer values less than N.
 *
 ****/
#ifndef bag_hpp
#define bag_hpp

template <class T, unsigned int N>										// or size_t or int
class Bag {
    private:
        std::array<int, N+1>	indices;									// with holes, indexed by labels, holds indices of obj[]
        std::array<int, N+1>	elements;									// continuous, holds labes
        unsigned int _size;

enum : int {
           EMPTY = -1												// or constexpr
       };

    public:

       Bag() : _size{0} {
           indices.fill(EMPTY);										// initialize indices with EMPTY
           elements.fill(EMPTY);
       }

       bool contains(int key) const {
           return indices[key] != EMPTY;
       }

       void add(T& obj) { add(obj.getKey()); }
       void remove(T& obj) { remove(obj.getKey()); }

       void add(int key) {
           //assert(!contains(key));									// or check directly

           indices[key]		= _size;
           elements[_size]	= key;
           _size		++;
       }


       void remove(int key) {
           //assert(contains(key));
           _size --;

           auto index	= indices[key];
           auto last	= elements[_size];

           elements[index]		= last;
           elements[_size] = EMPTY;
           indices[last]		= index;
           indices[key]		= EMPTY;
       }

       void log() {
           printf("indices\n");
           for(int i = 0; i < indices.size(); i++) {
               printf("%d: %d\n", i, indices[i]);
           }
           printf("elements\n");
           for(int i = 0; i < elements.size(); i++) {
               printf("%d: %d\n", i, elements[i]);
           }

           printf("--\n");
       }

       /*int pick(uint64_t r) const {
           uint64_t x	= (r >> 32) * _size;
           unsigned int i = (x >> 32);
           // assert(i < _size);
           return obj[i];
       }*/

       int size() const {
           return _size;
       }
};
#endif

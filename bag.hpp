// Copyright 2018 Joren Brunekreef and Andrzej Görlich
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
#include <random>

template <class T, unsigned int N>  // or size_t or int
class Bag {
private:
	std::array<int, N> indices;  // with holes, indexed by labels, holds indices of obj[]
	std::array<int, N> elements;  // continuous, holds labes
	unsigned int capacity_;
	unsigned int size_;
	std::default_random_engine &rng;

enum : int {
		   EMPTY = -1  // or constexpr
	   };

public:
	   explicit Bag(std::default_random_engine &rng) : capacity_(N), size_(0), rng(rng) {
		   indices.fill(EMPTY);  // initialize indices with EMPTY
	   }

	   void add(T& obj) { add(obj.key()); }
	   void remove(T& obj) { remove(obj.key()); }
	   bool contains(T& obj) { return contains(obj.key()); }

	   void add(int key) {
		   // assert(!contains(key));  // check is currently performed in Universe
		   indices[key]	= size_;
		   elements[size_] = key;
		   size_++;
	   }

	   void remove(int key) {
		   // assert(contains(key));  // check is currently performed in Universe
		   size_--;

		   auto index = indices[key];
		   auto last = elements[size_];

		   elements[index] = last;
		   elements[size_] = EMPTY;
		   indices[last] = index;
		   indices[key] = EMPTY;
	   }

	   bool contains(int key) const {
		   return indices[key] != EMPTY;
	   }

	   void log() {
		   // printf("indices\n");
		   /* for(int i = 0; i < capacity_; i++) {
		   // printf("%d: %d\n", i, indices[i]);
		   } */
		   printf("elements\n");
		   for (int i = 0; i < size_; i++) {
			   printf("%d: %d\n", i, elements[i]);
		   }

		   printf("--\n");
	   }

	   int pick() const {
		   std::uniform_int_distribution<> uniform(0, size_ - 1);
		   return elements[uniform(rng)];
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

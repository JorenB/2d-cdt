// Copyright 2020 Joren Brunekreef and Andrzej Görlich
/****
 *
 * Bag is an implementation of a set-like data structure.
 * It provides fast (O(1)) add, remove and pick operations.
 * It stores integer values less than N.
 *
 ****/
#pragma once

#include <cassert>
#include <random>

template <class T, unsigned int N>  // or size_t or int
class Bag {
	using Label	= typename T::Label;

private:
	std::array<int, N> indices;  // With holes, indexed by labels, holds indices of obj[]
	std::array<Label, N> elements;  // Continuous, holds labels
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

	   int size() const noexcept {
		   return size_;
	   }

	   bool contains(Label obj) const {
		   return indices[obj] != EMPTY;  // Automatic Label to int conversion
	   }

	   void add(Label obj) {
		   assert(!contains(obj));  // check is currently performed in Universe
		   indices[obj]	= size_;
		   elements[size_] = obj;
		   size_++;
	   }

	   void remove(Label obj) {
		   assert(contains(obj));  // check is currently performed in Universe
		   size_--;

		   auto index = indices[obj];
		   auto last = elements[size_];

		   elements[index] = last;
		   elements[size_] = EMPTY;
		   indices[last] = index;
		   indices[obj] = EMPTY;
	   }

	   Label pick() const {
		   assert(size_ > 0);
		   std::uniform_int_distribution<> uniform(0, size_ - 1);
		   return elements[uniform(rng)];
	   }

	   void log() {
		   printf("elements\n");
		   for (int i = 0; i < size_; i++) {
			   printf("%d: %d\n", i, elements[i]);
		   }

		   printf("--\n");
	   }


	   void clear() {
	       indices.fill(EMPTY);
	       elements.fill(EMPTY);
	       size_ = 0;
	   }

	   //// Iterator for objects stored in a Bag ////

	   auto begin() { return &elements[0]; }
	   auto end() { return &elements[size_]; }
};

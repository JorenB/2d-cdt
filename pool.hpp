// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef POOL_HPP_
#define POOL_HPP_

#include <array>
// T is the type of object stored
// N is the size of the pool
// Poolable objects need getter and setter methods for the key variable
// to store their position in the array
template <class T, int N>
class Pool {
public:
	std::array<T, N> elements;
	int first;  // the index of the first 'empty' slot
	int total;  // total number of 'active' slots

	Pool() {
		first = 1;
		total = 0;
		for (int i = 0; i < N; i++) {
			elements[i].key(- (i + 1));
		}
	}

	T& create() {
		T& obj = elements[first];
		auto tmp = obj.key();
		obj.key(first);
		first = -tmp;  // update key of first empty slot and 'activate' new object
		total++;
		return obj;
	}

	void destroy(int i) {
		elements[i].key(-first);  // 'deactivate' object at position i
		first = i;  // reset index of first inactive object
		total--;
	}

	void destroy(T& obj) {
		auto tmp = obj.key();
		obj.key(-first);
		first = tmp;  // 'deactivate' object and update first empty slot
		total--;
	}

	int size() const {
		return total;
	}

	T& operator[] (int i) {
		return elements[i];
	}
};
#endif  // POOL_HPP_

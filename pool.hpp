// Copyright 2018 Joren Brunekreef and Andrzej Görlich
#ifndef pool_hpp
#define pool_hpp

#include <array>
// T is the type of object stored
// N is the size of the pool
// Poolable objects need getter and setter methods for the key variable
// to store their position in the array
template <class T, int N>
class Pool {
public:
	T* elements = new T[N + 1];
	int first;  // the index of the first 'empty' slot
	int total;  // total number of 'active' slots

	Pool() {
		first = 1;
		total = 0;
		for (int i = 1; i < N + 1; i++) {
			elements[i].key(- (i + 1));
		}
	}

	T& create() {
		T& obj = elements[first];
		first = -obj.key(first);  // update key of first empty slot and 'activate' new object
		total++;
		return obj;
	}

	void destroy(int i) {
		elements[i].key(-first);  // 'deactivate' object at position i
		first = i;  // reset index of first inactive object
		total--;
	}

	void destroy(T& obj) {
		first = obj.key(-first);  // 'deactivate' object and update first empty slot
		total--;
	}

	int size() const {
		return total;
	}

	T& operator[] (int i) {
		return elements[i];
	}

	void log() {
		for (int i = 0; i < sizeof(elements); i++) {
			printf("%d: %d \n", i, elements[i].getKey());
		}
		printf("first: %d\n", first);
		printf("--\n");
	}
};
#endif

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once
/****

A Simplex<T> contains a single pool of objects of type ....

There exists only a single copy of a pool for given type.

 ****/

#include <cstdio>
#include <array>
#include <cassert>
#include <random>
#include <string>
#include <typeinfo>

/****
 *
 * Pool is a template class that maintains 
 * a single, static pool (array) of objects of given class T.
 * The T class should then extend the Pool<T>
 *
 ****/

template<class T>
class Pool {
private:
	static T *elements;  // elements shouldn't be public so not to change it outside but can be returned
	static int first;  // Index of the first empty cell
	static int total;  // No. used cells
	static int capacity;
	int next;  // Label or next free entry in pool (maybe protected)

	friend T;  // Private constructor only to prevent inheriting wrong template class realization,
	Pool() = default;  // like: Link : Pool<Vertex>. Has to be a friend

protected:
	static const unsigned pool_size = -1;  // Has to be overshaded in child class

public:
	Pool(const Pool&) = delete;  // Make poolable objects non-copyable
	Pool& operator=(const Pool&) = delete;
	Pool(Pool&&) = delete;  // Make poolable objects non-movable
	Pool& operator=(Pool&&) = delete;

/**** Label ****
 *
 * The nested Label type serves as a pointer to T,
 * but internally it is an integer.
 * Can be referred as T::Label.
 *
 ***************/

	class Label {
	private:
		int i;  // const?
	public:
		Label() = default;
		Label(int i) : i{i} { }
		T& operator*() const { return T::at(i); }
		T* operator->() const { return &T::at(i); }
		operator int&() { return i; }  // Basically a getter/setter
		operator int() const { return i; }  // Basically a getter
	};

	operator Label() const { return Label{next}; }

	static T* create_pool() {
		static_assert(T::pool_size > 0, "Pool size not defined in child class");

		capacity = T::pool_size;
		elements = new T[capacity];

		for(auto i = 0; i < capacity; i++)
			elements[i].next = ~(i + 1);  // Using not (~) solves the negative zero problem. ~x == -(x + 1)

		return elements;
	}

	static Label create() {
		auto tmp = first;
		assert(elements[tmp].next < 0);  // Check if element is really free
		first = ~elements[tmp].next;
		elements[tmp].next = tmp;
		total++;
		return tmp;  // Implicit 'Label(int)'' constructor
	}

	static void destroy(Label i) {
		elements[i].next = ~first;  // 'deactivate' object at position i
		first = i;  // Reset index of the first inactive object. Implicit Label::operator int&().
		total--;
	}

	static T& at(int i) { return elements[i]; }

	static int size() noexcept { return total; }
	static int pool_capacity() noexcept { return capacity; }

//// Checks if the object is indeed in the right position in array 'elements' ////
	void check_in_pool() {
		assert(this->next >= 0);
		assert(this->next < capacity);
		assert(this == elements + this->next);
	}

	void destroy() {
		check_in_pool();
		destroy(this->next);
	}

//// Pool iterator - even though there are no pool objects ////

	struct Iterator {
	private:
		int i;
		int cnt;

	public:
		Iterator(int i = 0, int cnt = 0) : i{i}, cnt{cnt} {}

		T& operator*() { return elements[i]; }
		bool operator==(const Iterator& b) const { return cnt == b.cnt; }
		bool operator!=(const Iterator& b) const { return !operator == (b); }

		Iterator& operator++() {
			if(cnt < total - 1)
				while(elements[++i].next < 0) continue;
			cnt++;

			return *this;
		}
	};

	struct Items {
		auto begin() {
			int i;
			for(i = 0; elements[i].next < 0; i++) continue;
			return Iterator{i, 0};
		}
		auto end() {
			return Iterator{-1, total};
		}
	};

	static Items items() { return Items{}; }  // items() return Items object with begin() and end() methods
};

template<class T> T* Pool<T>::elements = Pool<T>::create_pool();
template<class T> int Pool<T>::first{0};
template<class T> int Pool<T>::total{0};
template<class T> int Pool<T>::capacity;

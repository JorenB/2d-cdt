#ifndef POOL_HPP_
#define POOL_HPP_
template<class T>
class Pool {
private:
	static T *elements;
	static int first;  // Index of the first empty cell
	static int total;  // No. used cells
	static int capacity;
	int next;  // Label or next free entry in pool (maybe protected)

	Pool() {}  // Private constructor only to prevent inheriting wrong template class realization,
	friend T;  // like: Link : Pool<Vertex>. Has to be a friend

protected:
	static const unsigned pool_size = -1;  // Has to be overshaded in child class

public:
	static T* create_pool() {
		const unsigned capacity	= T::pool_size;

		static_assert(capacity > 0, "Pool size not defined in child class");
		elements = new T[capacity];

		for(auto i = 0; i < capacity; i++) {
			elements[i].next = -(i + 1);  // Equivalent to ~i
		}

		return elements;
	}

	static int create() {
		auto tmp = first;

		//	Add assert

		first = - elements[tmp].next;
		elements[tmp].next = tmp;
		total++;
		return tmp;
	}

	static void destroy(int i) {
		elements[i].next = - first;  // 'deactivate' object at position i
		first = i;  // Reset index of the first inactive object
		total--;
	}

	static T& at(int i) { return elements[i]; }

	static int size() noexcept { return total; }
	static int pool_capacity() noexcept { return capacity; }

	int key() const { return next; }  // Getter
	void key(int val) { next = val; }  // Setter

	void destroy() {
		auto tmp = this->next;  // Index in the pool of the object to destroy
		assert(this == elements + tmp);
		this->next	= - first;
		first = tmp;
		total--;
	}
};

template<class T> T* Pool<T>::elements = Pool<T>::create_pool();
template<class T> int Pool<T>::first{1};
template<class T> int Pool<T>::total{0};
template<class T> int Pool<T>::capacity;
#endif

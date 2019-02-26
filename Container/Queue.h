#ifndef QUEUE_H
#define QUEUE_H
#include"Deque.h"

template<typename T,typename Container=Deque<T>>
class Queue {
public:
	typedef T value_type;
	typedef Container container_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
private:
	container_type _container;
public:
	explicit Queue(const container_type &ctnr) :_container(ctnr) {};

	explicit Queue(container_type &&ctnr = container_type()) :_container(ctnr) {};

	template<typename Alloc>
	explicit Queue(const Alloc &alloc) :_container(alloc) {};

	template<typename Alloc>
	explicit Queue(const container_type &ctnr, const Alloc &alloc) :_container(ctnr, alloc) {};

	template<typename Alloc>
	explicit Queue(const Queue &x, const Alloc &alloc) :_container(x._container, alloc) {};

	bool empty()const {
		return _container.empty();
	}

	size_type size()const {
		return _container.size();
	}

	reference front() {
		return _container.front();
	}

	const_reference front()const {
		return _container.front();
	}

	reference back() {
		return _container.back();
	}

	const_reference back()const {
		return _container.back();
	}

	void push(const value_type &val) {
		_container.push_back(val);
	}

	template<typename... Args>
	void emplace(Args&&... args) {
		_container.emplace_back(args...);
	}

	void pop() {
		_container.pop_front();
	}

	void swap(Queue &x) {
		_container.swap(x._container);
	}
};

#endif // !QUEUE_H
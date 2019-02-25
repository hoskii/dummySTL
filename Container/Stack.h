#ifndef STACK_H
#define STACK_H
#include"Deque.h"

template<typename T>
class Stack {
public:
	typedef T        value_type;
	typedef Deque<T> container_type;
	typedef T&       reference;
	typedef const T& const_reference;
	typedef size_t   size_type;
private:
	container_type _deque;
public:
	explicit Stack(const container_type &ctnr) :_deque(ctnr) {};

	explicit Stack(container_type &&ctnr = container_type()) :_deque(ctnr) {};

	template<typename Alloc> 
	explicit Stack(const Alloc &alloc) :_deque(alloc) {};

	template<typename Alloc> 
	Stack(const container_type &ctnr, const Alloc &alloc) :_deque(ctnr, alloc) {};

	template<typename Alloc> 
	Stack(const Stack &x, const Alloc &alloc) :_deque(x._deque, alloc) {};

	bool empty() const {
		return _deque.empty();
	}

	size_type size() const {
		return _deque.size();
	}

	reference& top() {
		return _deque.back();
	}

	const_reference& top() const {
		return _deque.back();
	}

	void push(const value_type &val) {
		_deque.push_back(val);
	}

	template<typename... Args> 
	void emplace(Args&&... args) {
		_deque.emplace_back(args...);
	}

	void pop() {
		_deque.pop_back();
	}

	void swap(Stack &x) {
		_deque.swap(x._deque);
	}
};

#endif // !STACK_H
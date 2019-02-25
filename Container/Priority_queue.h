#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE
#include"Vector.h"

template<typename T,typename Container = Vector<T>,typename Compare = std::less<T>>
class Priority_queue {
public:
	typedef T         value_type;
	typedef Container container_type;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef size_t    size_type;
private:
	Container _container;
	Compare _compare;
public:
	Priority_queue(const Compare &comp, const Container &ctnr) :_compare(comp),_container(ctnr) {
		if (!_container.empty()) {
			value_type v = _container[0];
			_container.push_back(v);
			_make_heap();
		}
	}

	template <typename InputIterator>
	Priority_queue(InputIterator first, InputIterator last, const Compare &comp, const Container &ctnr) : _compare(comp),_container(ctnr) {
		if (!_container.empty()) {
			value_type v = _container[0];
			_container.push_back(v);
			_container.insert(_container.end(), first, last);
			_make_heap();
		}
		else if(first!=last){
			_container.push_back(*first);
			_container.insert(_container.end(), first, last);
			_make_heap();
		}
	}

	explicit Priority_queue(const Compare &comp = Compare(), Container &&ctnr = Container()) :_container(ctnr), _compare(comp) {};

	template <typename Alloc> 
	explicit Priority_queue(const Alloc &alloc) :_container(alloc) {};

	template <typename Alloc> 
	Priority_queue(const Compare &comp, const Alloc &alloc) :_container(alloc), _compare(comp) {};

	template <typename Alloc> 
	Priority_queue(const Compare &comp, const Container &ctnr, const Alloc &alloc) :_container(alloc), _compare(comp) {
		if (!_container.empty()) {
			value_type v = _container[0];
			_container.push_back(v);
			_make_heap();
		}
	}

	template <typename Alloc> 
	Priority_queue(const Priority_queue &x, const Alloc &alloc) :_container(x._container, alloc), _compare(x._compare) {};

	bool empty() const {
		return _container.empty() || _container.size() == 1;
	}

	size_type size() const {
		return _container.size() - 1;
	}

	const_reference top() const {
		return _container.at(1);
	}

	void push(const value_type &val) {
		if(_container.empty())
			_container.push_back(val);
		_container.push_back(val);
		_upstream(size());
	}

	template <typename... Args> 
	void emplace(Args&&... args) {
		value_type t(args...);
		push(t);
	}

	void pop() {
		if (empty())
			throw std::exception("");
		std::swap(_container[1], _container[size()]);
		_container.pop_back();
		_heapify(1);
	}

	void swap(Priority_queue &x) {
		_container.swap(x._container);
		Compare c(_compare);
		_compare = x._compare;
		x._compare = c;
	}
private:
	void _make_heap() {
		size_type i = _parent(size());
		while (i > 0) {
			_heapify(i);
			--i;
		}
	}

	void _heapify(size_type i) {
		size_type index = i;
		size_type left = _left_child(i);
		size_type right = _right_child(i);
		if (left < _container.size() && _compare(_container[index], _container[left]))
			index = left;
		if (right < _container.size() && _compare(_container[index], _container[right]))
			index = right;
		if (index != i) {
			std::swap(_container[i], _container[index]);
			_heapify(index);
		}
	}

	void _upstream(size_type i) {
		while (i > 1 && _compare(_container[_parent(i)], _container[i])) {
			std::swap(_container[_parent(i)], _container[i]);
			i = _parent(i);
		}
	}

	size_type _left_child(size_type i) {
		return i * 2;
	}

	size_type _right_child(size_type i) {
		return i * 2 + 1;
	}

	size_type _parent(size_type i) {
		return i / 2;
	}
};

#endif // !PRIORITY_QUEUE
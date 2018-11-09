#ifndef VECTOR_H
#define VECTOR_H
#include<memory>
#include<stdexcept>
#include<initializer_list>
#include<type_traits>
#include<iterator>
#include<exception>

template<typename T, typename Alloc = std::allocator<T>>
class Vector {
public:
	typedef Alloc                                 allocator_type;
	typedef size_t                                size_type;
	typedef T                                     value_type;
	typedef T&                                    reference;
	typedef const T&                              const_reference;
	typedef T*                                    iterator;
	typedef const T*                              const_iterator;
	typedef T*                                    pointer;
	typedef const T*                              const_pointer;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
private:
	iterator _start;
	iterator  _end;
	iterator _endOfStorage;
	allocator_type _alloc;
public:
	explicit Vector(const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_start = _end = _alloc.allocate(1);
		_endOfStorage = _start + 1;
	};

	explicit Vector(size_type n)  {
		if (n == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(n);
		_end = _endOfStorage = std::uninitialized_fill_n(_start, n, value_type());
	};

	Vector(size_type n, const value_type &val, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		if (n == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(n);
		_end = _endOfStorage = std::uninitialized_fill_n(_start, n, val);
	};
	
	template<typename InputIterator>
	Vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_construction_aux(first, last, std::is_integral<InputIterator>::type());
	};
	
	Vector(const Vector &x) {
		if (x.empty()) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(x.size());
		_end = _endOfStorage = std::uninitialized_copy(x.begin(), x.end(), _start);
	};

	Vector(const Vector &x, const allocator_type &alloc) :_alloc(alloc) {
		if (x.empty()) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(x.size());
		_end = _endOfStorage = std::uninitialized_copy(x.begin(), x.end(), _start);
	};

	Vector(Vector &&x):_start(x._start),_end(x._end),_endOfStorage(x._endOfStorage) {
		x._start = x._end = x._endOfStorage = nullptr;
	};

	Vector(Vector &&x, const allocator_type &alloc) :_start(x._start), _end(x._end), _endOfStorage(x._endOfStorage), _alloc(alloc) {
		x._start = x._end = x._endOfStorage = nullptr;
	};

	Vector(std::initializer_list<value_type> il, const allocator_type &alloc = allocator_type()) : _alloc(alloc) {
		if (il.size() == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(il.size());
		_end = _endOfStorage = std::uninitialized_copy(il.begin(), il.end(), _start);
	};

	~Vector() {
		_clear_aux();
	};
	
	Vector& operator=(const Vector &x) {
		if (x._start == _start)
			return *this;
		_clear_aux();
		if (x.empty()) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return *this;
		}
		_start = _alloc.allocate(x.size());
		_end = _endOfStorage = std::uninitialized_copy(x.begin(), x.end(), _start);
		return *this;
	};

	Vector& operator=(Vector&& x) {
		_clear_aux();
		_start = x._start;
		_end = x._end;
		_endOfStorage = x._endOfStorage;
		x._start = x._end = x._endOfStorage = nullptr;
		return *this;
	};

	Vector& operator=(std::initializer_list<value_type> il) {
		_clear_aux();
		if (il.size() == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return *this;
		}
		_start = _alloc.allocate(il.size());
		_end = _endOfStorage = std::uninitialized_copy(il.begin(), il.end(), _start);
		return *this;
	}

	iterator begin() {
		return _start;
	}

	const_iterator begin()const {
		return _start;
	}

	iterator end() {
		return _end;
	}

	const_iterator end()const {
		return _end;
	}

	reverse_iterator rbegin() {
		return reverse_iterator(_end);
	}

	const_reverse_iterator rbegin()const {
		return const_reverse_iterator(_end);
	}

	reverse_iterator rend() {
		return reverse_iterator(_start);
	}

	const_reverse_iterator rend()const {
		return const_reverse_iterator(_start);
	}

	const_iterator cbegin()const {
		return _start;
	}

	const_iterator cend()const {
		return _end;
	}

	const_reverse_iterator crbegin()const {
		return const_reverse_iterator(_end);
	}

	const_reverse_iterator crend()const {
		return const_reverse_iterator(_start);
	}
	
	size_type size()const {
		return _end - _start;
	}

	size_type max_size()const {
		//testing on my machine ???
		return 1073741823;
	}

	void resize(size_type n) {
		resize(n, value_type());
	}

	void resize(size_type n,const value_type &val) {
		if (n == 0) {
			_clear_aux();
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		else if (n < size()) {
			if (!is_trivially_destructible<T>::value){
				iterator it = _start + n;
				while (it != _end)
					_alloc.destroy(it++);
			}
			_end = _start + n;
		}
		else if (n > size()) {
			if (n - size() <= _endOfStorage - _end) {
				iterator newEnd = _start + n;
				std::uninitialized_fill(_end, newEnd, val);
				_end = newEnd;
			}
			else {
				iterator newStart = _alloc.allocate(n);
				iterator tempEnd = std::uninitialized_copy(_start, _end, newStart);
				_clear_aux();
				_start = newStart;
				iterator newEnd = _start + n;
				std::uninitialized_fill(tempEnd, newEnd, val);
				_end = _endOfStorage = newEnd;
			}
		}
	}

	size_type capacity()const {
		return _endOfStorage - _start;
	}

	bool empty()const {
		return _end - _start == 0;
	}

	void reserve(size_type n) {
		if (n > capacity()) {
			iterator newStart = _alloc.allocate(n);
			iterator newEnd = std::uninitialized_copy(_start, _end, newStart);
			_clear_aux();
			_start = newStart;
			_end = newEnd;
			_endOfStorage = _start + n;
		}
	}

	void shrink_to_fit() {
		if (capacity() == 1)
			return;
		if (_end != _endOfStorage) {
			iterator newStart = _alloc.allocate(size());
			iterator newEnd = std::uninitialized_copy(_start, _end, newStart);
			_clear_aux();
			_start = newStart;
			_end = _endOfStorage = newEnd;
		}

	}
	
	reference operator[](size_type n) {
		if (n >= size())
			throw std::exception("");
		return *(_start + n);
	}

	const_reference operator[](size_type n)const {
		if (n >= size())
			throw std::exception("");
		return *(_start + n);
	}

	reference at(size_type n) {
		return operator[](n);
	}

	const_reference at(size_type n)const {
		return operator[](n);
	}

	reference front() {
		if (empty())
			throw std::exception("");
		return *_start;
	}

	const_reference front()const {
		if (empty())
			throw std::exception("");
		return *_start;
	}

	reference back() {
		if (empty())
			throw std::exception("");
		return *(_end - 1);
	}

	const_reference back()const {
		if (empty())
			throw std::exception("");
		return *(_end - 1);
	}

	pointer data() {
		return _start;
	}

	const_pointer data()const {
		return _start;
	}

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last) {
		_assign_aux(first, last, std::is_integral<InputIterator>::type());
	}

	void assign(size_type n, const value_type &val) {
		_clear_aux();
		if (n == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(n);
		_end = _endOfStorage = std::uninitialized_fill_n(_start, n, val);
	}

	void assign(std::initializer_list<value_type> il) {
		_clear_aux();
		if (il.size() == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(il.size());
		_end = _endOfStorage = std::uninitialized_copy(il.begin(), il.end(), _start);
	}

	void push_back(const value_type &val) {
		if (_end != _endOfStorage) {
			_alloc.construct(_end++, val);
			return;
		}
		else {
			size_type newCapacity = size() * 2;
			iterator newStart = _alloc.allocate(newCapacity);
			iterator newEnd = std::uninitialized_copy(_start, _end, newStart);
			_clear_aux();
			_start = newStart;
			_end = newEnd;
			_endOfStorage = _start + newCapacity;
			_alloc.construct(_end++, val);
		}
	}

	void pop_back() {
		if (empty())
			throw std::exception("");
		_alloc.destroy(--_end);
	}

	iterator insert(const_iterator position, const value_type &val) {
		iterator pos = _start + (position - _start);
		if (_end != _endOfStorage) {
			std::copy_backward(pos, _end, _end + 1);
			++_end;
			_alloc.construct(pos, val);
			return pos;
		}
		else if (!empty()) {
			size_type newCapacity = size() * 2;
			iterator newStart = _alloc.allocate(newCapacity);
			iterator newPos = std::uninitialized_copy(_start, pos, newStart);
			_alloc.construct(newPos, val);
			iterator newEnd = std::uninitialized_copy(pos, _end, newPos + 1);
			_clear_aux();
			_start = newStart;
			_end = newEnd;
			_endOfStorage = _start + newCapacity;
			return newPos;
		}
		throw std::exception("");
	}

	iterator insert(const_iterator position, size_type n, const value_type &val) {
		iterator pos = _start + (position - _start);
		if (_endOfStorage - _end >= n) {
			std::copy_backward(pos, _end, _end + n);
			while (n--) {
				_alloc.construct(position++, val);
			}
			return pos;
		}
		else if(!empty()){
			size_type newCapacity = size() * 2;
			while (newCapacity < size() + n)
				newCapacity *= 2;
			iterator newStart = _alloc.allocate(newCapacity);
			iterator newPos = std::uninitialized_copy(_start, pos, newStart);
			iterator newEnd = std::uninitialized_copy(pos, _end, newPos + n);
			pos = newPos;
			while (n--) {
				_alloc.construct(pos++, val);
			}
			_clear_aux();
			_start = newStart;
			_end = newEnd;
			_endOfStorage = _start + newCapacity;
			return newPos;
		}
		else if (position == begin() || position == end()) {
			_alloc.deallocate(_start, _endOfStorage - _start);
			_start = _end = _alloc.allocate(n);
			while (n--) {
				_alloc.construct(_end++, val);
			}
			_endOfStorage = _end;
			return _start;
		}
		throw std::exception("");
	}

	template<typename InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last) {
		return _insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	iterator insert(const_iterator position, value_type &&val) {
		iterator pos = _start + (position - _start);
		if (_end != _endOfStorage) {
			std::copy_backward(pos, _end, _end + 1);
			++_end;
			*pos = val;
			return pos;
		}
		else if (!empty()) {
			size_type newCapacity = size() * 2;
			iterator newStart = _alloc.allocate(newCapacity);
			iterator newPos = std::uninitialized_copy(_start, pos, newStart);
			*newPos = val;
			iterator newEnd = std::uninitialized_copy(pos, _end, newPos + 1);
			_clear_aux();
			_start = newStart;
			_end = newEnd;
			_endOfStorage = _start + newCapacity;
			return newPos;
		}
		throw std::exception("");
	}

	iterator insert(const_iterator position, std::initializer_list<value_type> il) {
		return insert(position, il.begin(), il.end());
	}

	iterator erase(const_iterator position) {
		iterator pos = _start + (position - _start);
		_alloc.destroy(pos);
		_end = std::uninitialized_copy(pos + 1, _end, pos);
		return pos;
	}

	iterator erase(const_iterator first, const_iterator last){
		iterator pos = _start + (first - _start);
		while (first != last)
			_alloc.destroy(first++);
		const_iterator constEnd = _end;
		_end = std::uninitialized_copy(last, constEnd, pos);
		return pos;
	}

	void swap(Vector &x) {
		//All iterators, references and pointers remain valid for the swapped objects.
		if (x.size() != size())
			throw std::exception("");
		if (empty())
			return;
		iterator tempStart = _alloc.allocate(capacity());
		iterator tempEnd = std::uninitialized_copy(_start, _end, tempStart);
		std::uninitialized_copy(x.begin(), x.end(), _start);
		std::uninitialized_copy(tempStart, tempEnd, x.begin());
		iterator it = tempStart;
		while (it != tempEnd)
			_alloc.destroy(it++);
		_alloc.deallocate(tempStart, capacity());
	}

	void clear() noexcept {
		resize(0);
	}

	template<typename... Args>
	iterator emplace(const_iterator position, Args&&... args) {
		value_type t(args...);
		return insert(position, t);
	}
	
	template<typename... Args>
	iterator emplace_back(Args&&... args) {
		value_type t(args...);
		return insert(end(), t);
	}

	allocator_type get_allocator()const noexcept {
		return allocator_type();
	}
private:
	void _clear_aux() {
		if (!is_trivially_destructible<T>::value) {
			pointer it = _start;
			while (it != _end)
				_alloc.destroy(it++);
		}
		_alloc.deallocate(_start, _endOfStorage - _start);
	}

	void _construction_aux(size_type n, const value_type &val, std::true_type) {
		if (n == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(n);
		_end = _endOfStorage = std::uninitialized_fill_n(_start, n, val);
	}

	template<typename InputIterator>
	void _construction_aux(InputIterator first, InputIterator last, std::false_type) {
		if (first == last) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(last - first);
		_end = _endOfStorage = std::uninitialized_copy(first, last, _start);
	};

	void _assign_aux(size_type n, const value_type &val, std::true_type) {
		_clear_aux();
		if (n == 0) {
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		_start = _alloc.allocate(n);
		_end = _endOfStorage = std::uninitialized_fill_n(_start, n, val);
	}

	template<typename InputIterator>
	void _assign_aux(InputIterator first, InputIterator last, std::false_type) {
		if (first == last) {
			_clear_aux();
			_start = _end = _alloc.allocate(1);
			_endOfStorage = _start + 1;
			return;
		}
		InputIterator firstCpy = _alloc.allocate(last - first);
		InputIterator lastCpy = std::uninitialized_copy(first, last, firstCpy);
		_clear_aux();
		_start = _alloc.allocate(lastCpy - firstCpy);
		_end = _endOfStorage = std::uninitialized_copy(firstCpy, lastCpy, _start);
	};

	iterator _insert_aux(const_iterator position, size_type n, const value_type &val, std::true_type) {
		return insert(position, (size_type)n, (const value_type&)val);
	}

	template<typename InputIterator>
	iterator _insert_aux(const_iterator position, InputIterator first, InputIterator last, std::false_type) {
		iterator pos = _start + (position - _start);
		int n = last - first;
		if (_endOfStorage - _end >= n) {
			std::copy_backward(pos, _end, _end + n);
			std::uninitialized_copy(first, last, position);
			return pos;
		}
		else if (!empty()) {
			size_type newCapacity = size() * 2;
			while (newCapacity < size() + n)
				newCapacity *= 2;
			iterator newStart = _alloc.allocate(newCapacity);
			iterator newPos = std::uninitialized_copy(_start, pos, newStart);
			std::uninitialized_copy(first, last, newPos);
			iterator newEnd = std::uninitialized_copy(pos, _end, newPos + n);
			_clear_aux();
			_start = newStart;
			_end = newEnd;
			_endOfStorage = _start + newCapacity;
			return newPos;
		}
		else if (position == begin() || position == end()) {
			_clear_aux();
			_start = _alloc.allocate(n);
			_end = _endOfStorage = std::uninitialized_copy(first, last, _start);
			return _start;
		}
		throw std::exception("");
	}
};

#endif // !VECTOR_H
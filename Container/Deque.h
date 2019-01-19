#ifndef DEQUE_H
#define DEQUE_H
#include<memory>
#include<initializer_list>
#include<type_traits>
#include<iterator>

template<typename T, typename Alloc = std::allocator<T>>
class Deque;

template<typename T>
class deque_iterator {
public:
	typedef T                               value_type;
	typedef T*                              pointer;
	typedef T**                             map_pointer;
	typedef T&                              reference;
	typedef size_t                          size_type;
	typedef ptrdiff_t                       difference_type;
	typedef std::random_access_iterator_tag iterator_category;
protected:
	pointer _first;
	pointer _last;
	pointer _cur;
	map_pointer _node;

	friend class Deque<T>;
public:
	deque_iterator() :_first(nullptr), _last(nullptr), _cur(nullptr), _node(nullptr) {};

	deque_iterator(const deque_iterator &x) :_first(x._first), _last(x._last), _cur(x._cur), _node(x._node) {};

	deque_iterator& operator=(const deque_iterator &x) {
		_first = x._first;
		_last = x._last;
		_cur = x._cur;
		_node = x._node;
		return *this;
	}

	bool operator==(const deque_iterator &x)const {
		return _cur == x._cur;
	}

	bool operator!=(const deque_iterator &x)const {
		return _cur != x._cur;
	}

	reference operator*()const {
		return *_cur;
	}

	pointer operator->()const {
		return _cur;
	}

	deque_iterator& operator++() {
		return *this += 1;
	}

	deque_iterator operator++(int) {
		deque_iterator retIt(*this);
		++*this;
		return retIt;
	}

	deque_iterator& operator--() {
		return *this += -1;
	}

	deque_iterator operator--(int) {
		deque_iterator retIt(*this);
		--*this;
		return retIt;
	}

	deque_iterator& operator+=(difference_type n) {
		difference_type offset = (_cur - _first) + n;
		if (offset >= 0 && offset < _buffer_size())
			_cur += n;
		else {
			difference_type nodeOffset = offset > 0 ? offset / difference_type(_buffer_size()) : (offset + 1) / difference_type(_buffer_size()) - 1;
			_set_node(_node + nodeOffset);
			_cur = _first + (offset - nodeOffset * difference_type(_buffer_size()));
		}
		return *this;
	}

	deque_iterator& operator-=(difference_type n) {
		return *this += -n;
	}

	deque_iterator operator+(difference_type n)const {
		deque_iterator retIt(*this);
		retIt += n;
		return retIt;
	}

	deque_iterator operator-(difference_type n)const {
		deque_iterator retIt(*this);
		retIt -= n;
		return retIt;
	}

	difference_type operator-(const deque_iterator &x)const {
		return (_node - x._node - 1) * _buffer_size() + (_cur - _first) + (x._last - x._cur);
	}
	
	template<typename T>
	friend deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n, const deque_iterator<T> &x);

	bool operator<(const deque_iterator &x)const {
		if (_node == x._node)
			return _cur < x._cur;
		else
			return _node < x._node;
	}

	bool operator>(const deque_iterator &x)const {
		if (_node == x._node)
			return _cur > x._cur;
		else
			return _node > x._node;
	}

	bool operator<=(const deque_iterator &x)const {
		if (_node == x._node)
			return _cur <= x._cur;
		else
			return _node <= x._node;
	}

	bool operator>=(const deque_iterator &x)const {
		if (_node == x._node)
			return _cur >= x._cur;
		else
			return _node >= x._node;
	}

	reference operator[](difference_type n)const {
		return *(*this + n);
	}

protected:
	void _set_node(map_pointer new_node) {
		_node = new_node;
		_first = *new_node;
		_last = _first + _buffer_size();
	}

	size_type _buffer_size()const {
		return sizeof(value_type) > 512 ? 1 : 512 / sizeof(value_type);
	}
};

template<typename T>
deque_iterator<T> operator+(typename deque_iterator<T>::difference_type n, const deque_iterator<T> &x) {
	deque_iterator<T> retIt(x);
	retIt += n;
	return retIt;
}

template<typename T>
class const_deque_iterator :public deque_iterator<T> {
public:
	friend class Deque<T>;

	typedef T                               value_type;
	typedef T*                              pointer;
	typedef const T*                        const_pointer;
	typedef T**                             map_pointer;
	typedef T&                              reference;
	typedef const T&                        const_reference;
	typedef size_t                          size_type;
	typedef ptrdiff_t                       difference_type;
	typedef std::random_access_iterator_tag iterator_category;

	const_deque_iterator() :deque_iterator<T>() {};

	const_deque_iterator(const deque_iterator<T> &x) :deque_iterator<T>(x) {};

	const_deque_iterator& operator=(const deque_iterator<value_type> &x) {
		this->_set_node(x._node);
		this->_cur = x._cur;
		return *this;
	}

	const_reference operator*()const {
		return *(this->_cur);
	}

	const_pointer operator->()const {
		return this->_cur;
	}

	const_deque_iterator& operator++() {
		return *this += 1;
	}

	const_deque_iterator operator++(int) {
		const_deque_iterator retIt(*this);
		++*this;
		return retIt;
	}

	const_deque_iterator& operator--() {
		return *this += -1;
	}

	const_deque_iterator operator--(int) {
		const_deque_iterator retIt(*this);
		--*this;
		return retIt;
	}

	const_deque_iterator& operator+=(difference_type n) {
		difference_type offset = (this->_cur - this->_first) + n;
		if (offset >= 0 && offset < this->_buffer_size())
			this->_cur += n;
		else {
			difference_type nodeOffset = offset > 0 ? offset / difference_type(this->_buffer_size()) : (offset + 1) / difference_type(this->_buffer_size()) - 1;
			this->_set_node(this->_node + nodeOffset);
			this->_cur = this->_first + (offset - nodeOffset * difference_type(this->_buffer_size()));
		}
		return *this;
	}

	const_deque_iterator& operator-=(difference_type n) {
		return *this += -n;
	}

	const_deque_iterator operator+(difference_type n)const {
		deque_iterator retIt(*this);
		retIt += n;
		return retIt;
	}

	const_deque_iterator operator-(difference_type n)const {
		deque_iterator retIt(*this);
		retIt -= n;
		return retIt;
	}

	difference_type operator-(const deque_iterator<T> &x)const {
		return deque_iterator<T>::operator-(x);
	}

	template<typename T>
	friend const_deque_iterator<T> operator+(typename const_deque_iterator<T>::difference_type n, const const_deque_iterator<T> &x);

	const reference operator[](difference_type n)const {
		return *(*this + n);
	}
};

template<typename T>
const_deque_iterator<T> operator+(typename const_deque_iterator<T>::difference_type n, const const_deque_iterator<T> &x) {
	const_deque_iterator<T> retIt(x);
	retIt += n;
	return retIt;
}

template<typename T, typename Alloc>
class Deque {
public:
	typedef Alloc                                 allocator_type;
	typedef T                                     value_type;
	typedef T*                                    pointer;
	typedef T**                                   map_pointer;
	typedef T&                                    reference;
	typedef const T&                              const_reference;
	typedef deque_iterator<T>                     iterator;
	typedef const_deque_iterator<T>               const_iterator;
	typedef size_t                                size_type;
	typedef ptrdiff_t                             difference_type;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
private:
	map_pointer _map;
	size_type _mapSize;
	iterator _start;
	iterator _end;
	allocator_type _alloc;
public:
	explicit Deque(const allocator_type &alloc = allocator_type()):_alloc(alloc) {
		_initialize_map_and_nodes(0);
	}

	explicit Deque(size_type n) {
		_construct_bynvals(n, value_type());
	}
	
	explicit Deque(size_type n, const value_type &val, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_construct_bynvals(n, val);
	}

	template<typename InputIterator>
	Deque(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type()) : _alloc(alloc) {
		_construction_aux(first, last, std::is_integral<InputIterator>::type());
	}

	Deque(const Deque &x) {
		_initialize_map_and_nodes(x.size());
		_copy(x.begin(), x.end(), _start);
	}

	Deque(const Deque &x, const allocator_type &alloc):_alloc(alloc) {
		_initialize_map_and_nodes(x.size());
		_copy(x.begin(), x.end(), _start);
	}

	Deque(std::initializer_list<value_type> il, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_initialize_map_and_nodes(il.size());
		std::uninitialized_copy(il.begin(), il.end(), _start);
	}

	~Deque() {
		_reset();
	}

	Deque& operator=(const Deque &x) {
		if (_map == x._map)
			return *this;
		_reset();
		_construction_aux(x.begin(), x.end(), std::false_type());
		return *this;
	}

	Deque& operator=(std::initializer_list<value_type> il) {
		_reset();
		_construction_aux(il.begin(), il.end(), std::false_type());
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
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin()const {
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend()const {
		return const_reverse_iterator(begin());
	}

	const_iterator cbegin()const {
		return _start;
	}

	const_iterator cend()const {
		return _end;
	}

	const_reverse_iterator crbegin()const {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crend()const {
		return const_reverse_iterator(begin());
	}

	size_type size()const {
		return _end - _start;
	}

	size_type max_size()const {
		return -1;
	}

	void resize(size_type n, value_type val = value_type()) {
		difference_type dif = n - size();
		if (dif > 0) {
			while (dif--) {
				push_back(val);
			}
		}
		else if (dif < 0) {
			dif = -dif;
			while (dif--) {
				pop_back();
			}
		}
	}

	bool empty()const {
		return _start == _end;
	}

	void shrink_to_fit() {
		//pseudo shrink_to_fit()
		for (map_pointer node = _map; node != _start._node; ++node)
			if (*node) {
				_alloc.deallocate(*node, _buffer_size());
				*node = nullptr;
			}
		for (map_pointer node = _end._node + 1; node != _map + _mapSize; ++node)
			if (*node) {
				_alloc.deallocate(*node, _buffer_size());
				*node = nullptr;
			}
	}

	reference operator[](size_type n) {
		return *(_start + n);
	}

	const_reference operator[](size_type n)const {
		return *(_start + n);
	}

	reference at(size_type n) {
		if (n >= size())
			throw std::exception("");
		return *(_start + n);
	}

	const_reference at(size_type n)const {
		if (n >= size())
			throw std::exception("");
		return *(_start + n);
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

	template <typename InputIterator>
	void assign(InputIterator first, InputIterator last) {
		_assign_aux(first, last, std::is_integral<InputIterator>::type());
	}

	void assign(size_type n, const value_type &val) {
		_reset();
		_construct_bynvals(n, val);
	}

	void assign(std::initializer_list<value_type> il) {
		_reset();
		_initialize_map_and_nodes(il.size());
		std::uninitialized_copy(il.begin(), il.end(), _start);
	}

	void push_back(const value_type &val) {
		if (_end._node == _map + _mapSize) {
			_reallocate_map(1, false);
		}
		if (*_end._node == nullptr) {
			*_end._node = _alloc.allocate(_buffer_size());
			_end._set_node(_end._node);
			_end._cur = _end._first;
		}
		_alloc.construct(_end._cur, val);
		++_end;
	}

	void push_front(const value_type &val) {
		--_start;
		if (_start._node < _map) {
			++_start;
			_reallocate_map(1, true);
			--_start;
		}
		if (*_start._node == nullptr) {
			*_start._node = _alloc.allocate(_buffer_size());
			_start._set_node(_start._node);
			_start._cur = _start._first + _buffer_size() - 1;
		}
		_alloc.construct(_start._cur, val);
	}

	void pop_back() {
		--_end;
		_alloc.destroy(_end._cur);
	}

	void pop_front() {
		_alloc.destroy(_start._cur);
		++_start;
	}

	iterator insert(const_iterator position, const value_type &val) {
		iterator pos(position);
		_move_elements(pos, 1, true);
		_alloc.construct(pos._cur, val);
		return pos;
	}

	iterator insert(const_iterator position, size_type n, const value_type &val) {
		iterator pos(position);
		_move_elements(pos, n, true);
		std::uninitialized_fill_n(pos, n, val);
		return pos;
	}

	template<typename InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last) {
		return _insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	iterator insert(const_iterator position, std::initializer_list<value_type> il) {
		return insert(position, il.begin(), il.end());
	}

	iterator erase(const_iterator position) {
		iterator pos(position);
		_alloc.destroy(pos._cur);
		if (pos - _start < _end - pos) {
			std::copy_backward(_start, pos, pos + 1);
			++_start;
			return pos + 1;
		}
		else {
			std::uninitialized_copy(pos + 1, _end, pos);
			--_end;
			return pos;
		}
	}

	iterator erase(const_iterator first, const_iterator last) {
		iterator it(first);
		while (it != last) {
			_alloc.destroy(it._cur);
			++it;
		}
		if (first - _start < _end - last) {
			std::copy_backward(_start, iterator(first), iterator(last));
			_start += last - first;
			return last;
		}
		else {
			std::uninitialized_copy(iterator(last), _end, iterator(first));
			_end -= last - first;
			return first;
		}
	}

	void swap(Deque &x) {
		map_pointer tempMap = _map;
		size_type tempSize = _mapSize;
		iterator tempStart = _start;
		iterator tempEnd = _end;
		_map = x._map;
		_mapSize = x._mapSize;
		_start = x._start;
		_end = x._end;
		x._map = tempMap;
		x._mapSize = tempSize;
		x._start = tempStart;
		x._end = tempEnd;
	}

	void clear() {
		_clear_aux();
	}

	template<typename... Args>
	iterator emplace(const_iterator position, Args&&... args) {
		value_type t(args...);
		return insert(position, t);
	}

	template <typename... Args>
	void emplace_front(Args&&... args) {
		emplace(begin(), args...);
	}

	template <typename... Args>
	void emplace_back(Args&&... args) {
		emplace(end(), args...);
	}
private:
	void _reset() {
		_clear_aux();
		size_type bufferSize = _buffer_size();
		for (map_pointer node = _map; node < _map + _mapSize; ++node)
			if (*node != nullptr)
				_alloc.deallocate(*node, bufferSize);
		delete[]_map;
	}

	void _clear_aux() {
		if (!std::is_trivially_destructible<value_type>::value) {
			iterator it = _start;
			while (it != _end) {
				_alloc.destroy(it._cur);
				++it;
			}
		}
		_end = _start;
	}

	void _reallocate_map(size_type newNodesNum, bool addToFront) {
		size_type newMapSize = _mapSize + (_mapSize > newNodesNum ? _mapSize : newNodesNum) + 2;
		map_pointer newMap = new pointer[newMapSize];
		memset(newMap, 0, sizeof(pointer) * newMapSize);
		map_pointer startNode = newMap + ((newMapSize - (_mapSize + newNodesNum)) >> 1);
		if (addToFront)
			startNode += newNodesNum;
		for (int i = 0; i < _mapSize; ++i)
			*(startNode + i) = *(_map + i);
		size_type oriSize = size();
		difference_type offsetStart = _start._node - _map;
		_start._set_node(startNode + offsetStart);
		_end = _start + oriSize;
		delete[]_map;
		_map = newMap;
		_mapSize = newMapSize;
	}
	
	size_type _buffer_size() {
		return sizeof(value_type) > 512 ? 1 : 512 / sizeof(value_type);
	}

	void _initialize_map_and_nodes(size_type n) {
		size_type nodesNum = n / _buffer_size() + 1;
		_mapSize = nodesNum + 2;
		if (_mapSize < 8)
			_mapSize = 8;
		_map = new pointer[_mapSize];
		memset(_map, 0, sizeof(pointer) * _mapSize);
		map_pointer startNode = _map + ((_mapSize - nodesNum) >> 1);
		for (int i = 0; i < nodesNum; ++i)
			*(startNode + i) = _alloc.allocate(_buffer_size());
		_start._set_node(startNode);
		_start._cur = _start._first;
		_end = _start + n;
	}

	void _construct_bynvals(size_type n, const value_type &val) {
		_initialize_map_and_nodes(n);
		std::uninitialized_fill_n(_start, n, val);
	}
	
	void _construction_aux(size_type n, const value_type &val, std::true_type) {
		_construct_bynvals(n, val);
	}

	template<typename InputIterator>
	void _construction_aux(InputIterator first, InputIterator last, std::false_type) {
		_initialize_map_and_nodes(last - first);
		_end = std::uninitialized_copy(first, last, _start);
	}

	void _assign_aux(size_type n, const value_type &val, std::true_type) {
		assign(n, val);
	}

	template<typename InputIterator>
	void _assign_aux(InputIterator first, InputIterator last, std::false_type) {
		_reset();
		_initialize_map_and_nodes(last - first);
		std::uninitialized_copy(first, last, _start);
	}

	void _move_elements(iterator pos, difference_type distance, bool backward) {
		int nodesNeed = distance / _buffer_size() + 1;
		if (backward) {
			if (_mapSize - (_end._node - _map + 1) < nodesNeed)
				_reallocate_map(nodesNeed - (_mapSize - (_end._node - _map + 1)), false);
			for (int i = 0; i <= nodesNeed; ++i)
				if (!*(_end._node + i))
					*(_end._node + i) = _alloc.allocate(_buffer_size());
			std::copy_backward(pos, _end, _end + distance);
			_end += distance;
		}
		else {
			if (_start._node - _map < nodesNeed)
				_reallocate_map(nodesNeed - (_start._node - _map), true);
			for (int i = 1; i <= nodesNeed; ++i)
				if (!*(_start._node - i))
					*(_start._node - i) = _alloc.allocate(_buffer_size());
			std::uninitialized_copy(_start, pos, _start - distance);
			_start -= distance;
		}
	}

	iterator _insert_aux(const_iterator position, size_type n, const value_type &val, std::true_type) {
		return insert(position, n, val);
	}

	template<typename InputIterator>
	iterator _insert_aux(const_iterator position, InputIterator first, InputIterator last, std::false_type) {
		difference_type posOffset = position - _start;
		_move_elements(position, last - first, true);
		iterator pos = _start + posOffset;
		std::uninitialized_copy(first, last, pos);
		return pos;
	}

	void _copy(iterator first, iterator last, iterator des) {
		for (iterator it = first; it != last; ++it) {
			std::uninitialized_copy(it._first, it._last, des);
		}
	}
};

#endif // !DEQUE_H
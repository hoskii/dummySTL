#ifndef LIST_H
#define LIST_H
#include<memory>
#include<initializer_list>
#include<iterator>
#include<type_traits>

template<typename T>
struct list_node {
	T _data;
	list_node *_next;
	list_node *_pre;

	explicit list_node(const T &data = T(), list_node *next = nullptr, list_node *pre = nullptr) :_data(data), _next(next), _pre(pre) {};
};

template<typename T, typename Alloc = std::allocator<list_node<T>>>
class List;

template<typename T>
class list_iterator {
public:
	typedef T                               value_type;
	typedef ptrdiff_t                       difference_type;
	typedef T&                              reference;
	typedef T*                              pointer;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef list_node<T>*                   link_type;
protected:
	mutable link_type _ptr;
	friend class List<T>;
public:
	list_iterator() :_ptr(nullptr) {};

	list_iterator(const link_type ptr) :_ptr(ptr) {};

	list_iterator(const list_iterator &x) :_ptr(x._ptr) {};

	list_iterator& operator=(const list_iterator &x) {
		_ptr = x._ptr;
		return *this;
	}

	bool operator==(const list_iterator &x)const {
		return _ptr == x._ptr;
	}

	bool operator!=(const list_iterator &x)const {
		return _ptr != x._ptr;
	}

	reference operator*()const {
		return _ptr->_data;
	}

	pointer operator->()const {
		return _ptr;
	}

	list_iterator& operator++() {
		_ptr = _ptr->_next;
		return *this;
	}

	list_iterator operator++(int) {
		list_iterator retIt(_ptr);
		_ptr = _ptr->_next;
		return retIt;
	}

	list_iterator& operator--() {
		_ptr = _ptr->_pre;
		return *this;
	}

	list_iterator operator--(int) {
		list_iterator retIt(_ptr);
		_ptr = _ptr->_pre;
		return retIt;
	}

	link_type Ptr()const {
		return _ptr;
	}
};

template<typename T>
class const_list_iterator :public list_iterator<T> {
	friend class List<T>;
public:
	typedef T                               value_type;
	typedef ptrdiff_t                       difference_type;
	typedef T&                              reference;
	typedef T*                              pointer;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef list_node<T>*                   link_type;
public:
	const_list_iterator() :list_iterator<T>() {};

	const_list_iterator(const link_type ptr) :list_iterator<T>(ptr) {};

	const_list_iterator(const list_iterator<T> &x) :list_iterator<T>(x) {};

	const_list_iterator& operator=(list_iterator<T> &x) {
		this->_ptr = x.Ptr();
		return *this;
	}

	bool operator==(const list_iterator<T> &x) {
		return this->_ptr == x.Ptr();
	}

	bool operator!=(const list_iterator<T> &x) {
		return this->_ptr != x.Ptr();
	}

	const reference operator*() {
		return this->_ptr->_data;
	}

	const pointer operator->() {
		return this->_ptr;
	}

	const_list_iterator& operator++() {
		this->_ptr = this->_ptr->_next;
		return *this;
	}

	const_list_iterator operator++(int) {
		const_list_iterator retIt(this->_ptr);
		this->_ptr = this->_ptr->_next;
		return retIt;
	}

	const_list_iterator& operator--() {
		this->_ptr = this->_ptr->_pre;
		return *this;
	}

	const_list_iterator operator--(int) {
		const_list_iterator retIt(this->_ptr);
		this->_ptr = this->_ptr->_pre;
		return retIt;
	}
};

template<typename T, typename Alloc>
class List {
public:
	typedef Alloc                                 allocator_type;
	typedef size_t                                size_type;
	typedef T                                     value_type;
	typedef T&                                    reference;
	typedef const T&                              const_reference;
	typedef list_node<T>*                         link_type;
	typedef T*                                    pointer;
	typedef const T*                              const_pointer;
	typedef list_iterator<T>                      iterator;
	typedef const_list_iterator<T>                const_iterator;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
private:
	link_type _blankNode;
	allocator_type _alloc;
public:
	explicit List(const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_init_blanknode();
	}

	explicit List(size_type n) {
		_init_blanknode();
		_insert_bynvals(begin(), n, value_type());
	}

	List(size_type n, const value_type &val, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_init_blanknode();
		_insert_bynvals(begin(), n, val);
	}

	template <class InputIterator>
	List(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_init_blanknode();
		_construct_aux(first, last, std::is_integral<InputIterator>::type());
	}

	List(const List &x) {
		_init_blanknode();
		_insert_byiterator(begin(), x.begin(), x.end());
	}

	List(const List &x, const allocator_type &alloc) : _alloc(alloc) {
		_init_blanknode();
		_insert_byiterator(begin(), x.begin(), x.end());
	}

	List(std::initializer_list<value_type> il, const allocator_type &alloc = allocator_type()) :_alloc(alloc) {
		_init_blanknode();
		_insert_byiterator(begin(), il.begin(), il.end());
	}

	~List() {
		clear();
		_alloc.deallocate(_blankNode, 1);
	}

	List& operator= (const List &x) {
		if (this == &x)
			return *this;
		clear();
		_insert_byiterator(begin(), x.begin(), x.end());
	}


	iterator begin() {
		return _blankNode->_next;
	}

	const_iterator begin()const {
		return _blankNode->_next;
	}

	iterator end() {
		return _blankNode;
	}

	const_iterator end()const {
		return _blankNode;
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
		return _blankNode;
	}

	const_iterator cend()const {
		return _blankNode;
	}

	const_reverse_iterator crbegin()const {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crend()const {
		return const_reverse_iterator(begin());
	}

	bool empty()const {
		return _blankNode->_next == _blankNode;
	}

	size_type size()const {
		return std::distance(begin(), end());
	}

	size_type max_size()const {
		return 357913941;
	}

	reference front() {
		if (empty())
			throw std::exception("");
		return *begin();
	}

	const_reference front()const {
		if (empty())
			throw std::exception("");
		return *begin();
	}

	reference back() {
		if (empty())
			throw std::exception("");
		return *(--end());
	}

	const_reference back()const {
		if (empty())
			throw std::exception("");
		return *(--end());
	}

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last) {
		_assign_aux(first, last, std::is_integral<InputIterator>::type());
	}

	void assign(size_type n, const value_type &val) {
		clear();
		_insert_bynvals(begin(), n, val);
	}

	void assign(std::initializer_list<value_type> il) {
		clear();
		_insert_byiterator(begin(), il.begin(), il.end());
	}

	template <typename... Args>
	void emplace_front(Args&&... args) {
		value_type val(args...);
		_insert_bynvals(begin(), 1, val);
	}

	void push_front(const value_type &val) {
		_insert_bynvals(begin(), 1, val);
	}

	void pop_front() {
		erase(begin());
	}

	template <typename... Args>
	void emplace_back(Args&&... args) {
		value_type val(args...);
		_insert_bynvals(end(), 1, val);
	}

	void push_back(const value_type &val) {
		_insert_bynvals(end(), 1, val);
	}

	void pop_back() {
		erase(_blankNode->_pre);
	}

	template <typename... Args>
	iterator emplace(const_iterator position, Args&&... args) {
		value_type val(args...);
		return _insert_bynvals(position, 1, val);
	}

	iterator insert(const_iterator position, const value_type &val) {
		return _insert_bynvals(position, 1, val);
	}

	iterator insert(const_iterator position, size_type n, const value_type &val) {
		return _insert_bynvals(position, n, val);
	}

	template <typename InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last) {
		return _insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	iterator insert(const_iterator position, std::initializer_list<value_type> il) {
		return _insert_byiterator(position, il.begin(), il.end());
	}

	iterator erase(const_iterator position) {
		link_type pos = position._ptr;
		if (pos == _blankNode)
			throw std::exception("");
		link_type ret = pos->_next;
		pos->_pre->_next = pos->_next;
		pos->_next->_pre = pos->_pre;
		_alloc.destroy(pos);
		_alloc.deallocate(pos, 1);
		return ret;
	}

	iterator erase(const_iterator first, const_iterator last) {
		link_type firstPos = first._ptr;
		link_type lastPos = last._ptr;
		firstPos->_pre->_next = lastPos;
		lastPos->_pre = firstPos->_pre;
		link_type it;
		while (firstPos != lastPos) {
			it = firstPos;
			firstPos = firstPos->_next;
			_alloc.destroy(it);
			_alloc.deallocate(it, 1);
		}
		return lastPos;
	}

	void swap(List &x) {
		link_type tempBlankNode = _blankNode;
		_blankNode = x._blankNode;
		x._blankNode = tempBlankNode;
	}

	void resize(size_type n) {
		resize(n, value_type());
	}

	void resize(size_type n, const value_type &val) {
		int s = size();
		if (n < s) {
			link_type pos = _blankNode->_next;
			while (n--)
				pos = pos->_next;
			erase(pos, end());
		}
		else if (n > s) {
			_insert_bynvals(end(), n - s, val);
		}
	}

	void clear() noexcept {
		link_type curIt = _blankNode->_next;
		link_type preIt;
		if (!std::is_trivially_destructible<value_type>::value) 
			while (curIt != _blankNode) {
				preIt = curIt;
				curIt = curIt->_next;
				_alloc.destroy(preIt);
				_alloc.deallocate(preIt, 1);
			}
		else 
			while (curIt != _blankNode) {
				preIt = curIt;
				curIt = curIt->_next;
				_alloc.deallocate(preIt, 1);
			}
		_blankNode->_next = _blankNode;
		_blankNode->_pre = _blankNode;
	}

	void splice(const_iterator position, List &x) {
		if (this == &x)
			return;
		splice(position, x, x.begin(), x.end());
	}

	void splice(const_iterator position, List &x, const_iterator i) {
		const_iterator iNext(i);
		++iNext;
		if (position == i)
			return;
		splice(position, x, i, iNext);
	}

	void splice(const_iterator position, List &x, const_iterator first, const_iterator last) {
		if (position != last) {
			int n = std::distance(first, last);
			if (n == 0)
				return;
			link_type firstPos = first._ptr, lastPos = last._ptr;
			link_type beforeLastPos = lastPos->_pre;
			firstPos->_pre->_next = lastPos;
			lastPos->_pre = firstPos->_pre;
			link_type insertedPos = position._ptr;
			firstPos->_pre = insertedPos->_pre;
			insertedPos->_pre->_next = firstPos;
			beforeLastPos->_next = insertedPos;
			insertedPos->_pre = beforeLastPos;
		}
	}

	void remove(const value_type &val) {
		remove_if([&val](const value_type &v)->bool {return val == v;});
	}

	template<typename Predicate>
	void remove_if(Predicate pred) {
		iterator it = begin();
		while (it != end()) {
			if (pred(*it))
				it = erase(it);
			else
				++it;
		}
	}

	void unique() {
		unique([](const value_type &v1, const value_type &v2)->bool {return v1 == v2; });
	}

	template <typename BinaryPredicate>
	void unique(BinaryPredicate binary_pred) {
		iterator slow = begin();
		iterator fast = slow;
		++fast;
		while (fast != end()) {
			if (binary_pred(*fast, *slow)) {
				while (fast != end() && binary_pred(*++fast, *slow));
				slow = fast = erase(++slow, fast);
			}
			else {
				++slow;
				++fast;
			}
		}
	}

	void merge(List &x) {
		merge(x, [](const value_type &v1, const value_type &v2)->bool{return v1 < v2; });
	}

	template<typename Compare>
	void merge(List &x, Compare comp) {
		if (this == &x)
			return;
		link_type newBlankNode = _alloc.allocate(1);
		newBlankNode->_next = newBlankNode;
		newBlankNode->_pre = newBlankNode;
		link_type curNode = newBlankNode;
		link_type thisIt = _blankNode->_next;
		link_type xIt = x._blankNode->_next;
		while (thisIt != _blankNode || xIt != x._blankNode) {
			if (thisIt == _blankNode) {
				curNode->_next = xIt;
				xIt->_pre = curNode;
				x._blankNode->_pre->_next = newBlankNode;
				newBlankNode->_pre = x._blankNode->_pre;
				break;
			}
			else if (xIt == x._blankNode) {
				curNode->_next = thisIt;
				thisIt->_pre = curNode;
				_blankNode->_pre->_next = newBlankNode;
				newBlankNode->_pre = _blankNode->_pre;
				break;
			}
			else if(comp(thisIt->_data,xIt->_data)){
				curNode->_next = thisIt;
				thisIt->_pre = curNode;
				thisIt = thisIt->_next;
				curNode = curNode->_next;
			}
			else {
				curNode->_next = xIt;
				xIt->_pre = curNode;
				xIt = xIt->_next;
				curNode = curNode->_next;
			}
		}
		_alloc.deallocate(_blankNode, 1);
		_blankNode = newBlankNode;
		x._blankNode->_next = x._blankNode;
		x._blankNode->_pre = x._blankNode;
	}

	void sort() {
		sort(std::less<>());
	}

	template<typename Compare>
	void sort(Compare comp) {
		_quicksort(_blankNode->_next, _blankNode, size(), comp);
	}

	void reverse() {
		link_type curNode = _blankNode;
		while (true) {
			link_type nextNode = curNode->_next;
			curNode->_next = curNode->_pre;
			curNode->_pre = nextNode;
			if (nextNode == _blankNode)
				break;
			curNode = nextNode;
		}
	}

	allocator_type get_allocator() const {
		return allocator_type();
	}
private:
	void _init_blanknode() {
		_blankNode = _alloc.allocate(1);
		_blankNode->_next = _blankNode;
		_blankNode->_pre = _blankNode;
	}

	void _construct_aux(size_type n, const value_type &val, std::true_type) {
		_insert_bynvals(begin(), n, val);
	}

	template<typename InputIterator>
	void _construct_aux(InputIterator first, InputIterator last, std::false_type) {
		_insert_byiterator(begin(), first, last);
	}

	void _assign_aux(size_type n, const value_type &val, std::true_type) {
		clear();
		_insert_bynvals(begin(), n, val);
	}

	template<typename InputIterator>
	void _assign_aux(InputIterator first, InputIterator last, std::false_type) {
		List tempList(first, last); 
		clear();
		_insert_byiterator(begin(), tempList.begin(), tempList.end());
	}

	iterator _insert_aux(const_iterator position, size_type n, const value_type &val, std::true_type) {
		return _insert_bynvals(position, n, val);
	}

	template<typename InputIterator>
	iterator _insert_aux(const_iterator position, InputIterator first, InputIterator last, std::false_type) {
		return _insert_byiterator(position, first, last);
	}

	iterator _insert_bynvals(const_iterator position,size_type n, const value_type &val) {
		if (n == 0)
			return nullptr;
		link_type begin = _alloc.allocate(1);
		_alloc.construct(begin, val);
		link_type cur = begin;
		while (--n) {
			cur ->_next= _alloc.allocate(1);
			_alloc.construct(cur->_next, val);
			cur->_next->_pre = cur;
			cur = cur->_next;
		}
		_insert_byiterator(position, iterator(begin), iterator(cur->_next));
		return begin;
	}

	template<typename InputIterator>
	iterator _insert_byiterator(const_iterator position, InputIterator first, InputIterator last) {
		link_type beforePosition = position._ptr->_pre;
		link_type frontIt;
		link_type backIt = beforePosition;
		while (first != last) {
			frontIt = _alloc.allocate(1);
			_alloc.construct(frontIt, *first++);
			backIt->_next = frontIt;
			frontIt->_pre = backIt;
			backIt = frontIt;
		}
		backIt->_next = position._ptr;
		position._ptr->_pre = backIt;
		return beforePosition->_next;
	}

	template<typename Compare>
	link_type _quicksort(link_type first, link_type last, size_type size, Compare comp) {
		if (size < 2)
			return first;
		link_type mid = _advance(first, size >> 1);
		first = _quicksort(first, mid, size >> 1, comp);
		mid = _quicksort(mid, last, size - (size >> 1), comp);
		link_type newFirst = first;
		for (bool _initial_loop = true;; _initial_loop = false) {
			if (comp(mid->_data, first->_data)) {
				if (_initial_loop)
					newFirst = mid;
				mid = mid->_next;
				splice(first, *this, mid->_pre);
				if (mid == last)
					return newFirst;
			}
			else {
				first = first->_next;
				if (first == mid)
					return newFirst;
			}
		}
	}

	link_type _advance(link_type p, size_type n) {
		while (n--)
			p = p->_next;
		return p;
	}
};

#endif // !LIST_H
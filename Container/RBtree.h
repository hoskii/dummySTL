#ifndef RBTREE_H
#define RBTREE_H
#include<memory>
#include<initializer_list>
#define RED true
#define BLACK false

template<typename Value>
class RBtree_node {
public:
	typedef bool         color_type;
	typedef RBtree_node* link_type;

	Value _value;
	color_type _color;
	link_type _parent;
	link_type _left;
	link_type _right;

	RBtree_node(const Value &val, link_type parent = nullptr, link_type left = nullptr, link_type right = nullptr, color_type color = RED) \
		:_value(val), _parent(parent), _left(left), _right(right), _color(color) {};
};

template<typename Value,typename Ref,typename Ptr>
class rb_tree_iterator {
public:
	typedef Value                           value_type;
	typedef Ref                             reference;
	typedef Ptr                             pointer;
	typedef ptrdiff_t                       difference_type;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef RBtree_node<Value>*             link_type;
protected:
	link_type _ptr;
public:
	rb_tree_iterator() :_ptr(nullptr) {};

	rb_tree_iterator(link_type p) :_ptr(p) {};

	rb_tree_iterator(const rb_tree_iterator &x) :_ptr(x._ptr) {};

	rb_tree_iterator& operator=(const rb_tree_iterator &x) {
		_ptr = x._ptr;
		return *this;
	}

	bool operator==(const rb_tree_iterator &x)const {
		return _ptr == x._ptr;
	}

	bool operator!=(const rb_tree_iterator &x)const {
		return _ptr != x._ptr;
	}

	reference operator*()const {
		return _ptr->_value;
	}

	pointer operator->()const {
		return &(_ptr->_value);
	}

	rb_tree_iterator& operator++() {
		if (_ptr->_right) {
			_ptr = _ptr->_right;
			while (_ptr->_left)
				_ptr = _ptr->_left;
		}
		else {
			link_type p = _ptr->_parent;
			while (p->_right == _ptr) {
				_ptr = p;
				p = p->_parent;
			}
			if (_ptr->_right != p)
				_ptr = p;
		}
		return *this;
	}

	rb_tree_iterator operator++(int) {
		rb_tree_iterator ret(*this);
		++*this;
		return ret;
	}

	rb_tree_iterator& operator--() {
		if (_ptr->_color == RED && _ptr->_parent->_parent == _ptr)
			_ptr = _ptr->_right;
		else if (_ptr->_left) {
			_ptr = _ptr->_left;
			while (_ptr->_right)
				_ptr = _ptr->_right;
		}
		else {
			link_type p = _ptr->_parent;
			while (p->_left == _ptr) {
				_ptr = p;
				p = p->_parent;
			}
			_ptr = p;
		}
		return *this;
	}

	rb_tree_iterator operator--(int) {
		rb_tree_iterator ret(*this);
		--*this;
		return ret;
	}

	link_type _Ptr()const {
		return _ptr;
	}
};

template<typename Value, typename Ref, typename Ptr>
class const_rb_tree_iterator :public rb_tree_iterator<Value, Ref, Ptr> {
public:
	typedef Value                             value_type;
	typedef Ref                               reference;
	typedef Ptr                               pointer;
	typedef ptrdiff_t                         difference_type;
	typedef std::bidirectional_iterator_tag   iterator_category;
	typedef RBtree_node<Value>*               link_type;
	typedef rb_tree_iterator<Value, Ref, Ptr> parent_iterator;

	const_rb_tree_iterator(const parent_iterator &x) :parent_iterator(x) {};

	const_rb_tree_iterator& operator=(const parent_iterator &x) {
		parent_iterator::operator=(x);
		return *this;
	}

	const Value& operator*()const {
		return this->_ptr->_value;
	}

	const Value* operator->()const {
		return &(this->_ptr->_value);
	}

	const_rb_tree_iterator& operator++() {
		parent_iterator::operator++();
		return *this;
	}

	const_rb_tree_iterator operator++(int) {
		const_rb_tree_iterator ret(*this);
		++*this;
		return ret;
	}

	const_rb_tree_iterator& operator--() {
		parent_iterator::operator--();
		return *this;
	}

	const_rb_tree_iterator operator--(int) {
		const_rb_tree_iterator ret(*this);
		--*this;
		return ret;
	}
};

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = std::allocator<RBtree_node<Value>>>
class RBtree {
public:
	typedef Key                                     key_type;
	typedef Value                                   value_type;
	typedef size_t                                  size_type;
	typedef RBtree_node<Value>*                     link_type;
	typedef Alloc                                   allocator_type;
	typedef Value&                                  reference;
	typedef Value*                                  pointer;
	typedef rb_tree_iterator<Value, Value&, Value*> iterator;
private:
	link_type _header;
	size_type _size;
	Compare _keyCompare;
	KeyOfValue _keyOfValue;
	allocator_type _alloc;
public:
	explicit RBtree(const Compare &compare, const allocator_type &alloc = allocator_type()) :_keyCompare(compare), _alloc(alloc), _size(0), _keyOfValue() {
		_init_header();
	};

	template<typename InputIterator>
	RBtree(InputIterator first, InputIterator last, const Compare &compare, bool repeat, const allocator_type &alloc = allocator_type()) :_keyCompare(compare), _alloc(alloc), _keyOfValue() {
		_init_header();
		if (repeat) {
			while (first != last) {
				insert_equal(*first);
				++first;
			}
		}
		else {
			while (first != last) {
				insert_unique(*first);
				++first;
			}
		}
	};

	RBtree(const RBtree &x, bool repeat) :_keyCompare(x._keyCompare), _alloc(x._alloc), _keyOfValue() {
		_init_header();
		if (repeat) {
			for (auto it = x.begin(); it != x.end(); ++it)
				insert_equal(*it);
		}
		else {
			for (auto it = x.begin(); it != x.end(); ++it)
				insert_unique(*it);
		}
	};

	RBtree(std::initializer_list<value_type> il, const Compare &compare, bool repeat, const allocator_type &alloc = allocator_type()) :_keyCompare(compare), _alloc(alloc), _keyOfValue() {
		_init_header();
		if (repeat) {
			for (auto it = il.begin(); it != il.end(); ++it)
				insert_equal(*it);
		}
		else {
			for (auto it = il.begin(); it != il.end(); ++it)
				insert_unique(*it);
		}
	};

	~RBtree() {
		clear();
		_alloc.deallocate(_header, 1);
	}

	std::pair<iterator, bool> insert_unique(const value_type &x) {
		if (_size == 0) {
			_header->_left = _header->_right = _header->_parent = _alloc.allocate(1);
			_alloc.construct(_header->_parent, x, _header, nullptr, nullptr, BLACK);
			++_size;
			return std::pair<iterator, bool>(_header->_parent, true);
		}
		else {
			std::pair<iterator, bool> ret;
			_header->_parent = _put_unique(_header->_parent, _header, x, ret);
			_header->_parent->_parent = _header;
			_adjust_header_insert(ret.first._Ptr());
			return ret;
		}
	}

	iterator insert_equal(const value_type &x) {
		if (_size == 0) {
			_header->_left = _header->_right = _header->_parent = _alloc.allocate(1);
			_alloc.construct(_header->_parent, x, _header, nullptr, nullptr, BLACK);
			++_size;
			return iterator(_header->_parent);
		}
		else {
			iterator ret;
			_header->_parent = _put_equal(_header->_parent, _header, x, ret);
			_header->_parent->_parent = _header;
			_adjust_header_insert(ret._Ptr());
			return ret;
		}
	}

	size_type size()const {
		return _size;
	}

	value_type max()const {
		return _header->_right->_value;
	}

	value_type min()const {
		return _header->_left->_value;
	}

	iterator begin()const {
		return _header->_left;
	}

	iterator end()const {
		return _header;
	}

	void assign(const RBtree &x, bool repeat) {
		clear();
		if (repeat) {
			for (auto it = x.begin(); it != x.end(); ++it)
				insert_equal(*it);
		}
		else {
			for (auto it = x.begin(); it != x.end(); ++it)
				insert_unique(*it);
		}
	}

	void assign(std::initializer_list<value_type> il, bool repeat) {
		clear();
		if (repeat) {
			for (auto it = il.begin(); it != il.end(); ++it)
				insert_equal(*it);
		}
		else {
			for (auto it = il.begin(); it != il.end(); ++it)
				insert_unique(*it);
		}
	}

	void clear() {
		if (_size == 0)
			return;
		_clear_aux(_header->_parent);
		_header->_parent = _header->_left = _header->_right = _header;
		_size = 0;
	}

	bool empty()const {
		return _size == 0;
	}

	void swap(RBtree &x) {
		link_type tmp_header = _header;
		size_type tmp_size = _size;
		_header = x._header;
		_size = x._size;
		x._header = tmp_header;
		x._size = tmp_size;
	}

	iterator find(const key_type &key)const {
		if (empty())
			return end();
		link_type h = _header->_parent;
		while (h) {
			if (_is_equal_keyval(key,h->_value))
				return h;
			else if (_do_compare_keyval(key, h->_value))
				h = h->_left;
			else
				h = h->_right;
		}
		return end();
	}

	//iterator lower_bound(const value_type &x)const {
	//	if (empty())
	//		return end();
	//	if (_do_compare(max(), x))
	//		return end();
	//	link_type h = _header->_parent;
	//	link_type res = nullptr;
	//	while (h) {
	//		if (_is_equal(x, h->_value))
	//			return h;
	//		else if (_do_compare(x, h->_value)) {
	//			if (!res)
	//				res = h;
	//			else
	//				res = _do_compare(h->_value, res->_value) ? h : res;
	//			h = h->_left;
	//		}
	//		else {
	//			h = h->_right;
	//		}
	//	}
	//	return res;
	//}

	//iterator upper_bound(const value_type &x)const {
	//	if (empty())
	//		return end();
	//	if (_do_compare(max(), x) || _is_equal(max(), x))
	//		return end();
	//	link_type h = _header->_parent;
	//	link_type res = nullptr;
	//	while (h) {
	//		if (_is_equal(x, h->_value)) {
	//			iterator it(h);
	//			++it;
	//			return it;
	//		}
	//		else if (_do_compare(x, h->_value)) {
	//			if (!res)
	//				res = h;
	//			else
	//				res = _do_compare(h->_value, res->_value) ? h : res;
	//			h = h->_left;
	//		}
	//		else {
	//			h = h->_right;
	//		}
	//	}
	//	return res;
	//}

	iterator lower_bound(const key_type &k)const {
		if (empty())
			return end();
		if (_do_compare_valkey(max(), k))
			return end();
		link_type h = _header->_parent;
		link_type res = nullptr;
		while (h) {
			if (_is_equal_keyval(k, h->_value))
				return h;
			else if (_do_compare_keyval(k, h->_value)) {
				if (!res)
					res = h;
				else
					res = _do_compare_valval(h->_value, res->_value) ? h : res;
				h = h->_left;
			}
			else {
				h = h->_right;
			}
		}
		return res;
	}

	iterator upper_bound(const key_type &k)const {
		if (empty())
			return end();
		if (_do_compare_valkey(max(), k) || _is_equal_keyval(k, max()))
			return end();
		link_type h = _header->_parent;
		link_type res = nullptr;
		while (h) {
			if (_is_equal_keyval(k, h->_value)) {
				iterator it(h);
				++it;
				return it;
			}
			else if (_do_compare_keyval(k, h->_value)) {
				if (!res)
					res = h;
				else
					res = _do_compare_valval(h->_value, res->_value) ? h : res;
				h = h->_left;
			}
			else {
				h = h->_right;
			}
		}
		return res;
	}

	void print_tree()const {
		print_tree(_header->_parent);
		std::cout << "size: " << size() << std::endl;
		std::cout << "max: " << max() << std::endl;
		std::cout << "min: " << min() << std::endl;
	}

	void print_tree(link_type root)const {
		if (!root)
			return;
		print_tree(root->_left);
		std::cout << root->_value << std::endl;
		print_tree(root->_right);
	}
private:
	void _init_header() {
		_header = _alloc.allocate(1);
		_header->_color = RED;
		_header->_parent = _header;
		_header->_left = _header;
		_header->_right = _header;
	}

	link_type _put_unique(link_type root, link_type parent, const value_type &x, std::pair<iterator, bool> &ret) {
		if (!root) {
			link_type newNode = _alloc.allocate(1);
			_alloc.construct(newNode, x, parent);
			ret.first = iterator(newNode);
			ret.second = true;
			++_size;
			return newNode;
		}
		if (!_do_compare_valval(x, root->_value) && !_do_compare_valval(root->_value, x)) {
			ret.first = root;
			ret.second = false;
			return root;
		}
		else if (_do_compare_valval(x, root->_value))
			root->_left = _put_unique(root->_left, root, x, ret);
		else
			root->_right = _put_unique(root->_right, root, x, ret);
		if (!_is_red(root->_left) && _is_red(root->_right))
			root = _rotate_left(root);
		if (_is_red(root->_left) && _is_red(root->_left->_left))
			root = _rotate_right(root);
		if (_is_red(root->_left) && _is_red(root->_right))
			_flip_colors(root);
		return root;
	}

	link_type _put_equal(link_type root, link_type parent, const value_type &x, iterator &ret) {
		if (!root) {
			link_type newNode = _alloc.allocate(1);
			_alloc.construct(newNode, x, parent);
			ret = iterator(newNode);
			++_size;
			return newNode;
		}
		if (_do_compare_valval(x, root->_value))
			root->_left = _put_equal(root->_left, root, x, ret);
		else
			root->_right = _put_equal(root->_right, root, x, ret);
		if (!_is_red(root->_left) && _is_red(root->_right))
			root = _rotate_left(root);
		if (_is_red(root->_left) && _is_red(root->_left->_left))
			root = _rotate_right(root);
		if (_is_red(root->_left) && _is_red(root->_right))
			_flip_colors(root);
		return root;
	}

	bool _is_red(link_type node)const {
		if (!node)
			return false;
		return node->_color == RED;
	}

	link_type _rotate_left(link_type h) {
		link_type tmp = h->_right;
		h->_right = tmp->_left;
		if(tmp->_left)
			tmp->_left->_parent = h;
		tmp->_left = h;
		tmp->_parent = h->_parent;
		h->_parent = tmp;
		tmp->_color = h->_color;
		h->_color = RED;
		return tmp;
	}

	link_type _rotate_right(link_type h) {
		link_type tmp = h->_left;
		h->_left = tmp->_right;
		if(tmp->_right)
			tmp->_right->_parent = h;
		tmp->_right = h;
		tmp->_parent = h->_parent;
		h->_parent = tmp;
		tmp->_color = h->_color;
		h->_color = RED;
		return tmp;
	}

	void _flip_colors(link_type h) {
		h->_color = RED;
		h->_left->_color = BLACK;
		h->_right->_color = BLACK;
	}

	void _adjust_header_insert(link_type h) {
		if (_size == 1) {
			_header->_parent = _header->_left = _header->_right = h;
			h->_parent = _header;
		}
		else {
			if (_do_compare_valval(h->_value, _header->_left->_value))
				_header->_left = h;
			else if(_do_compare_valval(_header->_right->_value,h->_value))
				_header->_right = h;
		}
		_header->_parent->_color = BLACK;
	}

	bool _do_compare_valval(const value_type &lhs, const value_type &rhs)const {
		return _keyCompare(_keyOfValue(lhs), _keyOfValue(rhs));
	}

	bool _do_compare_keyval(const key_type &key, const value_type &val)const {
		return _keyCompare(key, _keyOfValue(val));
	}

	bool _do_compare_valkey(const value_type &val, const key_type &key)const {
		return _keyCompare(_keyOfValue(val), key);
	}

	void _clear_aux(link_type h) {
		if (h->_left)
			_clear_aux(h->_left);
		if (h->_right)
			_clear_aux(h->_right);
		_alloc.deallocate(h, 1);
	}

	bool _is_equal_keyval(const key_type &key, const value_type &val)const {
		return !_keyCompare(key, _keyOfValue(val)) && !_keyCompare(_keyOfValue(val), key);
	}
};

#endif // !RBTREE_H
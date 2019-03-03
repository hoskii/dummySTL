#ifndef SET_H
#define SET_H
#include"RBtree.h"

template<typename T>
class identity {
public:
	const T& operator()(const T& x)const {
		return x;
	}
};

template<typename T,typename Compare=std::less<T>,typename Alloc = std::allocator<RBtree_node<T>>>
class Set {
public:
	typedef T                                     key_type;
	typedef T                                     value_type;
	typedef Compare                               key_compare;
	typedef Compare                               value_compare;
	typedef Alloc                                 allocator_type;
	typedef T&                                    reference;
	typedef const T&                              const_reference;
	typedef T*                                    pointer;
	typedef const T*                              const_pointer;
	typedef const_rb_tree_iterator<T, T&, T*>     iterator;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef ptrdiff_t                             difference_type;
	typedef size_t                                size_type;
	typedef RBtree<T, T, identity<T>, Compare>    tree_type;
private:
	tree_type _tree;
public:
	explicit Set(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :_tree(comp, alloc) {};

	explicit Set(const allocator_type &alloc) :_tree(key_compare(), alloc) {};

	template<typename InputIterator>
	Set(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :_tree(first, last, comp, false, alloc) {};

	Set(const Set &x) :_tree(x._tree, false) {};

	Set(std::initializer_list<value_type> il, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :_tree(il, comp, false, alloc) {};

	Set& operator=(const Set &x) {
		_tree.assign(x._tree, false);
		return *this;
	}

	Set& operator=(std::initializer_list<value_type> il) {
		_tree.assign(il, false);
		return *this;
	}

	iterator begin()const {
		return _tree.begin();
	}

	iterator end()const {
		return _tree.end();
	}

	reverse_iterator rbegin()const {
		return reverse_iterator(end());
	}

	reverse_iterator rend()const {
		return reverse_iterator(begin());
	}

	bool empty()const {
		return _tree.empty();
	}

	size_type size()const {
		return _tree.size();
	}

	size_type max_size()const {
		return -1;
	}

	std::pair<iterator, bool> insert(const value_type &val) {
		return _tree.insert_unique(val);
	}

	template<typename InputIterator>
	void insert(InputIterator first, InputIterator last) {
		while (first != last) {
			_tree.insert_unique(*first);
			++first;
		}
	}

	void insert(std::initializer_list<value_type> il) {
		auto it = il.begin();
		while (it != il.end()) {
			_tree.insert_unique(*it);
			++it;
		}
	}

	void swap(Set &x) {
		_tree.swap(x._tree);
	}

	void clear() {
		_tree.clear();
	}

	template<typename... Args>
	std::pair<iterator, bool> emplace(Args&&... args) {
		value_type val(args...);
		return _tree.insert_unique(val);
	}

	iterator find(const value_type &x)const {
		return _tree.find(x);
	}

	size_type count(const value_type &x)const {
		return _tree.find(x) != end();
	}

	iterator lower_bound(const value_type &x)const {
		return _tree.lower_bound(x);
	}

	iterator upper_bound(const value_type &x)const {
		return _tree.upper_bound(x);
	}
};

#endif // !SET_H
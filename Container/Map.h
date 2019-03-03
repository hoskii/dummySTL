#ifndef MAP_H
#define MAP_H
#include"RBtree.h"

template<typename Key,typename Value>
class select1st {
public:
	const Key& operator()(const std::pair<Key, Value> &x)const {
		return x.first;
	}
};

template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<RBtree_node<T>>>
class Map {
public:
	typedef Key                                                                     key_type;
	typedef T                                                                       mapped_type;
	typedef std::pair<const Key, T>                                                 value_type;
	typedef Compare                                                                 key_compare;
	typedef Alloc                                                                   allocator_type;
	typedef value_type&                                                             reference;
	typedef const value_type&                                                       const_reference;
	typedef value_type*                                                             pointer;
	typedef const value_type*                                                       const_pointer;
	typedef rb_tree_iterator<value_type, value_type&, value_type*>                  iterator;
	typedef const_rb_tree_iterator<value_type, value_type&, value_type*>            const_iterator;
	typedef std::reverse_iterator<iterator>                                         reverse_iterator;
	typedef std::reverse_iterator<const_iterator>                                   const_reverse_iterator;
	typedef ptrdiff_t                                                               difference_type;
	typedef size_t                                                                  size_type;
	typedef RBtree<key_type, value_type, select1st<key_type, mapped_type>, Compare> tree_type;
private:
	tree_type _tree;
public:
	explicit Map(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :_tree(comp, alloc) {};

	explicit Map(const allocator_type &alloc) :_tree(key_compare(), alloc) {};

	template<typename InputIterator>
	Map(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :_tree(first, last, comp, false, alloc) {};

	Map(const Map &x) :_tree(x._tree, false) {};

	Map(std::initializer_list<value_type> il, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) :_tree(il, comp, false, alloc) {};

	Map& operator=(const Map &x) {
		_tree.assign(x._tree, false);
		return *this;
	}

	Map& operator=(std::initializer_list<value_type> il) {
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

	const_iterator cbegin()const {
		return _tree.begin();
	}

	const_iterator cend()const {
		return _tree.end();
	}

	const_reverse_iterator crbegin()const {
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crend()const {
		return const_reverse_iterator(begin());
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

	mapped_type& operator[](const key_type &k) {
		return (*((_tree.insert_unique(std::make_pair(k, mapped_type()))).first)).second;
	}

	mapped_type& at(const key_type &k) {
		auto it = _tree.find(k);
		if (it == _tree.end())
			throw std::exception("");
		return *it.second;
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

	void swap(Map &x) {
		_tree.swap(x._tree);
	}

	void clear() {
		_tree.clear();
	}

	template<typename... Args>
	std::pair<iterator, bool> emplace(Args&&... args) {
		value_type val(args...);
		return insert(val);
	}

	iterator find(const key_type &k) {
		return _tree.find(k);
	}

	size_type count(const key_type &k)const {
		return _tree.find(k) != end();
	}

	iterator lower_bound(const key_type &k)const {
		return _tree.lower_bound(k);
	}

	iterator upper_bound(const key_type &k)const {
		return _tree.upper_bound(k);
	}
};

#endif // !MAP_H
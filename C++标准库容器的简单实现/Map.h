/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef MAP
#define MAP

#include <iostream>
#include <vector>
using namespace std;

//template struct, pair.
template <typename elem_1, typename elem_2> 
struct Pair
{
	elem_1 first;
	elem_2 second;
	Pair(const elem_1 &f = elem_1(), const elem_2 &s = elem_2()) :first(f), second(s) {}
};

//template function, make_pair.
template <typename elem_1, typename elem_2>
auto make_Pair(const elem_1 &k, const elem_2 &v) ->Pair<elem_1, elem_2>
{
	return Pair<elem_1, elem_2>(k, v);
};

//insert({a,b}) ---- insert(initializer_list<value_type>).
template <typename Key, typename Value, typename compare = less<Key>> class Map
{
public:
	typedef Key key_type;
	typedef Value mapped_type;
	typedef Pair<const Key,Value> value_type;

private:
	//node of the map tree.
	struct Node
	{
		value_type elem;
		Node *parent;
		Node *left;		//left child;
		Node *right;	//right child;
		Node(const value_type &k, Node *pr = nullptr, Node *lc = nullptr, Node *rc = nullptr)
			:elem(k), parent(pr), left(lc), right(rc) {}
	};

public:
	class const_iterator
	{
	public:
		//default constructor.
		const_iterator() = default;

		//overloaded operator *.
		const value_type& operator*()const { return current->elem; }

		//overloaded operator ->.
		const value_type* operator->()const { return &current->elem; }

		//prefix increment operator.
		const_iterator& operator++()
		{
			Node *t;
			if (current->right)
			{
				//To find the smallest element on the right subtree;
				t = current->right;
				while (t->left != nullptr)
					t = t->left;
				current = t;
			}
			else
			{
				//To find the bigger one node among the parent list.
				compare lessThan = compare();
				t = current->parent;
				while (t && lessThan(t->elem.first, current->elem.first))
					t = t->parent;
				current = t;  //t might be nullptr, to construct tail iterator.
			}
			return *this;
		}

		//suffix increment operator.
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		//comparision operators.
		bool operator==(const const_iterator &rhs)const
		{ 
			return current == rhs.current;
		}
		bool operator!=(const const_iterator &rhs)const 
		{ 
			return current != rhs.current;
		}

	protected: //friend class can use these but users cannot.
		Node *current = nullptr;
		const_iterator(Node *p) :current(p) {}
		friend class Map<Key, Value, compare>;
	};

	class iterator :public const_iterator
	{
	public:
		//default constructor.
		iterator() = default;

		//overloaded operator *.
		value_type& operator*() { return current->elem; }

		//overloaded operator *.
		const value_type& operator*()const { return const_iterator::operator*(); }

		//overloaded operator ->.
		value_type* operator->() { return &current->elem; }

		//overloaded operator ->.
		const value_type* operator->()const { return const_iterator::operator->(); }

		//prefix increment operator.
		iterator& operator++()
		{
			Node *t;
			if (current->right)
			{
				//To find the smallest element on the right subtree;
				t = current->right;
				while (t->left != nullptr)
					t = t->left;
				current = t;
			}
			else
			{
				//To find the bigger one node among the parent list.
				compare lessThan = compare();
				t = current->parent;
				while (t && lessThan(t->elem.first, current->elem.first))
					t = t->parent;
				current = t;  //t might be nullptr, to construct tail iterator.
			}
			return *this;
		}

		//suffix increment operator.
		iterator operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

	protected:
		iterator(Node *p) :const_iterator(p) {}
		friend class Map<Key, Value, compare>;
	};

public:
	//default constructor.
	Map() = default;

	//copy constructor.
	Map(const Map &rhs) { *this = rhs; }

	//move constructor.
	Map(Map &&rhs) { *this = rhs; }

	//constructor, the parameter is a list.
	Map(initializer_list<value_type> lst) { insert(lst); }

	//constructor, the parameter is a sequence.
	template <typename Iter>
	Map(Iter beg, Iter end) { insert(beg, end); }

	//destructor.
	~Map() { clear(root); }

	//copy assignment operator.
	Map& operator=(const Map &rhs)
	{
		if (this != &rhs)
		{
			theSize = rhs.theSize;
			clear(root);
			root = clone(rhs.root);	//Remember to assign the return value to root;
			setParent(root, nullptr);	//set parents for all nodes.
		}
		return *this;
	}

	//move assignment operator.
	Map& operator=(Map &&rhs)
	{
		if (this != &rhs)
		{
			theSize = rhs.theSize;
			clear(root);
			root = rhs.root;
			rhs.root = nullptr;
			rhs.theSize = 0;
		}
		return *this;
	}

	//return size of the map.
	int size()const { return theSize; }

	//check whether if the map is empty.
	bool empty()const { return theSize == 0; }

	//return different iterators.
	iterator begin() 
	{ 
		return iterator(findMin(root)); 
	}
	const_iterator begin()const 
	{ 
		return const_iterator(findMin(root));
	}
	const_iterator cbegin()
	{
		return const_iterator(findMin(root));
	}
	iterator end() 
	{ 
		return iterator(); 
	}
	const_iterator end() const 
	{ 
		return const_iterator();
	}
	const_iterator cend() const 
	{ 
		return const_iterator(); 
	}

	//return reference of the pointed element.
	Value& operator[](const Key &key)
	{
		auto ret = find(key, root);
		if (ret != end())
			return ret->second;
		else
		{
			auto tmp = insert(value_type(key, Value()), root);
			return (tmp.first)->second;
		}
	}

	//return iterator of the minimum element.
	const_iterator findMin()const
	{ 
		return const_iterator(findMin(root));
	}

	//return iterator of the maximum element.
	const_iterator findMax()const 
	{ 
		return const_iterator(findMax(root));
	}

	//return iterator of the pointed element.
	iterator find(const Key &k)const 
	{ 
		return find(k, root); 
	}

	//insert an element(pair), the parameter is a pair.
	Pair<iterator, bool> insert(const Pair<Key, Value> &v) 
	{ 
		return insert(value_type(v.first, v.second), root);
	}

	//insert an element(pair), the parameter is a value_type.
	Pair<iterator, bool> insert(const value_type &v) 
	{ 
		return insert(v, root);
	}

	//insert an element, construct it with provided args.
	Pair<iterator, bool> emplace(const Key &k, const Value &v) 
	{ 
		return insert(value_type(k, v), root);
	}

	//insert an element, searching from the pointed position.
	Pair<iterator, bool> insert(const_iterator p, const Pair<Key, Value> &k)
	{ 
		return insert(value_type(k.first, k.second), p.current);
	}

	//insert an element, construct it with provided args, searching from the pointed position.
	Pair<iterator, bool> emplace(const_iterator p, const Key &k, const Value &v) 
	{
		return insert(value_type(k, v), p.current);
	}

	//insert a list.
	void insert(initializer_list<value_type> lst)
	{
		for (const auto &elem : lst)
			insert(elem, root);
	}

	//insert a sequence.
	template <typename Iter>
	void insert(Iter beg, Iter end)
	{
		for (; beg != end; ++beg)
			insert(value_type(beg->first,beg->second), root);
	}

	//erase the pointed element, the parameter is a key.
	iterator erase(const Key &key) 
	{ 
		return erase(key, root);
	}

	//erase the pointed element, the parameter is an iterator.
	iterator erase(const_iterator p) 
	{ 
		return erase(p->first, root);
	}

	//erase the pointed sequence.
	iterator erase(const_iterator beg, const_iterator end)
	{
		Key ret = end->first;
		vector<Key> num;
		while (beg->first != ret)	//memory might change in the process of erasing elements.
			num.push_back(beg++->first);
		for (const auto &k : num)
			erase(k, root);
		return find(ret);
	}

	//clear all the elements.
	void clear() { clear(root); }

private:
	size_t theSize = 0;
	Node *root = nullptr;

	//to find the minimum element.
	Node* findMin(Node *t)const
	{
		if (t != nullptr)
			while (t->left != nullptr)
				t = t->left;
		return t;
	}

	//to find the maximum element.
	Node* findMax(Node *t)const
	{
		if (t != nullptr)
			while (t->right != nullptr)
				t = t->right;
		return t;
	}

	//to find the appointed element.
	iterator find(const Key &k, Node *t)const
	{
		compare lessThan = compare();
		if (t == nullptr)
			return iterator(t);
		if (lessThan(k, t->elem.first))
			return find(k, t->left);
		if (lessThan(t->elem.first, k))
			return find(k, t->right);
		return iterator(t);
	}

	//insert an element(pair).
	Pair<iterator, bool> insert(const value_type &v, Node* &t, Node *parent = nullptr)
	{
		//pay attention to the statement before the function body.
		compare lessThan = compare();
		if (t == nullptr)	//insert into current position.
		{
			t = new Node(v, parent); theSize++;
			return make_Pair(iterator(t), true);
		}
		if (lessThan(v.first, t->elem.first))	//insert into the left subtree.
			return insert(v, t->left, t);
		if (lessThan(t->elem.first, v.first))	//insert into the right subtree.
			return insert(v, t->right, t);
		return make_Pair(iterator(t), false);	//the given element is already in the map.
	}

	//erase the given element.
	iterator erase(const Key &k, Node* &t)
	{
		compare lessThan = compare();
		if (t == nullptr)
			return iterator(t);
		if (lessThan(k, t->elem.first))
			return erase(k, t->left);
		if (lessThan(t->elem.first, k))
			return erase(k, t->right);
		if (t->left != nullptr && t->right != nullptr)	//both left and right subtrees are not empty.
		{
			//foolish method.
			Node *tmp = findMin(t->right);
			const_cast<Key&>(t->elem.first) = tmp->elem.first;
			t->elem.second = tmp->elem.second;
			erase(t->elem.first, t->right);
			return iterator(t);
		}
		else	//current node has only one subtree, left or right.
		{
			Node *oldNode = t;
			iterator next = ++iterator(t);
			t = (t->left == nullptr) ? t->right : t->left;
			if (t != nullptr)
				t->parent = oldNode->parent;
			delete oldNode;
			theSize--;
			return next;
		}
	}

	//clear the map.
	void clear(Node* &t)
	{
		if (t != nullptr)
		{
			clear(t->left);
			clear(t->right);
			delete t;
		}
		t = nullptr;
	}

	//clone the map tree.
	Node* clone(Node* t)const
	{
		if (t != nullptr)	//set the parent as nullptr temporarily.
			return new Node(t->elem, nullptr, clone(t->left), clone(t->right));
		return nullptr;
	}

	//traverse the map tree, set parent for all nodes.
	void setParent(Node *cur, Node *pre)
	{
		if (cur != nullptr)
		{
			setParent(cur->left, cur);
			setParent(cur->right, cur);
			cur->parent = pre;
		}
	}
};

#endif
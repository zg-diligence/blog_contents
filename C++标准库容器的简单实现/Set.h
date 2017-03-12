/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef SET
#define SET

#include <iostream>
#include "Map.h"
#include <vector>
using namespace std;

//insert(iterator,key) ---- insert(first,beg).
template <typename Key, typename compare = less<Key>> class Set
{
public:
	typedef Key key_type;
	typedef Key value_type;

private:
	//node of the set tree.
	struct Node
	{
		Key data;		//key;
		Node *parent;
		Node *left;		//left child;
		Node *right;	//right child;
		Node(const Key &k, Node *pr = nullptr, Node *lc = nullptr, Node *rc = nullptr) :data(k), parent(pr), left(lc), right(rc) {}
	};

public:
	class const_iterator
	{
	public:
		//defalut constructor.
		const_iterator() = default;

		//overloaded operator *.
		const Key& operator*()const { return current->data;; }

		//prefix increment operator.
		const_iterator& operator++()
		{
			Node *t;
			if (current->right)
			{
				//to find the smallest element in the right subtree.
				t = current->right;
				while (t->left != nullptr)
					t = t->left;
				current = t;
			}
			else
			{
				//to find the bigger one node among the parent list.
				compare lessThan = compare();
				t = current->parent;
				while (t && lessThan(t->data, current->data))
					t = t->parent;
				current = t;	//t might be nullptr, to construct tail iterator.
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

	protected:	//friend class can use these but users cannot.
		Node *current = nullptr;
		const_iterator(Node *p) :current(p) {}
		friend class Set<Key, compare>;
	};

	class iterator :public const_iterator
	{
	public:
		//default constructor.
		iterator() = default;

		//overloaded operator *.

		const Key& operator*() { return current->data; }

		//overloaded operator *.
		const Key& operator*()const { return const_iterator::operator*(); }

		//prefix increment operator.
		iterator& operator++()
		{
			Node *t;
			if (current->right)
			{
				//to find the smallest element in the right subtree.
				t = current->right;
				while (t->left != nullptr)
					t = t->left;
				current = t;
			}
			else
			{
				//to find the bigger one node among the parent list.
				compare lessThan = compare();
				t = current->parent;
				while (t && lessThan(t->data, current->data))
					t = t->parent;
				current = t;	//t might be nullptr, to construct tail iterator.
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
		friend class Set<Key, compare>;
	};

public:
	//default constructor.
	Set() = default;

	//copy constructor.
	Set(const Set &rhs) { *this = rhs; }

	//move constructor.
	Set(Set &&rhs) { *this = rhs; }

	//constructor, the parameter is a list.
	Set(initializer_list<Key> lst) { insert(lst); }

	//constructor, the parameter is a sequence.
	template <typename Iter>
	Set(Iter beg, Iter end) { insert(beg, end); }

	//destructor.
	~Set() { clear(root); }

	//copy assignment operator.
	Set& operator=(const Set &rhs)
	{
		if (this != &rhs)
		{
			theSize = rhs.theSize;
			clear(root);
			root = clone(rhs.root);	//Remember to assign the return value to root;
			setParent(root, nullptr); //set parents for all nodes.
		}
		return *this;
	}

	//move assignment operator.
	Set& operator=(Set &&rhs)
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

	//return size of the set.
	int size()const { return theSize; }

	//check whether if the set is empty.
	bool isEmpty()const { return theSize == 0; }

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

	//return iterator of the minimum element.
	const_iterator& findMin()const 
	{ 
		return const_iterator(findMin(root);) 
	}

	//return iterator of the maximum element.
	const_iterator& findMax()const
	{ 
		return const_iterator(findMax(root)); 
	}

	//return iterator of the pointed element.
	iterator find(const Key& k)const 
	{ 
		return find(k, root); 
	}

	//insert an element.
	Pair<iterator, bool> insert(const Key &k) 
	{ 
		return insert(k, root); 
	}

	//insert an element.
	Pair<iterator, bool> insert(const_iterator &p, const Key &k)
	{ 
		return insert(k, p.current);
	}

	//insert a list.
	void insert(initializer_list<Key> lst)
	{
		for (const auto &elem : lst)
			insert(elem, root);
	}

	//insert a sequence.
	template <typename Iter>
	void insert(Iter beg, Iter end)
	{
		for (; beg != end; ++beg)
			insert(*beg, root);
	}

	//erase the pointed element, the parameter is a key.
	iterator erase(const Key &k) 
	{ 
		return erase(k, root);
	}

	//erase the pointed element, the parameter is an iterator.
	iterator erase(const_iterator p)
	{ 
		return erase(*p, root); 
	}

	//erase a sequence.
	iterator erase(const_iterator beg, const_iterator end)
	{
		Key ret = *end;
		vector<Key> num;
		while (*beg != ret)
			num.push_back(*beg++);
		for (const auto &k : num)
			erase(k, root);
		return find(ret);
	}

	//clear all elements.
	void clear() { clear(root); }

private:
	int theSize = 0;
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
		if (lessThan(k, t->data))
			return find(k, t->left);
		if (lessThan(t->data, k))
			return find(k, t->right);
		return iterator(t);
	}

	//insert an element(pair).
	Pair<iterator, bool> insert(const Key &k, Node* &t, Node *parent = nullptr)
	{
		//pay attention to the statement before the function body.
		compare lessThan = compare();
		if (t == nullptr)	//insert into current position.
		{
			t = new Node(k, parent); theSize++;
			return make_Pair(iterator(t), true);
		}
		if (lessThan(k, t->data))		//insert into the left subtree.
			return insert(k, t->left, t);
		if (lessThan(t->data, k))		//insert into the right subtree.
			return insert(k, t->right, t);
		return make_Pair(iterator(t), false);	//the given element is already in the set.
	}

	//erase the given element.
	iterator erase(const Key &k, Node* &t)
	{
		compare lessThan = compare();
		if (t == nullptr)
			return iterator(t);
		if (lessThan(k, t->data))
			return erase(k, t->left);
		if (lessThan(t->data, k))
			return erase(k, t->right);
		if (t->left != nullptr && t->right != nullptr)	//both left and right subtrees are not empty.
		{
			//foolish method.
			Node *tmp = findMin(t->right);
			t->data = tmp->data;
			erase(t->data, t->right);
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

	//clear the set.
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

	//clone the set tree.
	Node* clone(Node *t)const
	{
		if (t != nullptr)	//set parent ad nullptr temporarily.
			return new Node(t->data, nullptr, clone(t->left), clone(t->right));
		return nullptr;
	}

	//traverse the set tree, set parent for all nodes.
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
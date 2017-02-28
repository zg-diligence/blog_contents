#ifndef MAP
#define MAP

#include <iostream>
using namespace std;

//template class, Pair of map.
template <typename elem_1, typename elem_2>
struct Pair
{
	elem_1 first;
	elem_2 second;
	Pair(const elem_1 &f = elem_1(), const elem_2 &s = elem_2()) :first(f), second(s) {}
};

//template function, point out the return type after the function boby.
template <typename elem_1, typename elem_2>
auto make_Pair(const elem_1 &k, const elem_2 &v) ->Pair<elem_1, elem_2>
{
	return Pair<elem_1, elem_2>(k, v);
};

//template class, implement map with bintree.
template <typename Key, typename Value, typename compare = less<Key>> class Map
{
public:
	typedef Key key_type;
	typedef Value mapped_type;
	typedef Pair<Key,Value> value_type;

private:
	//node of the map tree.
	struct Node
	{
		value_type elem;
		Node *parent;
		Node *left;		//left child;
		Node *right;	//right child;
		Node(const value_type &obj, Node *pr = nullptr, Node *lc = nullptr, Node *rc = nullptr)
			:elem(obj), parent(pr), left(lc), right(rc) {}
	};

public:
	class const_iterator
	{
	public:
		//overloaded operator *, return const reference of the element.
		const value_type& operator*()const { return current->elem; }

		//overloaded operator ->, return const pointer of the element.
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
		bool operator==(const const_iterator &rhs)const { return current == rhs.current; }
		bool operator!=(const const_iterator &rhs)const { return current != rhs.current; }

	protected: //friend class can use these but users cannot.
		Node *current = nullptr;
		const_iterator(Node *ptr = nullptr) :current(ptr) {}
		friend class Map<Key, Value, compare>;
	};

	class iterator :public const_iterator
	{
	public:
		//overloaded operator *, return reference of the element.
		value_type& operator*() { return current->elem; }

		//overloaded operator *, return const reference of the element.
		//call function of parent class.
		const value_type& operator*()const { return const_iterator::operator*(); }

		//overloaded operator ->, return pointer of the element.
		value_type* operator->() { return &current->elem; }

		//overloaded operator ->,return const pointer of the element.
		//call function of parent class.
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
		iterator(Node *p = nullptr) :const_iterator(p) {}
		friend class Map<Key, Value, compare>;
	};

public:
	//default constructor.
	Map() = default;

	//constructor, the parameter is another map.
	Map(const Map &rhs) :root(nullptr), theSize(0) { *this = rhs; }

	//constructor, the parameter is a list.
	Map(initializer_list<value_type> il) :root(nullptr), theSize(0)
	{
		for (const auto &obj : il)
			insert(obj, root);
	}

	//destructor.
	~Map() { clear(root); }

	//copy assignment operator.
	Map& operator=(const Map &rhs);

	//move assignment operator.
	Map& operator=(Map &&rhs);

	//return size of the map.
	int size()const { return theSize; }

	//check whether if the map is empty.
	bool empty()const { return theSize == 0; }

	//to find the given element, return iterator of it.
	iterator find(const Key &key)const { return find(key, root); }

	//to find the minimum element.
	const_iterator findMin()const { return const_iterator(findMin(root)); }

	//to find the maximum element.
	const_iterator findMax()const { return const_iterator(findMax(root)); }

	//return appointed iterators.
	iterator begin() { return iterator(findMin(root)); }
	const_iterator begin()const { return const_iterator(findMin(root)); }
	iterator end() { return iterator(); }
	const_iterator end()const { return const_iterator(); }

	//vistited appointed element.
	//if there is no mathched element, create an element with the given key and an initial value. 
	Value& operator[](const Key &key);

	//insert an element(pair), the function provided for users to call.
	Pair<iterator, bool> insert(const value_type &obj) { return insert(obj, root); }

	//insert an element, the parameters are provided for constructing the new element.
	Pair<iterator, bool> emplace(const Key &k, const Value &v) { return insert(value_type(k, v), root); }

	//insert an element after given position.
	Pair<iterator, bool> insert(const_iterator p, const value_type &obj) { return insert(obj, p.current); }

	//insert an element after given position and the other two parameters are provided for constructing the new element.
	Pair<iterator, bool> emplace(const_iterator p, const Key &k, const Value &v) { return insert(value_type(k, v), p.current); }

	//insert a sequence limited by two iterators.
	//Note that this function is template function beacuase of the uncertainty of parameters' type.
	template <typename first, typename last>
	bool insert(first beg, last end)
	{
		bool flag = false;
		for (; beg != end; ++beg)
			if (insert(*beg, root).second)
				flag = true;
		return flag;
	}

	//erase the given element, the function provided for users to call.
	bool erase(const Key &key) { return erase(key, root); }
	bool erase(const_iterator p) { return erase(p->first, root); }

	//clear the map, provided for users to call.
	void clear() { clear(root); }

private:
	int theSize = 0;
	Node *root = nullptr;

	//to find the minimum element, the function actually do this.
	Node* findMin(Node *t)const;

	//to find the maximum element, the function actually do this.
	Node* findMax(Node *t)const;

	//to find the appointed element, the function actually do this.
	iterator find(const Key &key, Node *t)const;

	//insert an element(pair), the function actually do this.
	Pair<iterator, bool> insert(const value_type &obj, Node* &t, Node *parent = nullptr);

	//erase the given element, the function actually do this.
	bool erase(const Key &key, Node* &t);

	//clear the map.
	void clear(Node* &t);

	//clone the map tree.
	Node* clone(Node* t)const;

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

template <typename Key, typename Value, typename compare>
Map<Key, Value, compare>& Map<Key, Value, compare>::operator=(const Map &rhs)
{
	if (this != &rhs)
	{
		theSize = rhs.theSize;
		clear(root);
		root = clone(rhs.root);	//Remember to assign the return value to root;
		setParent(root,nullptr);//set parents for all nodes.
	}
	return *this;
}

template <typename Key, typename Value, typename compare>
Map<Key, Value, compare>& Map<Key, Value, compare>::operator=(Map &&rhs)
{
	theSize = rhs.theSize;
	clear(root);
	root = rhs.root;
	rhs.root = nullptr;
	rhs.theSize = 0;
}

template <typename Key, typename Value, typename compare>
typename Map<Key, Value, compare>::Node* Map<Key, Value, compare>::findMin(Node *t)const
{
	if (t != nullptr)
		while (t->left != nullptr)
			t = t->left;
	return t;
}

template <typename Key, typename Value, typename compare>
typename Map<Key, Value, compare>::Node* Map<Key, Value, compare>::findMax(Node *t)const
{
	if (t != nullptr)
		while (t->right != nullptr)
			t = t->right;
	return t;
}

template <typename Key, typename Value, typename compare>
typename Map<Key, Value, compare>::iterator Map<Key, Value, compare>::find(const Key &key, Node *t)const
{
	compare lessThan = compare();
	if (t == nullptr)
		return iterator(t);
	if (lessThan(key, t->elem.first))
		return find(key, t->left);
	if (lessThan(t->elem.first, key))
		return find(key, t->right);
	return iterator(t);
}

template <typename Key, typename Value, typename compare>
Value& Map<Key, Value, compare>::operator[](const Key &key)
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

template <typename Key, typename Value, typename compare>
Pair<typename Map<Key, Value, compare>::iterator, bool> Map<Key, Value, compare>::insert(const value_type &obj, Node* &t, Node *parent)
{
	//pay attention to the statement before the function body.
	compare lessThan = compare();
	if (t == nullptr)	//insert into current position.
	{
		t = new Node(obj, parent);theSize++;
		return make_Pair(iterator(t), true);
	}
	if (lessThan(obj.first, t->elem.first))	//insert into the left subtree.
		return insert(obj, t->left, t);
	if (lessThan(t->elem.first, obj.first))	//insert into the right subtree.
		return insert(obj, t->right, t);
	return make_Pair(iterator(t), false);	//the given element is already in the map.
}

template <typename Key, typename Value, typename compare>
bool Map<Key, Value, compare>::erase(const Key &key, Node* &t)
{
	compare lessThan = compare();
	if (t == nullptr)
		return false;
	else if (lessThan(key, t->elem.first))
		return erase(key, t->left);
	else if (lessThan(t->elem.first, key))
		return erase(key, t->right);
	else if (t->left != nullptr && t->right != nullptr)
	{
		//foolish method.
		Node *temp = findMin(t->right);
		t->elem.first = temp->elem.first;
		t->elem.second = temp->elem.second;
		return erase(t->elem.first, t->right);
	}
	else
	{
		Node *oldNode = t;
		t = (t->left == nullptr) ? t->right : t->left;
		if (t != nullptr)
			t->parent = oldNode->parent;
		delete oldNode;
		theSize--;
		return true;
	}
	return false;
}

template <typename Key, typename Value, typename compare>
void Map<Key, Value, compare>::clear(Node* &t)
{
	if (t != nullptr)
	{
		clear(t->left);
		clear(t->right);
		delete t;
	}
	t = nullptr;
}

template <typename Key, typename Value, typename compare>
typename Map<Key, Value, compare>::Node* Map<Key, Value, compare>::clone(Node* t)const
{
	if (t != nullptr)	//set the parent as nullptr temporarily.
		return new Node(t->elem, nullptr, clone(t->left), clone(t->right));
	return nullptr;
}

#endif
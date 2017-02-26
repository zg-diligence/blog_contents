#ifndef MAP
#define MAP

#include <iostream>
#include <functional>
using namespace std;

template <typename Key,typename Value,typename compare=less<Key>> class Map
{
public:
	typedef pair<Key, Value> value_type;

private:
	struct Node
	{
		value_type keyValue;
		Node *parent;
		Node *left;
		Node *right;
		Node(const value_type &obj, Node *p = nullptr, Node *l = nullptr, Node *r = nullptr) :keyValue(obj), parent(p), left(l), right(r){}
	};

public:
	class const_iterator
	{
	public:
		const_iterator() :current(nullptr) {}

		const value_type& operator*()const { return current->keyValue; }
		const value_type* operator->()const { return &(*(*this)); }
		
		const_iterator& operator++()
		{
			Node *t;
			if (current->right)//To find the smallest node on the right;
			{
				t = current->right;
				while (t->left != nullptr)
					t = t->left;
				current = t;
			}
			else//To find the bigger node among the parent nodes;
			{
				compare lessThan = compare();
				t = current->parent;
				while (t && lessThan(t->keyValue.first,current->keyValue.first))
					t = t->parent;
				current = t;
			}
			return *this;
		}
		const_iterator operator++(int)
		{
			Node *old = current;
			++(*this);
			return old;
		}

		bool operator==(const const_iterator &rhs)const { return current == rhs.current; }
		bool operator!=(const const_iterator &rhs)const { return !(*this == rhs); }

	protected:
		Node *current;
		const_iterator(Node *ptr) :current(ptr) {}
		friend class Map<Key,Value,compare>;
	};

	class iterator :public const_iterator
	{
	public:
		iterator(){}

		value_type& operator*() { return current->keyValue; }
		const value_type& operator*()const { return const_iterator::operator*(); }
		value_type* operator->(){ return &(*(*this)); }
		const value_type* operator->()const { return const_iterator::operator->(); }

		iterator& operator++()
		{
			Node *t;
			if (current->right)
			{
				t = current->right;
				while (t->left != nullptr)
					t = t->left;
				current = t;
			}
			else
			{
				compare lessThan = compare();
				t = current->parent;
				while (t && lessThan(t->keyValue.first,current->keyValue.first))
					t = t->parent;
				current = t;
			}
			return *this;
		}
		iterator operator++(int)
		{
			Node *old = current;
			++(*this);
			return old;
		}

	protected:
		iterator(Node *p) :const_iterator(p) {}
		friend class Map<Key,Value,compare>;
	};

public:
	Map() :root(nullptr), theSize(0) {}
	Map(const Map &rhs) :root(nullptr), theSize(0) {*this = rhs;}
	Map(initializer_list<value_type> il) :root(nullptr), theSize(0)
	{
		for (const auto &obj : il)
			insert(obj,root);
	}
	~Map() { clear(root); }
	Map& operator=(const Map &rhs);

	int size()const { return theSize; }
	bool isEmpty()const { return theSize == 0; }

	iterator find(const Key &key)const { return find(key, root); }
	const_iterator findMin()const { return const_iterator(findMin(root)); }
	const_iterator findMax()const { return const_iterator(findMax(root)); }
	iterator begin() { return iterator(findMin(root)); }
	const_iterator begin()const{ return const_iterator(findMin(root)); }
	iterator end() { return iterator(); }
	const_iterator end()const { return const_iterator(); }

	Value& operator[](const Key &key);
	pair<iterator, bool> insert(const value_type &obj) { return insert(obj, root); }
	pair<iterator, bool> insert(const value_type &obj, const iterator &itr) { return insert(obj, itr.current); }
	bool erase(const Key &key) { return erase(key, root); }
	void clear() { clear(root); }

private:
	int theSize;
	Node *root;

	Node* findMin(Node *t)const;
	Node* findMax(Node *t)const;
	iterator find(const Key &key, Node *t)const;

	pair<iterator, bool> insert(const value_type &obj, Node* &t, Node *parent = nullptr);
	bool erase(const Key &key, Node* &t);
	void clear(Node* &t);

	void setParent(Node *current, Node *parent);
	Node* clone(Node* t)const;
};

template <typename Key,typename Value,typename compare> 
Map<Key, Value, compare>& Map<Key,Value,compare>::operator=(const Map &rhs)
{
	if (this != &rhs)
	{
		theSize = rhs.theSize;
		clear(root);
		root = clone(rhs.root);//Remember to assign the return value to root;
		setParent(root, nullptr);//The function clone cannot set the parent node;
	}
	return *this;
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
	else if (lessThan(key, t->keyValue.first))
		return find(key, t->left);
	else if (lessThan(t->keyValue.first, key))
		return find(key, t->right);
	else
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
		auto retTemp = insert(value_type(key, Value()), root);
		return (retTemp.first)->second;
	}	
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
pair<typename Map<Key, Value, compare>::iterator, bool> Map<Key, Value, compare>::insert(const value_type &obj, Node* &t,Node *parent)
{
	compare lessThan = compare();
	if (t == nullptr)
	{
		t = new Node(obj, parent);
		theSize++;
	}
	else if (lessThan(obj.first, t->keyValue.first))
		return insert(obj, t->left,t);
	else if (lessThan(t->keyValue.first,obj.first))
		return insert(obj, t->right,t);
	else
		return make_pair(iterator(t), false);

	return make_pair(iterator(t), true);
}

template <typename Key, typename Value, typename compare> 
bool Map<Key, Value, compare>::erase(const Key &key, Node* &t)
{
	compare lessThan = compare();
	if (t == nullptr)
		return false;
	else if(lessThan(key, t->keyValue.first))
		return erase(key, t->left);
	else if (lessThan(t->keyValue.first,key))
		return erase(key, t->right);
	else if (t->left != nullptr && t->right != nullptr)
	{
		//Not do theSize-- here!
		Node *temp = findMin(t->right);
		t->keyValue.first = temp->keyValue.first;
		t->keyValue.second = temp->keyValue.second;
		return erase(t->keyValue.first, t->right);
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
typename Map<Key, Value, compare>::Node* Map<Key, Value, compare>::clone(Node* t)const
{
	if (t != nullptr)
		return new Node(t->keyValue, nullptr, clone(t->left), clone(t->right));//Note:The sub node cannot get his parent node by recursion,so set it's parent node as nullptr;
	return nullptr;
}

template <typename Key, typename Value, typename compare>  void Map<Key, Value, compare>::setParent(Node *current, Node *parent)
{
	if (current != nullptr)
	{
		current->parent = parent;
		setParent(current->left, current);
		setParent(current->right, current);
	}
}

#endif
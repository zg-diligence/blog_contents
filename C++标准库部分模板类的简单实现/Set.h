#ifndef SET
#define SET

#include <iostream>
using namespace std;

template <typename object> class Set
{
private:
	struct Node
	{
		object data;
		Node *parent;
		Node *left;
		Node *right;
		Node(const object &obj = object(), Node *p = nullptr, Node *l = nullptr, Node *r = nullptr) :data(obj), parent(p), left(l), right(r) {}
	};

public:
	class const_iterator
	{
	public:
		const_iterator():current(nullptr){}

		const object& operator*()const { return current->data;; }
		
		const_iterator& operator++()
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
				t = current->parent;
				while (t && t->data < current->data)
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
		const_iterator(Node *ptr):current(ptr){}
		friend class Set<object>;
	};

	class iterator :public const_iterator
	{
	public:
		object& operator*() { return current->data; }
		const object& operator*()const { return const_iterator::operator*(); }

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
				t = current->parent;
				while (t && t->data < current->data)
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
		iterator(Node *p):const_iterator(p){}
		friend class Set<object>;
	};

public:
	Set() :root(nullptr),theSize(0) {}
	Set(const Set &rhs) :root(nullptr), theSize(0) { *this = rhs; }//It's necessary to assign nullptr to root in order to finish the initialization;
	Set(initializer_list<object> il) :root(nullptr), theSize(il.size())
	{
		for (const auto &obj : il)
			insert(obj);
	}
	~Set() { clear(root); }
	Set& operator=(const Set &rhs);

	const object& findMin()const
	{
		Node *ret = findMin(root);
		if (ret != nullptr)
			return ret->data;
		else
			return object();
	}
	const object& findMax()const
	{
		Node *ret = findMax(root);
		if (ret != nullptr)
			return ret->data;
		else
			return object();
	}
	int size()const { return theSize; }
	bool isEmpty()const { return theSize==0; }
	bool find(const object& obj)const { return find(obj, root); }
	void printTree()const { printTree(root); }

	void clear() { clear(root); }
	pair<iterator, bool> insert(const object &obj) { return insert(obj, root); }
	pair<iterator,bool> insert(const object &obj,const iterator &itr) { return insert(obj,itr->current); }
	bool erase(const object &obj) { return erase(obj, root); }

private:
	int theSize;
	Node *root;

	Node* findMin(Node *t)const;
	Node* findMax(Node *t)const;
	bool find(const object &obj, Node *t)const;
	void printTree(Node *t, int depth = 0)const;

	void clear(Node* &t);
	pair<iterator, bool> insert(const object &obj, Node* &t, Node *parent=nullptr);
	bool erase(const object &obj, Node* &t);
	Node* clone(Node *t)const;
};

template <typename object> Set<object>& Set<object>::operator=(const Set &rhs)
{
	if (this != &rhs)
	{
		theSize = rhs.theSize;
		clear(root);
		root = clone(rhs.root);//Remember to assign the return value to root;
	}
	return *this;
}

template <typename object> typename Set<object>::Node* Set<object>::findMin(Node *t)const
{
	if (t != nullptr)
		while (t->left != nullptr)
			t = t->left;
	return t;
}

template <typename object> typename Set<object>::Node* Set<object>::findMax(Node *t)const
{
	if (t != nullptr)
		while (t->right != nullptr)
			t = t->right;
	return t;
}

template <typename object> bool Set<object>::find(const object &obj, Node *t)const
{
	if (t == nullptr)
		return false;
	else if (obj < t->data)
		return find(obj, t->left);
	else if (obj > t->data)
		return find(obj, t->right);
	else
		return true;
}

template <typename object> void Set<object>::printTree(Node *t, int depth = 0)const
{
	if (t == nullptr)
		return;

	//control the print type by indent;
	int temp = depth;
	while (temp--)
		cout << '\t';
	cout << t->data << endl;

	printTree(t->left, depth + 1);
	printTree(t->right, depth + 1);
}

template <typename object> void Set<object>::clear(Node* &t)
{
	if (t != nullptr)
	{
		clear(t->left);
		clear(t->right);
		delete t;
	}
	t = nullptr;
}

template <typename object> pair<typename Set<object>::iterator, bool> Set<object>::insert(const object &obj, Node* &t,Node *parent)
{
	if (t == nullptr)
	{
		t = new Node(obj, parent);
		theSize++;
	}
	else if (obj < t->data)
		return insert(obj, t->left, t);
	else if (obj > t->data)
		return insert(obj, t->right,t);
	else
		return make_pair(iterator(t), false);

	return make_pair(iterator(t), true);
}

template <typename object> bool Set<object>::erase(const object &obj, Node* &t)
{
	if (t == nullptr)
		return false;

	if (obj < t->data)
		return erase(obj, t->left);
	else if (obj > t->data)
		return erase(obj, t->right);
	else if (t->left != nullptr && t->right != nullptr)
	{
		//Not do theSize-- here!
		Node *temp = findMin(t->right);
		t->data = temp->data;
		return erase(t->data, t->right);
	}
	else
	{
		Node *oldNode = t;
		t = (t->left == nullptr) ? t->right : t->left;
		if (t!=nullptr)
			t->parent = oldNode->parent;
		delete oldNode;
		theSize--;
		return true;
	}

	return false;
}

template <typename object> typename Set<object>::Node* Set<object>::clone(Node *t)const
{
	if (t != nullptr)
		return new Node(t->data, t->parent,clone(t->left), clone(t->right));
	return nullptr;
}

#endif
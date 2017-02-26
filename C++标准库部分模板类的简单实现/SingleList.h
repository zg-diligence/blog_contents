#ifndef SINGLELIST
#define SINGLELIST

#include <iostream>
using namespace std;

template<typename Object> class SingleList
{
	struct Node
	{
		//Node of the single linked list;
		Object data; //Data of current node;
		Node *next;  //Address of successor node;
		Node(const Object &obj = Object(), Node *n = nullptr) :data(obj), next(n) {}
	};
public:
	class const_iterator
	{
		//Const iterator pointing to node of the list,defined as local class;
	public:
		const_iterator() :current(nullptr), theList(nullptr) {}
		const_iterator(const const_iterator &rhs) :current(rhs.current), theList(rhs.theList) {}

		//Overloaded operators;
		const Object& operator*()const
		{
			outOfBounds(*this);
			return current->data;
		}
		const_iterator& operator++()
		{
			outOfBounds(*this);
			current = current->next;
			return *this;
		}
		const_iterator operator++(int)const
		{
			const_iterator old = *this;
			++(*this);
			return old;
		}
		const_iterator operator+(int n)const
		{
			const_iterator temp = *this;
			while (n--)
			{
				outOfBounds(temp);
				temp.current = temp.current->next;
			}
			return temp;
		}
		bool operator==(const const_iterator &rhs)const { return theList == rhs.theList && current == rhs.current; }
		bool operator!=(const const_iterator &rhs)const { return !(*this == rhs); }
	protected:
		Node *current;                      //Address of current node;
		const SingleList<Object> *theList;  //Address of the list that current node belongs to;

											//Constructor provided for designer of class to use;
		const_iterator(Node *p, const SingleList<Object> &lst) :current(p), theList(&lst) {}
		void outOfBounds(const const_iterator &iter)const
		{   //Check whether if current position is out of bounds;
			if (iter.current == theList->tail) { cout << "Iterator out of bounds exception!" << endl; exit(1); }
		}
		friend class SingleList<Object>;
	};
	class iterator :public const_iterator
	{
	public:
		iterator() {}
		iterator(const iterator &rhs) :const_iterator(rhs) {}

		Object& operator*()
		{
			outOfBounds(*this);
			return this->current->data;
		}
		const Object& operator*()const { return const_iterator::operator*(); }
		iterator& operator++()
		{
			this->outOfBounds(*this);
			this->current = this->current->next;
			return *this;
		}
		iterator operator++(int)const
		{
			iterator old = *this;
			++(*this);
			return old;
		}
		iterator operator+(int n)const
		{
			iterator temp = *this;
			while (n--)
			{
				this->outOfBounds(temp);
				temp.current = temp.current->next;
			}
			return temp;
		}
	protected:
		iterator(Node *p, const SingleList<Object> &lst) :const_iterator(p, lst) {}
		friend class SingleList<Object>;
	};

	SingleList() { init(); }
	SingleList(initializer_list<Object> il)
	{
		init();
		auto iter = before_begin();
		for (const auto &n : il)
			iter = insert_after(iter, n);
	}
	SingleList(const SingleList &rhs) { init(); *this = rhs; }
	SingleList(SingleList &&rhs) noexcept { init(); *this = rhs; }
	~SingleList()
	{
		makeEmpty();
		delete head;
		delete tail;
	}
	SingleList& operator=(const SingleList &rhs)
	{
		if (this != &rhs)
		{
			makeEmpty();
			auto iter = before_begin();
			for (auto _beg = rhs.cbegin(), _end = rhs.cend(); _beg != _end; ++_beg)
				iter = insert_after(iter, *_beg);
		}
		return *this;
	}
	SingleList& operator=(SingleList &&rhs)noexcept
	{
		if (this != &rhs)
		{
			makeEmpty();
			auto iter = before_begin();
			for (const auto &node : rhs)
				iter = insert_after(iter, std::move(node));
		}
		return *this;
	}

	//Return (const)iterator of sentry nodes or head node;
	iterator before_begin() { return iterator(head, *this); }
	const_iterator cbefore_begin()const { return const_iterator(head, *this); }
	iterator begin() { return iterator(head->next, *this); }
	const_iterator cbegin()const { return const_iterator(head->next, *this); }
	iterator end() { return iterator(tail, *this); }
	const_iterator cend()const { return const_iterator(tail, *this); }

	//Return information of current list;
	size_t size()const { return theSize; }
	bool empty()const { return theSize == 0; }
	Object& front()
	{   //Return reference of current node's data;
		isEmpty();
		return *begin();
	}
	const Object& front()const
	{   //Return const reference of current node's data;
		isEmpty();
		return *cbegin();
	}

	//Insert and erase operation;
	void push_front(const Object &obj)
	{   //Insert a node before the first node;
		insert_after(before_begin(), obj);
	}
	void pop_front()
	{   //Erase the first node;
		erase_after(before_begin());
	}
	iterator insert_after(const const_iterator &iter, const Object &obj)
	{   //Insert a node before the node iter points to,return iterator of inserted node;
		isValid(iter);
		theSize++;
		Node *p = iter.current;
		return iterator(p->next = new Node(obj, p->next), *this);
	}
	iterator erase_after(const const_iterator &iter)
	{   //Erase the node after the node that iter points to,return iterator of the node after the erased node;
		isEmpty(); isValid(iter); isTail(iter);
		Node *previous = iter.current;
		Node *current = (iter + 1).current;
		iterator retVal = iterator(current->next, *this);
		previous->next = current->next;
		delete current;
		theSize--;
		return retVal;
	}
	iterator erase(const const_iterator &beg, const const_iterator &end)
	{   //Erase nodes between beg and end,not including nodes that the iterator beg and end point to,return end;
		iterator iter;
		while (iter != end)
			iter = erase_after(beg);
		return iter;
	}
	void makeEmpty()
	{   //Make the list empty;
		while (!empty())
			pop_front();
	}

	//Check safety of current process;
	void isValid(const const_iterator &iter)const
	{   //If the given iterator points to a valid node in the list,exit the process;
		if (iter.theList != this || iter.current == nullptr) {
			cout << "Iterator out of bounds exception!" << endl; exit(1);
		}
	}
	void isTail(const const_iterator &iter)const
	{   //If the given iterator points to sentry node after the last node,exit the process;
		if (iter.current->next == tail) {
			cout << "Current node is the last node!" << endl; exit(1);
		}
	}
	void isEmpty()const
	{   //If current list is empty,exit the process;
		if (empty()) { cout << "Current list is empty!" << endl; exit(1); }
	}
private:
	int theSize;
	Node *head;
	Node *tail;

	//Init data members of the class object;
	void init()
	{
		theSize = 0;
		head = new Node;
		tail = new Node;
		head->next = tail;
	}
};

#endif
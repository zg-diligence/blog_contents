/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef FORWARD_LIST
#define FORWARD_LIST

#include <iostream>
using namespace std;

template <typename object> class Forward_List
{
	//node of the forward_list.
	struct Node
	{
		object data;
		Node *next;
		Node(const object &obj = object(), Node *n = nullptr) :data(obj), next(n) {}
	};

public:
	class const_iterator
	{
	public:
		//default constructor.
		const_iterator() = default;

		//overloaded operator *.
		const object& operator*()const
		{ 
			if (elem == lst->tail)
			{
				cout << "out of bounds." << endl;
				exit(1);
			}
			return elem->data;
		}

		//increment operators.
		const_iterator& operator++()
		{
			elem = elem->next;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator old = *this;
			++(*this);
			return old;
		}

		//comparision operators.
		bool operator==(const const_iterator &rhs)const
		{
			if (lst != rhs.lst)
			{
				cout << "Iterator is imcompatible!" << endl;
				exit(1);
			}
			return elem == rhs.elem;
		}
		bool operator!=(const const_iterator &rhs)const 
		{ 
			return !(*this == rhs); 
		}

	protected:
		Node *elem = nullptr;
		const Forward_List<object> *lst = nullptr;

		//constructor, using protect to prohibit users to call it.
		const_iterator(Node *p, const Forward_List<object> &lst) :elem(p), lst(&lst) {}
		friend class Forward_List<object>;
	};

	class iterator :public const_iterator
	{
	public:
		//default constructor.
		iterator() = default;

		//overloaded operator *.
		const object& operator*()const 
		{ 
			if (elem == lst->tail)
			{
				cout << "out of bounds." << endl;
				exit(1);
			}
			return elem->data;
		}
		object& operator*()
		{ 
			if (elem == lst->tail)
			{
				cout << "out of bounds." << endl;
				exit(1);
			}
			return elem->data;
		}

		//increment operators.
		iterator& operator++()
		{
			elem = elem->next;
			return *this;
		}
		iterator operator++(int)
		{
			iterator old = *this;
			++(*this);
			return old;
		}

	protected:
		//constructor, using protect to prohibit users to call it.
		iterator(Node *p, const Forward_List<object> &lst) :const_iterator(p, lst) {}
		friend class Forward_List<object>;
	};

public:
	//default constructor.
	Forward_List():theSize(0),head(nullptr),tail(nullptr){ init(); }

	//constructor, init the forward_list with cnt alike elements.
	Forward_List(int cnt, const object &obj = object())
	{
		init();
		insert_after(before_begin(), cnt, obj);
	}

	//constructor, the parameter is a lst.
	Forward_List(initializer_list<object> lst)
	{
		init();
		insert_after(before_begin(), lst);
	}

	//constructor, the parameter is a sequence.
	template <typename Iter>
	Forward_List(Iter beg, Iter end)
	{
		init();
		insert_after(before_begin(), beg, end);
	}

	//copy constructor.
	Forward_List(const Forward_List &rhs) 
	{ 
		init(); 
		*this = rhs;
	}

	//move constructor.
	Forward_List(Forward_List &&rhs)
	{
		init(); 
		*this = std::move(rhs);
	}

	//destructor.
	~Forward_List()
	{
		clear();
		delete head;
		delete tail;
	}

	//copy assignment operator.
	Forward_List& operator=(const Forward_List &rhs)
	{
		if (this != &rhs)
		{
			clear();
			insert_after(before_begin(), rhs.begin(), rhs.end());
		}
		return *this;
	}

	//move assignment operator.
	Forward_List& operator=(Forward_List &&rhs)
	{
		if (this != &rhs)
		{
			clear();
			if (rhs.theSize != 0)
			{
				head = rhs.head;
				tail = rhs.tail;
				theSize = rhs.theSize;
				rhs.init();
			}
		}
		return *this;
	}

	//copy assignment operator, the parameter is a list.
	Forward_List& operator=(initializer_list<object> lst)
	{
		clear();
		insert_after(before_begin(), lst.begin(), lst.end());
		return *this;
	}

	//swap elements of two lists.
	void swap(Forward_List<object> &rhs)
	{
		size_t cnt = theSize;
		theSize = rhs.theSize;
		rhs.theSize = cnt;

		Node *tmp = head;
		head = rhs.head;
		rhs.head = tmp;

		tmp = tail;
		tail = rhs.tail;
		rhs.tail = tmp;
	}

	//replace elements of the forward_list with cnt alike elements.
	void assign(int cnt, const object &obj)
	{
		clear();
		insert_after(before_begin(), cnt, obj);
	}

	//replace elements of the forward_list with the list.
	void assign(initializer_list<object> lst)
	{
		clear();
		insert_after(before_begin(), lst);
	}

	//repalce elements of the forward_list with the sequence.
	template <typename Iter>
	void assign(Iter beg, Iter end)
	{
		clear();
		insert_after(before_begin(), beg, end);
	}

	//return different iterators.
	iterator before_begin()
	{
		return iterator(head, *this);
	}
	const_iterator before_begin()const
	{
		return const_iterator(head, *this);
	}
	const_iterator cbefore_begin()
	{
		return const_iterator(head, *this);
	}
	iterator begin()
	{
		return iterator(head->next, *this);
	}
	const_iterator begin()const
	{
		return const_iterator(head->next, *this);
	}
	const_iterator cbegin()
	{
		return const_iterator(head->next, *this);
	}
	iterator end()
	{
		return iterator(tail, *this);
	}
	const_iterator end()const
	{
		return const_iterator(tail, *this);
	}
	const_iterator cend()
	{
		return const_iterator(tail, *this);
	}

	//check whether if the forward_list is empty.
	bool empty()const { return theSize == 0; }

	//return size of the forward_list.
	size_t size()const { return theSize; }

	//return (const) reference of the first element.
	object& front()
	{
		checkEmpty();
		return head->next->data;
	}
	const object& front()const
	{
		checkEmpty();
		return head->next->data;
	}

	//insert an element at beginning of the forward_list.
	void push_front(const object &obj)
	{
		insert_after(before_begin(), obj);
	}

	//append new element at beginning of the forward_list, construct it with provided args.
	template <typename...Args>
	void emplace_front(Args&& ...args)
	{
		push_front(object(args...));
	}

	//erase the first element of the forward_list.
	void pop_front()
	{
		checkEmpty();
		erase_after(before_begin());
	}
	
	//insert an element after the pointed position.
	iterator insert_after(const_iterator iter, const object &obj)
	{
		checkIterator(iter);
		theSize++;
		Node *p = iter.elem;
		return iterator(p->next = new Node(obj, p->next), *this);
	}

	//insert an element after the pointed position, construct it with provided args.
	template <typename...Args>
	iterator emplace_after(const_iterator iter, Args...args)
	{
		return insert(iter, object(args...));
	}

	//insert cnt alike elements after the pointed position.
	iterator insert_after(const_iterator iter, int cnt, const object &obj)
	{
		checkIterator(iter);
		theSize += cnt;
		Node *p = iter.elem;
		while (cnt--)
		{
			p->next = new Node(obj, p->next);
			p = p->next;
		}
		return iterator(iter.elem->next, *this);
	}

	//insert a lst after the pointed position.
	iterator insert_after(const_iterator iter, initializer_list<object> lst)
	{
		return insert_after(iter, lst.begin(), lst.end());
	}

	//insert a sequence after the pointed position.
	template <typename Iter>
	iterator insert_after(const_iterator iter, Iter beg, Iter end)
	{
		checkIterator(iter);
		Node *p = iter.elem;
		while (beg != end)
		{
			p->next = new Node(*beg++, p->next);
			p = p->next;
			theSize++;
		}
		return iterator(iter.elem->next, *this);
	}

	//erase the pointed element.
	iterator erase_after(const const_iterator &iter)
	{
		checkEmpty();
		checkIterator(iter);
		Node *p = iter.elem;
		if (p == tail || p->next == tail)
		{
			cout << "Iterator is imcompatible." << endl;
			exit(1);
		}

		Node *pre = iter.elem, *cur = pre->next;
		pre->next = cur->next;
		delete cur;
		theSize--;
		return iterator(pre->next, *this);
	}

	//erase the pointed sequence.
	iterator erase_after(const_iterator &beg, const const_iterator &end)
	{
		checkEmpty();
		checkIterator(beg);
		checkIterator(end);

		auto first = beg.elem, last = end.elem;
		first->next = last;

		//erase the sequence.
		Node *tmp = nullptr;
		first = first->next;
		while (first != last)
		{
			tmp = first;
			first = first->next;
			delete tmp;
			theSize--;
		}
		return iterator(last, *this);
	}

	//clear all elements.
	void clear()
	{
		Node *iter = head->next, *tmp = nullptr;
		while (iter != tail)
		{
			tmp = iter;
			iter = iter->next;
			delete tmp;
		}
		head->next = tail;
		theSize = 0;
	}

	//comparasion operators.
	bool operator==(const Forward_List<object> &rhs)const
	{
		if (theSize != rhs.theSize)
			return false;
		Node *lc = head->next, *rc = rhs.head->next;
		while (lc != tail)
		{
			if (lc->data != rc->data)
				return false;
			lc = lc->next;
			rc = rc->next;
		}
		
		return true;
	}
	bool operator!=(const Forward_List<object> &rhs)const
	{
		return !(*this == rhs);
	}
	bool operator<(const Forward_List<object> &rhs)const
	{
		Node *lc = head->next, *rc = rhs.head->next;
		while (lc != tail && rc != rhs.tail)
		{
			if (lc->data < rc->data)
				return true;
			if (lc->data > rc->data)
				return false;
			lc = lc->next;
			rc = rc->next;
		}
		if (lc == tail && rc != rhs.tail)
			return true;
		return false;
	}
	bool operator<=(const Forward_List<object> &rhs)const
	{
		return !(*this > rhs);
	}
	bool operator>(const Forward_List<object> &rhs)const
	{
		Node *lc = head->next, *rc = rhs.head->next;
		while (lc != tail && rc != rhs.tail)
		{
			if (lc->data > rc->data)
				return true;
			if (lc->data < rc->data)
				return false;
			lc = lc->next;
			rc = rc->next;
		}
		if (lc != tail && rc == rhs.tail)
			return true;
		return false;
	}
	bool operator>=(const Forward_List<object> &rhs)const
	{
		return !(*this < rhs);
	}

private:
	int theSize = 0;
	Node *head = nullptr;
	Node *tail = nullptr;

	//init variables of class.
	void init()
	{
		head = new Node;
		tail = new Node;
		head->next = tail;
	}

	//check whether if the iterator is valid.
	void checkIterator(const_iterator iter)const
	{
		if (iter.lst != this)
		{
			cerr << "Iterator is imcompatible." << endl;
			exit(1);
		}
	}

	//check whether if the forward_list is empty.
	void checkEmpty()
	{
		if (theSize == 0)
		{
			cout << "The forward_list is empty." << endl;
			exit(1);
		}
	}

	template <typename object> 
	friend void Swap(Forward_List<object> &lhs, Forward_List<object> &rhs);
};

template <typename object>
void Swap(Forward_List<object> &lhs, Forward_List<object> &rhs)
{
	size_t cnt = lhs.theSize;
	lhs.theSize = rhs.theSize;
	rhs.theSize = cnt;

	Forward_List<object>::Node *tmp = lhs.head;
	lhs.head = rhs.head;
	rhs.head = tmp;

	tmp = lhs.tail;
	lhs.tail = rhs.tail;
	rhs.tail = tmp;
}

#endif
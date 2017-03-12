/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef LIST
#define LIST

#include <iostream>
using namespace std;

template <typename object> class List
{
	//node of the list.
	struct Node
	{
		object data;
		Node *pre;
		Node *next;
		Node(const object &d = object(), Node *p = nullptr, Node *n = nullptr) :data(d), pre(p), next(n) {}
	};

public:
	class const_iterator
	{
	public:
		//overloaded operator *.
		const object& operator*()const
		{ 
			if (elem == lst->head || elem == lst->tail)
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
		const_iterator& operator--()
		{
			elem = elem->pre;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator old = *this;
			--(*this);
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
		const List<object> *lst = nullptr;

		//constructor, using protect to prohibit users to call it.
		const_iterator(Node *p, const List<object> &lst) :elem(p), lst(&lst) {}
		friend class List<object>;
	};

	class iterator :public const_iterator
	{
	public:
		//overloaded operator *.
		object& operator*() 
		{ 
			if (elem == lst->head || elem == lst->tail)
			{
				cout << "out of bounds." << endl;
				exit(1);
			}
			return elem->data; 
		}
		const object& operator*()const
		{ 
			return const_iterator::operator*();
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
		iterator& operator--()
		{
			elem = elem->pre;
			return *this;
		}
		iterator operator--(int)
		{
			iterator old = *this;
			--(*this);
			return old;
		}

	protected:
		//constructor, using protect to prohibit users to call this.
		iterator(Node *p, const List<object> &lst) :const_iterator(p, lst) {}
		friend class List<object>;
	};

	class const_reverse_iterator: public const_iterator
	{
	public:
		//overloaded operator *.
		const object& operator*()const 
		{ 
			if (elem == lst->head || elem == lst->tail)
			{
				cout << "out of bounds." << endl;
				exit(1);
			}
			return elem->data; 
		}

		//increment operators.
		const_reverse_iterator& operator++()
		{
			elem = elem->pre;
			return *this;
		}
		const_reverse_iterator operator++(int)
		{
			const_reverse_iterator old = *this;
			++(*this);
			return old;
		}
		const_reverse_iterator& operator--()
		{
			elem = elem->next;
			return *this;
		}
		const_reverse_iterator operator--(int)
		{
			const_reverse_iterator old = *this;
			--(*this);
			return old;
		}

	protected:
		//constructor, using protect to prohibit users to call this.
		const_reverse_iterator(Node *p, const List<object> &lst) :const_iterator(p, lst) {}
		friend class List<object>;
	};

	class reverse_iterator :public const_reverse_iterator
	{
	public:
		//default constructor.
		reverse_iterator() = default;

		//overloaded operator *.
		object& operator*() 
		{ 
			if (elem == lst->head || elem == lst->tail)
			{
				cout << "out of bounds." << endl;
				exit(1);
			}
			return elem->data;
		}
		const object& operator*()const 
		{ 
			return const_reverse_iterator::operator*();
		}

		//increment operators.
		reverse_iterator& operator++()
		{
			elem = elem->pre;
			return *this;
		}
		reverse_iterator operator++(int)
		{
			reverse_iterator old = *this;
			++(*this);
			return old;
		}
		reverse_iterator& operator--()
		{
			elem = elem->next;
			return *this;
		}
		reverse_iterator operator--(int)
		{
			reverse_iterator old = *this;
			--(*this);
			return old;
		}

	protected:
		//constructor, using protect to prohibit users to call this.
		reverse_iterator(Node *p, const List<object> &lst) :const_reverse_iterator(p, lst) {}
		friend class List<object>;
	};

public:
	//default constructor.
	List() :theSize(0), head(nullptr), tail(nullptr) { init(); }

	//constructor, init the list with cnt alike elements.
	List(int cnt, const object &obj = object())
	{
		init();
		insert(begin(), cnt, obj);
	}

	//constructor, the parameter is a list.
	List(initializer_list<object> il)
	{
		init();
		insert(begin(), il);
	}

	//constructor, the parameter is a sequence.
	template <typename Iter>
	List(Iter beg, Iter end)
	{
		init();
		insert(begin(), beg, end);
	}

	//copy constructor.
	List(const List &rhs)
	{
		init();
		*this = rhs;
	}

	//move constructor.
	List(List &&rhs)
	{
		init();
		*this = std::move(rhs);
	}

	//destructor.
	~List()
	{
		clear();
		delete head;
		delete tail;
	}

	//copy assignment operator.
	List& operator=(const List &rhs)
	{
		if (this != &rhs)
		{
			clear();
			insert(end(), rhs.begin(), rhs.end());
		}
		return *this;
	}

	//move assignment operator.
	List& operator=(List &&rhs)
	{
		if (this != &rhs)
		{
			clear();
			if (rhs.theSize != 0)
			{
				head->next = rhs.head->next;
				head->next->pre = head;
				tail->pre = rhs.tail->pre;
				tail->pre->next = tail;
				theSize = rhs.theSize;

				rhs.head->next = rhs.tail;
				rhs.tail->pre = rhs.head;
				rhs.theSize = 0;
			}
		}
		return *this;
	}

	//copy assignment, the parameter is a list.
	List& operator=(initializer_list<object> lst)
	{
		clear();
		insert(end(), lst.begin(), lst.end());
		return *this;
	}

	//swap elements of two lists.
	void swap(List<object> &rhs)
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

	//replace elements of the list with cnt alike new elements.
	void assign(int cnt, const object &obj)
	{
		clear();
		insert(begin(), cnt, obj);
	}

	//replace elements of the list with a list.
	void assign(initializer_list<object> lst)
	{
		clear();
		insert(begin(), lst);
	}

	//repalce elements of the list with a sequence.
	template <typename Iter>
	void assign(Iter beg, Iter end)
	{
		clear();
		insert(begin(), beg, end);
	}

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

	//return (const) reference of the last element.
	object& back() 
	{ 
		checkEmpty();
		return tail->pre->data;
	}
	const object& back()const 
	{ 
		checkEmpty();
		return tail->pre->data;
	}

	//check whether if the list is empty.
	bool empty()const { return theSize == 0; }

	//return size of the list.
	size_t size()const { return theSize; }

	//return different iterators.
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

	reverse_iterator rbegin()
	{
		return reverse_iterator(tail->pre, *this);
	}
	const_reverse_iterator rbegin()const
	{
		return const_reverse_iterator(tail->pre, *this);
	}
	const_reverse_iterator crbegin()
	{
		return const_reverse_iterator(tail->pre, *this);
	}
	reverse_iterator rend()
	{ 
		return reverse_iterator(head, *this); 
	}
	const_reverse_iterator rend()const 
	{ 
		return const_reverse_iterator(head, *this); 
	}
	const_reverse_iterator crend()
	{
		return const_reverse_iterator(head, *this);
	}

	//insert an element at beginning of the list.
	void push_front(const object &obj) { insert(begin(), obj); }

	//insert an element at beginning of the list, construct it with provided args.
	template <typename...Args>
	void emplace_front(Args&& ...args)
	{
		push_front(object(args...));
	}

	//append an element at end of the list.
	void push_back(const object &obj) { insert(end(), obj); }

	//append new element at end of the list, construct it with provided args.
	template <typename...Args>
	void emplace_back(Args&& ...args)
	{
		push_back(object(args...));
	}

	//erase the first element.
	void pop_front()
	{ 
		checkEmpty();
		erase(begin()); 
	}

	//erase the last element.
	void pop_back() 
	{
		checkEmpty();
		erase(--end()); 
	}

	//insert an element before the pointed position.
	iterator insert(const_iterator iter, const object &obj)
	{
		checkIterator(iter);
		Node *p = iter.elem;
		theSize++;
		return iterator(p->pre = p->pre->next = new Node(obj, p->pre, p), *this);
	}

	//insert an element before the pointed position, contruct it with provided args.
	template <typename...Args>
	iterator emplace(const_iterator iter, Args...args)
	{
		return insert(iter, object(args...));
	}

	//insert cnt aliked elements.
	iterator insert(const_iterator iter, int cnt, const object &obj)
	{
		checkIterator(iter);
		iterator ret = iterator(iter.elem->pre, *this);
		Node *rear = ret.elem;
		theSize += cnt;
		while (cnt--)
			rear = rear->next = new Node(obj, rear, nullptr);
		rear->next = iter.elem;
		iter.elem->pre = rear;
		return ++ret;
	}

	//insert a list before the pointed position.
	iterator insert(const_iterator iter, initializer_list<object> lst)
	{
		return insert(iter, lst.begin(), lst.end());
	}

	//insert a sequence before the pointed position.
	template <typename Iter>
	iterator insert(const_iterator iter, Iter beg, Iter end)
	{
		checkIterator(iter);
		iterator ret = iterator(iter.elem->pre, *this);
		Node *rear = ret.elem;
		while (beg != end)
		{
			theSize++;
			rear = rear->next = new Node(*beg++, rear, nullptr);
		}
		rear->next = iter.elem;
		iter.elem->pre = rear;
		return ++ret;
	}

	//erase the pointed element.
	iterator erase(const_iterator iter)
	{
		checkEmpty();
		checkIterator(iter);
		if (iter.elem == tail)
		{
			cout << "out of bounds." << endl;
			exit(1);
		}

		//erase the element.
		Node *p = iter.elem;
		iterator retVal(p->next, *this);
		p->pre->next = p->next;
		p->next->pre = p->pre;
		delete p;
		theSize--;
		return retVal;
	}

	//erase the pointed sequence.
	iterator erase(const_iterator beg, const_iterator end)
	{
		checkEmpty();
		checkIterator(beg);
		checkIterator(end);

		auto first = beg.elem, last = end.elem;
		first->pre->next = last;
		last->pre = first->pre;

		//erase the sequence.
		Node *tmp = nullptr;
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
		tail->pre = head;
		theSize = 0;
	}

	//comparasion operators.
	bool operator==(const List<object> &rhs)const
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
	bool operator!=(const List<object> &rhs)const
	{
		return !(*this == rhs);
	}
	bool operator<(const List<object> &rhs)const
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
	bool operator<=(const List<object> &rhs)const
	{
		return !(*this > rhs);
	}
	bool operator>(const List<object> &rhs)const
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
	bool operator>=(const List<object> &rhs)const
	{
		return !(*this < rhs);
	}

private:
	size_t theSize = 0;
	Node *head = nullptr;
	Node *tail = nullptr;
	
	//init variables of the list.
	void init()
	{
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->pre = head;
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

	//check whether if the list is empty.
	void checkEmpty()
	{
		if (theSize == 0)
		{
			cout << "The list is empty." << endl;
			exit(1);
		}
	}

	template <typename object>
	friend void Swap(List<object> &lhs, List<object> &rhs);
};

//swap elements of two lists.
template <typename object>
void Swap(List<object> &lhs, List<object> &rhs)
{
	size_t cnt = lhs.theSize;
	lhs.theSize = rhs.theSize;
	rhs.theSize = cnt;

	List<object>::Node *tmp = lhs.head;
	lhs.head = rhs.head;
	rhs.head = tmp;

	tmp = lhs.tail;
	lhs.tail = rhs.tail;
	rhs.tail = tmp;
}

#endif // LIST
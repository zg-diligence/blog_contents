#ifndef LIST
#define LIST

#include <iostream>
#include <initializer_list>
using namespace std;

template <typename object> class List
{
private:
	struct Node
	{
		object data;
		Node *prev;
		Node *next;
		Node(const object &d=object(),Node *p=nullptr,Node *n=nullptr):data(d),prev(p),next(n){}
	};

public:
	class const_iterator
	{
	public:
		const_iterator():current(nullptr),theList(nullptr){}
 		const_iterator(const const_iterator &rhs) :current(rhs.current), theList(rhs.theList) {}
		//move constructor is not correct,how to revise it ?!
		//const_iterator(const_iterator &&rhs) :current(std::move(rhs.current)), theList(std::move(rhs.theList)){rhs.current = nullptr;rhs.theList = nullptr;}

		const object& operator*()const { return current->data; }
		
		//Note: suffix version cannot return a value instead of reference;
		const_iterator& operator++()
		{
			current = current->next;
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
			current = current->prev;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator old = *this;
			--(*this);
			return old;
		}
		const_iterator operator+(int n)
		{
			const_iterator temp=*this;
			while (n--)
				temp.current = temp.current->next;
			return temp;
		}
		const_iterator operator-(int n)
		{
			const_iterator temp=*this;
			while (n--)
				temp.current = temp.current->prev;
			return temp;
		}

		bool operator==(const const_iterator &rhs)const { return theList == rhs.theList && current == rhs.current; }
		bool operator!=(const const_iterator &rhs)const { return !(*this == rhs); }

	protected:
		Node *current;
		const List<object> *theList;//Pointing to the list that the node belongs to;
		const_iterator(Node *p,const List<object> &lst):current(p),theList(&lst){}

		//Check whether if the present iterator is valid;
		void AssertIsValid()const
		{
			if (theList == nullptr || current == nullptr || current == theList->head)
			{
				cerr << "Iterator out of bounds exception!" << endl;
				exit(1);
			}	
		}
		friend class List<object>;
	};

	class iterator:public const_iterator
	{
	public:
		iterator(){}
 		iterator(const iterator &rhs) :const_iterator(rhs) {}
		//move constructor is not correct,how to revise it ?!
		//iterator(iterator &&rhs) :const_iterator(std::move(rhs)){}

		object& operator*() { return current->data; }
		const object& operator*()const { return const_iterator::operator*(); }//declare this function explicitly in case that it is hided by the revising version in the front;

		//Note: suffix version cannot return a value instead of reference;
		iterator& operator++()
		{
			current = current->next;
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
			current = current->prev;
			return *this;
		}
		iterator operator--(int)
		{
			iterator old = *this;
			--(*this);
			return old;
		}
		iterator operator+(int n)
		{
			iterator temp=*this;
			while (n--)
				temp.current = temp.current->next;
			return temp;
		}
		iterator operator-(int n)
		{
			iterator temp=*this;
			while (n--)
				temp.current = temp.current->prev;
			return temp;
		}

	protected:
		iterator(Node *p, const List<object> &lst):const_iterator(p, lst){}
		friend class List<object>;
	};

	class const_reverse_iterator
	{
	public:
		const_reverse_iterator() :current(nullptr), theList(nullptr) {}
		const_reverse_iterator(const const_reverse_iterator &rhs) :current(rhs.current), theList(rhs.theList) {}

		const object& operator*()const { return current->data; }

		const_reverse_iterator& operator++()
		{
			current = current->prev;
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
			current = current->next;
			return *this;
		}
		const_reverse_iterator operator--(int)
		{
			const_reverse_iterator old = *this;
			--(*this);
			return old;
		}
		const_reverse_iterator operator+(int n)
		{
			const_reverse_iterator temp = *this;
			while (n--)
				temp.current = temp.current->prev;
			return temp;
		}
		const_reverse_iterator operator-(int n)
		{
			const_reverse_iterator temp = *this;
			while (n--)
				temp.current = temp.current->next;
			return temp;
		}

		bool operator==(const const_reverse_iterator &rhs)const { return theList == rhs.theList && current == rhs.current; }
		bool operator!=(const const_reverse_iterator &rhs)const { return !(*this == rhs); }

	protected:
		Node *current;
		const List<object> *theList;//Pointing to the list that the node belongs to;
		const_reverse_iterator(Node *p, const List<object> &lst) :current(p), theList(&lst) {}

		//Check whether if the present iterator is valid;
		void AssertIsValid()const
		{
			if (theList == nullptr || current == nullptr || current == theList->head)
			{
				cerr << "Iterator out of bounds exception!" << endl;
				exit(1);
			}
		}
		friend class List<object>;
	};

	class reverse_iterator :public const_reverse_iterator
	{
	public:
		reverse_iterator() {}
		reverse_iterator(const reverse_iterator &rhs) :const_reverse_iterator(rhs) {}

		object& operator*() { return current->data; }
		const object& operator*()const { return const_reverse_iterator::operator*(); }

		reverse_iterator& operator++()
		{
			current = current->prev;
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
			current = current->next;
			return *this;
		}
		reverse_iterator operator--(int)
		{
			reverse_iterator old = *this;
			--(*this);
			return old;
		}
		reverse_iterator operator+(int n)
		{
			reverse_iterator temp = *this;
			while (n--)
				temp.current = temp.current->prev;
			return temp;
		}
		reverse_iterator operator-(int n)
		{
			reverse_iterator temp = *this;
			while (n--)
				temp.current = temp.current->next;
			return temp;
		}

	protected:
		reverse_iterator(Node *p, const List<object> &lst) :const_reverse_iterator(p, lst) {}
		friend class List<object>;
	};

public:
	List() { init(); }
	List(initializer_list<object> il)
	{
		init();
		for (const auto &obj : il)
			push_back(obj);
	}
	List(const List &rhs)
	{
		init();
		*this = rhs;
	}
	List(List &&rhs)
	{
		init();
		*this = rhs;
	}
	~List()
	{
		//clear all data nodes and delete sentry nodes;
		clear();
		delete head;
		delete tail;
	}

	List& operator=(const List &rhs)
	{
		if (this != &rhs)
		{
			clear();
			for (const_iterator beg = rhs.begin(), end = rhs.end(); beg != end; ++beg)
				push_back(*beg);
		}

		return *this;
	}
	List& operator=(List &&rhs)
	{
		if (this != &rhs)
		{
			clear();
			for (const_iterator beg = rhs.begin(), end = rhs.end(); beg != end; ++beg)
				push_back(std::move(*beg));//How to set the Node that beg point to initial state ?!
		}

		return *this;
	}

	iterator begin() { return iterator(head->next,*this); }
	const_iterator begin()const { return const_iterator(head->next,*this); }
	reverse_iterator rbegin() { return reverse_iterator(tail->prev, *this); }
	const_reverse_iterator rbegin()const { return const_reverse_iterator(tail->prev, *this); }
	iterator end() { return iterator(tail,*this); }
	const_iterator end()const { return const_iterator(tail,*this); }
	reverse_iterator rend() { return reverse_iterator(head, *this); }
	const_reverse_iterator rend()const { return const_reverse_iterator(head, *this); }

	int size()const { return theSize; }
	int empty()const { return theSize == 0; }

	object& front() { return *begin(); }
	const object& front()const { return *begin(); }
	object& back() { return *--end(); }
	const object& back()const { return *--end(); }

	void pop_front() { erase(begin()); }
	void pop_back() { erase(--end()); }
	void push_front(const object &obj) { insert(begin(), obj); }
	void push_back(const object &obj) { insert(end(), obj); }

	void splice(iterator iter, List<object> &lst)
	{
		iter.AssertIsValid();
		if (iter.theList != this)
		{
			cerr << "Iterator mismatch exception!" << endl;
			exit(1);
		}

		Node *p = iter.current;
		theSize += lst.size();
		p->prev->next = lst.head->next;
		lst.head->next->prev = p->prev;
		lst.tail->prev->next = p;
		p->prev = lst.tail->prev;
		lst.init();
	}
	iterator insert(iterator iter, const object &obj)
	{
		iter.AssertIsValid();
		if (iter.theList != this)
		{
			cerr << "Iterator mismatch exception!" << endl;
			exit(1);
		}
		Node *p = iter.current;
		theSize++;

		return iterator(p->prev = p->prev->next = new Node(obj, p->prev, p), *this);
	}
	iterator erase(iterator iter)
	{
		iter.AssertIsValid();
		if (iter.theList != this)
		{
			cerr << "Iterator mismatch exception!" << endl;
			exit(1);
		}
		Node *p = iter.current;
		iterator retVal(p->next, *this);
		p->prev->next = p->next;
		p->next->prev = p->prev;
		delete p;
		theSize--;

		return retVal;
	}
	iterator erase(iterator beg, iterator end)
	{
		//Check whether if the given sequence is in the list;
		if (beg.theList != this || end.theList != this)
		{
			cerr << "Iterator mismatch exception!" << endl;
			exit(1);
		}
		for (iterator iter = beg; iter != end;)
			erase(iter++);

		return end;
	}

	//Clear all the nodes except for two sentry nodes;
	void clear()
	{
		while (!empty())
			pop_front();
	}

private:
	int theSize;
	Node *head;
	Node *tail;

	void init()
	{
		theSize = 0;
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->prev = head;
	}
};
#endif // LIST
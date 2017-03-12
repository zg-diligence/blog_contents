/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#include <iostream>
using namespace std;

template <typename object> class Stack
{
	//node of the stack.
	struct Node
	{
		object data;
		Node *next;
		Node(const object &obj = object(), Node *p = nullptr) :data(obj), next(p) {}
	};

public:
	//default constructor.
	Stack() :head(new Node), theSize(0) {}
	
	//copy constructor.
	Stack(const Stack &rhs):head(new Node)
	{
		*this = rhs;
	}

	//move constructor.
	Stack(Stack &&rhs):head(new Node)
	{
		*this = std::move(rhs);
	}

	//destructor.
	~Stack()
	{
		clear();
		delete head;
	}

	//copy assignment operator.
	Stack& operator=(const Stack &rhs)
	{
		if (this != &rhs)
		{
			
			clear();

			Node *tmp = rhs.head->next;
			Node *last = head;
			while (tmp != nullptr)
			{
				last = last->next = new Node(tmp->data);
				tmp = tmp->next;
			}
			theSize = rhs.theSize;
		}
		return *this;
	}

	//move assignment operator.
	Stack& operator=(Stack &&rhs)
	{
		if (this != &rhs)
		{
			clear();

			head->next = rhs.head->next;
			theSize = rhs.theSize;
			rhs.head->next = nullptr;
			rhs.theSize = 0;
		}
		return *this;
	}
	
	//swap elements of two stacks.
	void swap(Stack &rhs)
	{
		size_t cnt = rhs.theSize;
		rhs.theSize = theSize;
		theSize = cnt;

		Node *tmp = rhs.head->next;
		rhs.head->next = head->next;
		head->next = tmp;
	}

	//return size of the stack.
	size_t size()const { return theSize; }

	//check whether if the stack is empty.
	bool empty()const { return theSize == 0; }

	//return (const) reference of the top element.
	object& top() 
	{ 
		checkEmpty();
		return head->next->data; 
	}
	const object& top()const 
	{ 
		checkEmpty();
		return head->next->data;
	}

	//push an element on top of the stack.
	void push(const object &obj)
	{
		head->next = new Node(obj, head->next);
		theSize++;
	}
	void push(object &&obj)
	{
		head->next = new Node(std::move(obj), head->next);
		theSize++;
	}

	//push an element constructed with the given args on top of the stack.
	template <typename...Args>
	void emplace(Args...args)
	{
		push(object(args...));
	}

	//pop the top element out of the stack.
	void pop()
	{
		checkEmpty();
		Node *tmp = head->next;
		head->next = tmp->next;
		delete tmp;
		theSize--;
	}

private:
	Node *head = nullptr;	//sentry node.
	size_t theSize = 0;

	//clear all the elements.
	void clear()
	{
		Node *tmp = head->next;
		head->next = nullptr;
		while (tmp != nullptr)
		{
			
			Node *nxt = tmp->next;
			delete tmp;
			tmp = nxt;
		}
		theSize = 0;
	}

	//check whether the stack is empty.
	void checkEmpty()
	{
		if (theSize == 0)
		{
			cout << "The stack is empty." << endl;
			exit(1);
		}
	}

	template <typename object>
	friend void Swap(Stack<object> &lhs, Stack<object> &rhs);
};

//swap elements of two stacks.
template <typename object>
void Swap(Stack<object> &lhs, Stack<object> &rhs)
{
	size_t cnt = rhs.theSize;
	rhs.theSize = lhs.theSize;
	lhs.theSize = cnt;

	Stack<object>::Node *tmp = rhs.head->next;
	rhs.head->next = lhs.head->next;
	lhs.head->next = tmp;
}
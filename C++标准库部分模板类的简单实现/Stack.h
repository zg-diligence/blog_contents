#include <iostream>
using namespace std;

template <typename object> class stack
{
public:
	stack():head(nullptr){}
	~stack() { while (head)pop(); }

	void push(const object &obj)
	{
		Node *ptr = new Node(obj, head);
		head = ptr;
	}

	object top() { return head->data; }

	void pop()
	{
		Node *ptr = head->next;
		delete head;
		head = ptr;
	}

private:
	class Node
	{
		object data;
		Node *next;
		Node(const object &obj=object(),Node *p=nullptr):data(obj),next(p){}
	};

private:
	Node *head;
};
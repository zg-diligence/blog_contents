#include <iostream>
using namespace std;


template <typename object> class deque
{
public:
	deque():head(nullptr),tail(nullptr){}
	~deque() { while (front)deque(); }

	void enque(const object &obj)
	{
		Node *ptr = new Node(front, nullptr);
		if (tail)
			tail = tail->next = ptr;
		else
			tail = front = ptr;
	}

	object deque()
	{
		object temp = front->data;
		Node *ptr = front;
		if (front->next == nullptr)
			front = rear = nullptr;
		else
			front = front->next;
		delete ptr;
		return temp;
	}

private:
	class Node
	{
		object data;
		Node *next;
		Node(const object &obj = object(), Node *p = nullptr) :data(obj), next(p) {}
	};
private:
	Node *front;
	Node *tail;
};
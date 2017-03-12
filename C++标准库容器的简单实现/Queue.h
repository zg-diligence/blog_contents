/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#include <iostream>
using namespace std;

template <typename object> class Queue
{
    //node of the queue.
    struct Node
    {
        object data;
        Node *next;
        Node(const object &obj = object(), Node *p = nullptr) :data(obj), next(p) {}
    };

public:
    //default cosntructor.
    Queue() = default;

    //copy constructor.
    Queue(const Queue &rhs)
    {
        *this = rhs;
    }

    //move constructor.
    Queue(Queue &&rhs)
    {
        *this = std::move(rhs);
    }

    //destrcutor.
    ~Queue() { clear(); }

    //copy assignment operator.
    Queue& operator=(const Queue &rhs)
    {
        if (this != &rhs)
        {
            clear();

            if (rhs.theSize != 0)
            {
                head = tail = new Node;	//temporary node.
                Node *tmp = rhs.head;
                while (tmp != nullptr)
                {
                    tail = tail->next = new Node(tmp->data);
                    tmp = tmp->next;
                }
                tmp = head;
                head = head->next;
                theSize = rhs.theSize;
                delete tmp;
            }
        }
        return *this;
    }

    //move assignment operator.
    Queue& operator=(Queue &&rhs)
    {
        if (this != &rhs)
        {
            clear();

            head = rhs.head;
            tail = rhs.tail;
            theSize = rhs.theSize;

            rhs.head = nullptr;
            rhs.tail = nullptr;
            rhs.theSize = 0;
        }
        return *this;
    }

    //swap elements of two queues.
    void swap(Queue &rhs)
    {
        size_t cnt = rhs.theSize;
        rhs.theSize = theSize;
        theSize = cnt;

        Node *tmp = rhs.head;
        rhs.head = head;
        head = tmp;

        tmp = rhs.tail;
        rhs.tail = tail;
        tail = tmp;
    }

    //return size of the queue.
    size_t size()const { return theSize; }

    //check whether if the queue is empty.
    bool empty()const { return theSize == 0; }

    //return (const) reference of the first elements.
    object& front()
    {
        checkEmpty();
        return head->data;
    }
    const object& front()const
    {
        checkEmpty();
        return head->data;
    }

    //return (const) reference of the last elemens.
    object& back()
    {
        checkEmpty();
        return tail->data;
    }
    const object& back()const
    {
        checkEmpty();
        return tail->data;
    }

    //add a new element at end of the queue.
    void push(const object &obj)
    {
        Node *ptr = new Node(obj);
        if (theSize == 0)
            head = tail = ptr;
        else
            tail = tail->next = ptr;
        theSize++;
    }
    void push(object &&obj)
    {
        Node *ptr = new Node(obj);
        if (theSize == 0)
            head = tail = ptr;
        else
            tail = tail->next = ptr;
        theSize++;
    }

    //add a new element constructed with the given args.
    template <typename...Args>
    void emplace(Args...args)
    {
        push(object(args...));
    }

    //erase the element at beginning of the queue.
    void pop()
    {
        checkEmpty();
        Node *tmp = head;
        head = head->next;
        if (head == nullptr)
            tail = nullptr;
        delete tmp;
        theSize--;
    }
    
private:
    Node *head = nullptr;	//the first element.
    Node *tail = nullptr;	//the last element.
    size_t theSize = 0;

    //clear all elements of the queueu.
    void clear()
    {
        Node *tmp = head;
        while (tmp != nullptr)
        {
            Node *nxt = tmp->next;
            delete tmp;
            tmp = nxt;
        }
        head = tail = nullptr;
        theSize = 0;
    }

    //check whether if the queue is empty.
    void checkEmpty()
    {
        if (theSize == 0)
        {
            cout << "The queue is empty." << endl;
            exit(1);
        }
    }

    template <typename object>
    friend void Swap(Queue<object> &lhs, Queue<object> &rhs);
};

//swap elements of two queues.
template <typename object>
void Swap(Queue<object> &lhs, Queue<object> &rhs)
{
    size_t cnt = rhs.theSize;
    rhs.theSize = lhs.theSize;
    lhs.theSize = cnt;

    Queue<object>::Node *tmp = rhs.head;
    rhs.head = lhs.head;
    lhs.head = tmp;

    tmp = rhs.tail;
    rhs.tail = lhs.tail;
    lhs.tail = tmp;
}
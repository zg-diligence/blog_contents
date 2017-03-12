/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef DEQUE
#define DEQUE
#define N 11	//default size of a deque block.

#include <iostream>
using namespace std;

template <typename object> class Deque
{
    //block of the deque.
    struct map_node
    {
        object *buffer;
        map_node *pre;
        map_node *next;
        map_node(map_node *p = nullptr, map_node *n = nullptr) :buffer(new object[N]), pre(p), next(n) {}
    };

public:
    class const_iterator
    {
    public:
        //constructor.
        const_iterator() = default;
        const_iterator(object *e, map_node *n, const Deque<object> &deq) :elem(e), nod(n), deq(&deq),buf(n->buffer){}

        //overloaded operator *.
        const object& operator*() { return *elem; }

        //increment operators.
        const_iterator& operator++()
        {
            if (elem == deq->last + 1)
            {
                cout << "out of bounds." << endl;
                exit(1);
            }
            if (elem == buf + N - 2)
            {
                if (deq->last == elem)
                    elem += 1;
                else
                {
                    nod = nod->next;
                    elem = buf = nod->buffer;
                }
            }
            else
                elem += 1;
            return *this;
        }
        const_iterator& operator++(int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }
        const_iterator& operator--()
        {
            if (elem == deq->first)
            {
                cout << "out of bounds." << endl;
                exit(1);
            }
            if (elem == buf)
            {
                nod = nod->pre;
                buf = nod->buffer;
                elem = buf + N - 2;
            }
            else
                elem -= 1;
            return *this;
        }
        const_iterator& operator--(int)
        {
            const_iterator old = *this;
            --(*this);
            return old;
        }

        //comparison operators.
        bool operator==(const const_iterator &rhs)const
        {
            if (deq != rhs.deq)
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
        object *elem = nullptr;		//current element.
        object *buf = nullptr;		//buffer block current element belongs to.
         map_node *nod = nullptr;	//map_node current element belongs to.
        const Deque<object> *deq = nullptr;	//deque current element belongs to.
    };

    class iterator:public const_iterator
    {
    public:
        //constructor.
        iterator() = default;
        iterator(object *e, map_node *n, const Deque<object> &deq) :const_iterator(e, n, deq) {}

        //overloaded operator *.
        object& operator*() { return *elem; }
        const object& operator*() const{ return *elem; }

        //increment operators.
        iterator& operator++()
        {
            if (elem == deq->last + 1)
            {
                cout << "out of bounds." << endl;
                exit(1);
            }
            if (elem == buf + N - 2)
            {
                if (deq->last == elem)
                    elem += 1;
                else
                {
                    nod = nod->next;
                    elem = buf = nod->buffer;
                }
            }
            else
                elem += 1;
            return *this;
        }
        iterator& operator++(int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }
        iterator& operator--()
        {
            if (elem == deq->first)
            {
                cout << "out of bounds." << endl;
                exit(1);
            }
            if (elem == buf)
            {
                nod = nod->pre;
                buf = nod->buffer;
                elem = buf + N - 2;
            }
            else
                elem -= 1;
            return *this;
        }
        iterator& operator--(int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }
    };

public:
    //default constructor.
    Deque() = default;

    //constructor, init a deque with cnt alike elements.
    Deque(int cnt, const object &obj = object())
    {
        assign(cnt, obj);
    }

    //constructor, the parameter is a list.
    Deque(initializer_list<object> lst)
    {
        *this = lst;
    }

    //constructor, the parameter is a sequence.
    template <typename Iter>
    Deque(Iter beg, Iter end)
    {
        assign(beg, end);
    }

    //copy constructor.
    Deque(const Deque &rhs)
    {
        *this = rhs;
    }

    //move constructor.
    Deque(Deque &&rhs)
    {
        *this = std::move(rhs);
    }

    //destructor.
    ~Deque()
    {
        clear();
    }

    //copy assignment operator.
    Deque& operator=(const Deque &rhs)
    {
        if (this != &rhs)
        {
            clear();
            if (rhs.theSize != 0)
            {
                map_node *tmp = rhs.left;
                left = right = new map_node();
                
                //copy the left block and find the first element.
                int i = rhs.first - tmp->buffer;
                first = left->buffer + i;
                for (; i != N-1; ++i)
                    left->buffer[i] = tmp->buffer[i];

                //copy other blocks.
                tmp = tmp->next;
                while (tmp != nullptr)
                {
                    right = right->next = new map_node(right);
                    for (int i = 0; i != N-1; ++i)
                        right->buffer[i] = tmp->buffer[i];
                    tmp = tmp->next;
                }

                //find the last element.
                i = rhs.last - rhs.right->buffer;
                last = right->buffer + i;
                theSize = rhs.theSize;
            }
        }
        return *this;
    }

    //move assignment operator.
    Deque& operator=(Deque &&rhs)
    {
        if (this != &rhs)
        {
            clear();
            left = rhs.left;
            right = rhs.right;
            first = rhs.first;
            last = rhs.last;
            theSize = rhs.theSize;

            rhs.left = rhs.right = nullptr;
            rhs.first = rhs.last = nullptr;
            rhs.theSize = 0;
        }
        return *this;
    }

    //copy assignment operator, the parameter is a list.
    Deque& operator=(initializer_list<object> lst)
    {
        clear();
        size_t cnt = lst.size();
        if (cnt != 0)
        {
            left = right = new map_node();
            first = last = right->buffer;
            for (const auto &elem : lst)
            {
                if (last == right->buffer + N - 2)
                {
                    map_node *t = new map_node();
                    t->pre = right;
                    right->next = t;
                    right = t;
                    //right = right->next = new map_node(right);
                    right->buffer[0] = elem;
                    last = right->buffer;
                }
                else
                    *(++last) = elem;
            }
            theSize = cnt;
            first++;
        }
        return *this;
    }

    //swap elements of two deques.
    void swap(Deque<object> &rhs)
    {
        size_t cnt = rhs.theSize;
        rhs.theSize = theSize;
        theSize = cnt;

        map_node *lt = rhs.left, *rt = rhs.right;
        rhs.left = left;
        rhs.right = right;
        left = lt;
        right = rt;

        object *lc = rhs.first, *rc = rhs.last;
        rhs.first = first;
        rhs.last = last;
        first = lc;
        last = rc;
    }

    //replace elements of the deque with cnt alike elements.
    void assign(int cnt, const object &obj)
    {
        clear();
        if (cnt != 0)
        {
            left = right = new map_node();
            first = last = right->buffer;
            theSize = cnt;
            while(cnt--)
            {
                if (last == right->buffer + N - 2)
                {
                    right = right->next = new map_node(right);
                    right->buffer[0] = obj;
                    last = right->buffer;
                }
                else
                    *(++last) = obj;
            }
            first++;
        }
    }
    
    //replace elements of the deque with a list.
    void assign(initializer_list<object> lst)
    {
        *this = lst;
    }

    //replace elements of the deque with a sequence.
    template <typename Iter>
    void assign(Iter beg, Iter end)
    {
        clear();
        if (beg != end)
        {
            left = right = new map_node();
            first = last = right->buffer;
            while (beg != end)
            {
                if (last == right->buffer + N - 2)
                {
                    right = right->next = new map_node(right);
                    right->buffer[0] = *beg++;
                    last = right->buffer;
                }
                else
                    *(++last) = *beg++;
                theSize++;
            }
            first++;
        }
    }

    //check whether if the list is empty.
    bool empty()const { return theSize == 0; }

    //return size of the list.
    size_t size()const { return theSize; }

    //return (const) reference of the pointed element.
    object& operator[](size_t n)
    {
        if (n >= theSize)
        {
            cout << "out of bounds." << endl;
            exit(1);
        }

        //in the first block.
        if (size_t(left->buffer + N - 1 - first) > n)
            return *(first + n);

        //more than one block.
        n -= left->buffer + N - 1 - first;
        map_node *tmp = left->next;
        while (n >= N - 1)
        {
            n -= (N - 1);
            tmp = tmp->next;
        }
        return tmp->buffer[n];
    }
    const object& operator[](size_t n)const
    {
        if (n >= theSize)
        {
            cout << "out of bounds." << endl;
            exit(1);
        }

        //in the first block.
        if (size_t(left->buffer + N - 1 - first) > n)
            return *(first + n);

        //more than one block.
        n -= left->buffer + N - 1 - first;
        map_node *tmp = left->next;
        while (n >= N - 1)
        {
            n -= (N - 1);
            tmp = tmp->next;
        }
        return tmp->buffer[n];
    }

    //return (const) reference of the first element.
    object& front()
    {
        checkEmpty();
        return *first;
    }
    const object& front()const
    {
        checkEmpty();
        return *first;
    }

    //return (const) reference of the last element.
    object& back()
    {
        checkEmpty();
        return *last;
    }
    const object& back()const
    {
        checkEmpty();
        return *last;
    }

    //return different kinds of iterators.
    iterator begin()
    { 
        return iterator(first, left, *this);
    }
    const_iterator begin()const 
    { 
        return const_iterator(first, left, *this); 
    }
    const_iterator cbegin() 
    { 
        return const_iterator(first, left, *this);
    }
    iterator end() 
    { 
        return iterator(last + 1, right, *this);
    }
    const_iterator end()const 
    { 
        return const_iterator(last + 1, right, *this); 
    }
    const_iterator cend()
    {
        return const_iterator(last + 1, right, *this);
    }

    //insert an element at beginning of the deque.
    void push_front(const object &obj)
    {
        if (theSize == 0)
        {
            left = right = new map_node();
            left->buffer[N - 1] = obj;
            first = last = left->buffer + N - 1;
        }
        else if (first == left->buffer)
        {
            left = new map_node(nullptr, left);
            left->next->pre = left;
            left->buffer[N - 1] = obj;
            first = left->buffer + N - 1;
        }
        else
            *(--first) = obj;
        theSize++;
    }
    
    //append an element at end of the deque.
    void push_back(const object &obj)
    {
        if (theSize == 0)
        {
            left = right = new map_node();
            left->buffer[0] = obj;
            first = last = left->buffer;
        }
        else if (last == right->buffer + N - 1)
        {
            right = new map_node(right, nullptr);
            right->pre->next = right;
            right->buffer[0] = obj;
            last = right->buffer;
        }
        else
            *(++last) = obj;
        theSize++;
    }

    //erase the first element.
    void pop_front()
    {
        checkEmpty();
        if (first == left->buffer + N - 1)
        {
            if(theSize!=1)
            {
                map_node *tmp = left;
                left = left->next;
                delete tmp;
                first = left->buffer;
            }
        }
        else
            first++;
        if (--theSize == 0)
        {
            delete[]left->buffer;
            delete left;
            left = right = nullptr;
            first = last = nullptr;
        }
    }

    //erase the last element.
    void pop_back()
    {
        checkEmpty();
        if (last == right->buffer)
        {
            if(theSize != 1)
            {
                map_node *tmp = right;
                right = right->pre;
                delete tmp;
                last = right->buffer + N - 1;
            }
        }
        else
            last--;
        if (--theSize == 0)
        {
            delete[]left->buffer;
            delete left;
            left = right = nullptr;
            first = last = nullptr;
        }
    }

    //clear the deque.
    void clear()
    {
        if (theSize != 0)
        {
            map_node *tmp = nullptr, *nxt = left;
            while (nxt != nullptr)
            {
                tmp = nxt;
                nxt = nxt->next;
                delete[]tmp->buffer;
                delete tmp;
            }
            theSize = 0;
            left = right = nullptr;
            first = last = nullptr;
        }
    }

    //comparasion operators.
    bool operator==(const Deque<object> &rhs)const
    {
        if (theSize != rhs.theSize)
            return false;
        for (int i = 0; i != theSize; ++i)
            if (operator[](i) != rhs[i])
                return false;
        return true;
    }
    bool operator!=(const Deque<object> &rhs)const
    {
        return !(*this == rhs);
    }
    bool operator<(const Deque<object> &rhs)const
    {
        int i = 0;
        for (; i != theSize && i != rhs.theSize; ++i)
        {
            if (operator[](i) < rhs[i])
                return true;
            if (operator[](i) > rhs[i])
                return false;
        }
        if (i == theSize && i != rhs.theSize)
            return true;
        return false;
    }
    bool operator<=(const Deque<object> &rhs)const
    {
        return !(*this > rhs);
    }
    bool operator>(const Deque<object> &rhs)const
    {
        int i = 0;
        for (; i != theSize && i != rhs.theSize; ++i)
        {
            if (operator[](i) > rhs[i])
                return true;
            if (operator[](i) < rhs[i])
                return false;
        }
        if (i != theSize && i == rhs.theSize)
            return true;
        return false;
    }
    bool operator>=(const Deque<object> &rhs)const
    {
        return !(*this < rhs);
    }

private:
    map_node *left = nullptr;
    map_node *right = nullptr;
    object *first = nullptr;
    object *last = nullptr;
    size_t theSize = 0;

    //check whether the deque is empty.
    void checkEmpty()
    {
        if (theSize == 0)
        {
            cout << "The deque is empty." << endl;
            exit(1);
        }
    }

    template <typename object>
    friend void Swap(Deque<object> &lhs, Deque<object> &rhs);
};

//swap elements of two deques.
template <typename object>
void Swap(Deque<object> &lhs, Deque<object> &rhs)
{
    size_t cnt = rhs.theSize;
    rhs.theSize = lhs.theSize;
    lhs.theSize = cnt;

    Deque<object>::map_node *lt = rhs.left, *rt = rhs.right;
    rhs.left = lhs.left;
    rhs.right = lhs.right;
    lhs.left = lt;
    lhs.right = rt;

    object *lc = rhs.first, *rc = rhs.last;
    rhs.first = lhs.first;
    rhs.last = lhs.last;
    lhs.first = lc;
    lhs.last = rc;
}

#endif
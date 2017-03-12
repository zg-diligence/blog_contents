/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef VECTOR
#define VECTOR

#include <iostream>
using namespace std;

template <typename object> class Vector
{
public:
    class const_iterator
    {
    public:
        typedef int ptrdiff_t;

    public:
        //default constructor.
        const_iterator() = default;
        
        //overloaded operator *.
        const object& operator*() 
        { 
            if (vec->objects > elem || (elem - vec->objects >= int(vec->theSize)))
            {
                cout << "Out of bounds." << endl;
                exit(1);
            }
            return *elem; 
        }

        //increment operators.
        const_iterator& operator++() 
        {
            elem += 1;
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
            elem -= 1;
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator old = *this;
            --(*this);
            return old;
        }

        //distance computing of iterators.
        const_iterator operator+(int n)
        {
            return const_iterator(elem + n, *vec);
        }
        const_iterator& operator+=(int n)
        {
            elem += n;
            return *this;
        }
        const_iterator operator-(int n)
        {
            return const_iterator(elem - n, *vec);
        }
        const_iterator& operator-=(int n)
        {
            elem -= n;
            return *this;
        }
        ptrdiff_t operator-(const_iterator rhs)
        {
            return elem - rhs.elem;
        }

        //comparision operators.
        bool operator==(const const_iterator &rhs)const
        {
            if (vec != rhs.vec)
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
        object *elem = nullptr;
        const Vector<object> *vec = nullptr;

        //constructor.
        const_iterator(object *e, const Vector<object> &v):elem(e),vec(&v){}
        friend class Vector<object>;
    };

    class iterator:public const_iterator
    {
    public:
        //default constructor.
        iterator() = default;

        //overloaded operator *.
        object& operator*()
        { 
            if (vec->objects > elem || (elem - vec->objects >= int(vec->theSize)))
            {
                cout << "Out of bounds." << endl;
                exit(1);
            }
            return *elem;
        }
        const object& operator*()const { return const_iterator::operator*(); }

        //increment operators.
        iterator& operator++()
        {
            elem += 1;
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
            elem -= 1;
            return *this;
        }
        iterator operator--(int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }

        //distance computing of iterators.
        iterator operator+(int n)
        {
            return iterator(elem + n, *vec);
        }
        iterator& operator+=(int n)
        {
            elem += n;
            return *this;
        }
        iterator operator-(int n)
        {
            return iterator(elem - n, *vec);
        }
        iterator& operator-=(int n)
        {
            elem -= n;
            return *this;
        }
        ptrdiff_t operator-(iterator rhs)
        {
            return elem - rhs.elem;
        }

    protected:
        iterator(object *e, const Vector<object> &v) :const_iterator(e,v) {}
        friend class Vector<object>;
    };

public:
    //default constructor;
    Vector() = default;

    //constructor, create a vector with cnt alike elements.
    Vector(int cnt, const object &obj = object())
    {
        insert(end(), cnt, obj);
    }

    //constructor, the parameter is a list.
    Vector(initializer_list<object> lst) 
    {
        *this = lst;
    }

    //constructor, the parameter is a sequence.
    template <typename Iter>
    Vector(Iter beg, Iter end)
    {
        insert(cend(), beg, end);
    }

    //copy constructor.
    Vector(const Vector &rhs) { *this = rhs; }

    //move constructor.
    Vector(Vector &&rhs) { *this = std::move(rhs); }

    //destructor.
    ~Vector() { clear(); }

    //copy assignment operator.
    Vector& operator=(const Vector &rhs)
    {
        if (this != &rhs)
        {
            delete[]objects;
            theSize = rhs.theSize;
            theCapacity = rhs.theCapacity;

            objects = new object[theCapacity + 1];
            for (int i = 0; i != theSize; ++i)
                objects[i] = rhs.objects[i];
        }
        return *this;
    }

    //move assignment operator.
    Vector& operator=(Vector &&rhs)
    {
        if (this != &rhs)
        {
            delete[]objects;
            objects = rhs.objects;
            theSize = rhs.theSize;
            theCapacity = rhs.theCapacity;
            
            rhs.theSize = 0;
            rhs.theCapacity = 0;
            rhs.objects = nullptr;
        }
        return *this;
    }

    //copy assignment operator, the parameter is a list.
    Vector& operator=(initializer_list<object> lst)
    {
        delete[]objects;
        theSize = lst.size();
        theCapacity = theSize;
        objects = new object[theCapacity + 1];
        object *iter = objects;
        for (const auto &elem : lst)
            *iter++ = elem;
        return *this;
    }

    //swap elements of two vectors.
    void swap(Vector<object> &rhs)
    {
        size_t cnt = theSize;
        theSize = rhs.theSize;
        rhs.theSize = cnt;

        cnt = theCapacity;
        theCapacity = rhs.theCapacity;
        rhs.theCapacity = cnt;

        object *tmp = objects;
        objects = rhs.objects;
        rhs.objects = tmp;
    }

    //replace elements of the vector with cnt alike new elements.
    void assign(int cnt, const object &obj)
    {
        delete[]objects;
        theSize = cnt;
        theCapacity = cnt;
        objects = new object[theCapacity + 1];
        for (int i = 0; i != cnt; ++i)
            objects[i] = obj;
    }

    //replace elements of the vector with a list;
    void assign(initializer_list<object> lst)
    {
        *this = lst;
    }

    //replace elements of the vector with a sequence; 
    template <typename Iter>
    void assign(Iter beg, Iter end)
    {
        clear();
        insert(cend(), beg, end);
    }

    //return (const) reference of the pointed element.
    object& operator[](size_t index)
    {
        if (index >= theSize)
        {
            cout << "Index out of bounds!" << endl;
            exit(1);
        }
        return objects[index];
    }
    const object& operator[](size_t index)const
    {
        if (index >= theSize)
        {
            cout << "Index out of bounds!" << endl;
            exit(1);
        }
        return objects[index];
    }

    //return (const) reference of the first element.
    object& front()
    {
        checkEmpty();
        return objects[0];
    }
    const object& front()const
    {
        checkEmpty();
        return objects[0];
    }

    //return (const) reference of the last element.
    object& back()
    {
        checkEmpty();
        return objects[theSize - 1];
    }
    const object& back()const
    {
        checkEmpty();
        return objects[theSize - 1];
    }

    //check whether if the class is empty.
    bool empty() const { return theSize == 0; }

    //return size of the vector.
    size_t size() const { return theSize; }

    //return capacity of the vector.
    size_t capacity() const{ return theCapacity; }

    //return different iterators.
    iterator begin()
    {
        return iterator(objects, *this);
    }
    const_iterator begin() const
    {
        return const_iterator(objects, *this);
    }
    const_iterator cbegin()const
    {
        return const_iterator(objects, *this);
    }
    iterator end()
    {
        return iterator(&objects[theSize], *this);
    }
    const_iterator end() const
    {
        return const_iterator(&objects[theSize], *this);
    }
    const_iterator cend()const
    {
        return const_iterator(&objects[theSize], *this);
    }

    //reset size of the vector.
    void resize(size_t newSize, const object &obj = object())
    {
        while(newSize > theCapacity)
            reserve(theCapacity * 2 + 1);
        if (newSize > theSize)
            for (int i = theSize; i != newSize; ++i)
                objects[i] = obj;
        theSize = newSize;
    }

    //reset capacity of the class.
    void reserve(size_t newCapacity)
    {
        if (newCapacity > theCapacity)
        {
            //apply for new memory and copy elements, and then release the old memory.
            object *old = objects;
            objects = new object[newCapacity + 1];
            theCapacity = newCapacity;
            for (int i = 0; i != theSize; ++i)
                objects[i] = old[i];
            delete[]old;
        }
    }

    //append new element at end of the vector.
    void push_back(const object &k)
    {
        if (theSize == theCapacity)
            reserve(theCapacity * 2 + 1);
        objects[theSize++] = k;
    }

    //append a new element at end of the vector, construct it with provided args.
    template <typename...Args>
    void emplace_back(Args&& ...args)
    {
        push_back(object(args...));
    }

    //erase the last element.
    void pop_back() 
    { 
        checkEmpty();
        --theSize; 
    }

    //insert an element before the pointed position.
    iterator insert(const_iterator p, const object &obj)
    {
        checkIterator(p);
        auto iter = moveBack(p, 1);
        *iter = obj;
        return iterator(iter, *this);
    }

    //insert an element before the pointed position, contruct it with provided args.
    template <typename...Args>
    iterator emplace(const_iterator p, Args...args)
    {
        return insert(p, object(args...));
    }

    //insert cnt alike elements before the pointed position.
    iterator insert(const_iterator p, int cnt, const object &obj)
    {
        checkIterator(p);
        if (cnt == 0)
            return iterator(p.elem, *this);

        //move elements backward and insert elements.
        auto iter = moveBack(p, cnt), ret = iter;
        int dist = cnt;
        while (dist--)
            *iter++ = obj;
        return iterator(ret, *this);
    }

    //insert a list before the pointed position.
    iterator insert(const_iterator p, initializer_list<object> lst)
    {
        checkIterator(p);
        if (lst.size() == 0)
            return iterator(p.elem, *this);

        //move elements backward and insert elements.
        auto iter = moveBack(p, lst.size()), ret = iter;
        for (auto &elem : lst)
            *iter++ = elem;
        return iterator(ret, *this);
    }

    //insert a sequence before the pointed position.
    template <typename Iter>
    iterator insert(const_iterator p, Iter beg, Iter end)
    {
        checkIterator(p);
        if (beg == end)
            return iterator(p.elem, *this);

        //caculate number of the elements to insert.
        size_t cnt = 0;
        Iter tmp = beg;
        while (tmp++ != end)
            cnt++;

        //move elements backward and insert elements.
        auto iter = moveBack(p, cnt), ret = iter;
        while(beg != end)
            *iter++ = *beg++;
        return iterator(ret, *this);
    }

    //erase the pointed element.
    iterator erase(const_iterator p)
    {
        checkEmpty();
        checkIterator(p);
        if (p == cend())
        {
            cout << "out of bounds." << endl;
            exit(1);
        }

        //move elements forward.
        object *cur = p.elem;
        while (cur != &objects[theSize - 1])
            *cur++ = *(cur + 1);
        theSize--;
        return iterator(p.elem, *this);
    }

    //erase the pointed elements sequence.
    iterator erase(const_iterator beg, const_iterator end)
    {
        checkEmpty();
        checkIterator(beg);
        checkIterator(end);
        if (beg == end)
            return iterator(end.elem, *this);

        //caculate number of the elements to erase.
        object *lc = beg.elem, *rc = end.elem;
        size_t cnt = rc - lc;

        ////move elements forward.
        while (rc != &objects[theSize])
            *lc++ = *rc++;
        theSize -= cnt;

        //the vector is empty or the end is cend().
        if (theSize == 0 || end.elem == &objects[theSize])
            return iterator(&objects[theSize], *this);
        return iterator(beg.elem, *this);
    }

    //clear the vector.
    void clear()
    {
        delete[]objects;
        objects = nullptr;
        theSize = 0;
        theCapacity = 0;
    }

    //comparasion operators.
    bool operator==(const Vector<object> &rhs)const
    {
        if (theSize != rhs.theSize)
            return false;
        for (int i = 0; i != theSize; ++i)
            if (objects[i] != rhs.objects[i])
                return false;
        return true;
    }
    bool operator!=(const Vector<object> &rhs)const
    { 
        return !(*this == rhs); 
    }
    bool operator<(const Vector<object> &rhs)const
    {
        int i = 0;
        for (; i != theSize && i != rhs.theSize; ++i)
        {
            if (objects[i] < rhs.objects[i])
                return true;
            if (objects[i] > rhs.objects[i])
                return false;
        }
        if (i == theSize && i != rhs.theSize)
            return true;
        return false;
    }
    bool operator<=(const Vector<object> &rhs)const
    {
        return !(*this > rhs);
    }
    bool operator>(const Vector<object> &rhs)const
    {
        int i = 0;
        for (; i != theSize && i != rhs.theSize; ++i)
        {
            if (objects[i] > rhs.objects[i])
                return true;
            if (objects[i] < rhs.objects[i])
                return false;
        }
        if (i != theSize && i == rhs.theSize)
            return true;
        return false;
    }
    bool operator>=(const Vector<object> &rhs)const
    {
        return !(*this < rhs);
    }

private:
    size_t theSize = 0;
    size_t theCapacity = 0;
    object *objects = nullptr;
    
    //move elements backward from the element the iterator refers to.
    object* moveBack(const_iterator p, size_t cnt)
    {
        //check whether if the memory is enough.
        int dist = cend() - p;	//have to use type int.
        while (theSize + cnt > theCapacity)
            reserve(2 * theCapacity + 1);

        //empty vector.
        if (theSize == 0)
        {
            theSize = cnt;
            return objects;
        }

        //move elements backward.
        object *iter = &objects[theSize - 1];
        theSize += cnt;	//this sentence must be there.
        while (dist--)
        {
            *(iter + cnt) = *iter;
            --iter;
        }
        return iter + 1;
    }

    //check whether if the iterator is valid.
    void checkIterator(const_iterator p)const
    {
        if (p.vec != this)
        {
            cerr << "Iterator is imcompatible." << endl;
            exit(1);
        }
    }

    //check whether if the vector is empty.
    void checkEmpty()const
    {
        if (theSize == 0)
        {
            cout << "The vector is empty." << endl;
            exit(1);
        }
    }

    template <typename object>
    friend void Swap(Vector<object> &lhs, Vector<object> &rhs);
};

//swap elements of two vectors.
template <typename object>
void Swap(Vector<object> &lhs, Vector<object> &rhs)
{
    size_t cnt = lhs.theSize;
    lhs.theSize = rhs.theSize;
    rhs.theSize = cnt;

    cnt = lhs.theCapacity;
    lhs.theCapacity = rhs.theCapacity;
    rhs.theCapacity = cnt;

    object *tmp = lhs.objects;
    lhs.objects = rhs.objects;
    rhs.objects = tmp;
}

#endif
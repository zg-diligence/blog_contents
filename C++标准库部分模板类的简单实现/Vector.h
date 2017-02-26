#ifndef VECTOR
#define VECTOR

#include <iostream>
#include <initializer_list>
using namespace std;

template <typename object> class Vector
{
public:
	Vector() = default;
	explicit Vector(int initsize) :theSize(initsize), theCapacity(initsize + SPARE_CAPACITY), objects(new object[theCapacity])
	{
		for (int i = 0; i != theSize; ++i)
			objects[i] = 0;
	}
	Vector(initializer_list<object> li) :theSize(li.size()), theCapacity(li.size()), objects(new object[theCapacity])
	{
		auto beg = li.begin();
		for (int i = 0; i != theSize; ++i)
			objects[i] = *beg++;
	}
	Vector(const Vector &rhs) { *this = rhs;}
	Vector(Vector &&rhs){ *this = rhs; }
	~Vector(){delete[]objects;}

	Vector& operator=(const Vector &rhs)
	{
		if (this != &rhs)
		{
			delete[]objects;
			theSize = rhs.theSize;
			theCapacity = rhs.theCapacity;

			objects = new object[theCapacity];
			for (int i = 0; i != theSize; ++i)
				objects[i] = rhs.objects[i];
		}
		return *this;
	}
	Vector& operator=(Vector &&rhs)
	{
		if (this != &rhs)
		{
			delete[]objects;
			theSize = std::move(rhs.theSize);
			theCapacity = std::move(rhs.theCapacity);
			objects = rhs.objects;

			rhs.theSize = 0;
			rhs.theCapacity = 0;
			rhs.objects = nullptr;
		}
		return *this;
	}

	void resize(int newSize)
	{
		if (newSize > theSize)
		{
			reserve(newSize * 2);
			for (int i = theSize; i != newSize; ++i)
				objects[i] = 0;
		}
		theSize = newSize;
	}
	void reserve(int newCapacity)
	{
		if (newCapacity > theCapacity)
		{
			object *oldArray = objects;

			objects = new object[newCapacity];
			theCapacity = newCapacity;

			for (int i = 0; i != theSize; ++i)
				objects[i] = oldArray[i];

			delete[]oldArray;
		}
	}

	object& operator[](int index) 
	{
		if (index < 0 || index >= theSize)
		{
			cout << "Index out of bounds!" << endl;
			exit(1);
		}
		return objects[index]; 
	}
	const object& operator[](int index)const 
	{ 
		if (index < 0 || index >= theSize)
		{
			cout << "Index out of bounds!" << endl;
			exit(1);
		}
		return objects[index];
	}

	bool empty() { return theSize==0; }
	int size() { return theSize; }
	int capacity() { return theCapacity; }

	void push_back(const object &obj)
	{
		if (theSize == theCapacity)
			reserve(theCapacity * 2);
		objects[theSize++] = obj;
	}
	void pop_back() { --theSize; }
	const object& back() { return objects[theSize - 1]; }

	typedef object * iterator;
	typedef const object * const_iterator;

	iterator insert(iterator p,const object &obj)
	{
		int pos = end() - p;
		if (theSize == theCapacity)
			reserve(2 * theCapacity);
		theSize++;
		auto iter = end() - 1;
		while (pos--)
		{
			*iter = *(iter - 1);
			--iter;
		}
		*iter = obj;

		return objects;
	}
	iterator erase(iterator p)
	{
		auto ret = p;
		while (p != end() - 1)
		{
			*p = *(p + 1);
			++p;
		}
		theSize--;
		return ret;
	}
	iterator erase(iterator erase_beg, iterator erase_end)
	{
		auto ret = erase_beg;
		size_t pos = erase_end - erase_beg;
		while (erase_end != end())
			*erase_beg++ = *erase_end++;
		theSize -= pos;

		return ret;
	}

	iterator begin() { return objects; }
	const_iterator begin() const{ return objects; }
	const_iterator cbegin()const { return objects; }
	iterator end() { return &objects[theSize]; }
	const_iterator end() const{ return &objects[theSize]; }
	const_iterator cend()const{ return &objects[theSize]; }

	enum { SPARE_CAPACITY = 16 };

private:
	int theSize = 0;
	int theCapacity = 0;
	object *objects = nullptr;
};

#endif
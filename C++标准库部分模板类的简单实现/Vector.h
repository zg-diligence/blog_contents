#ifndef VECTOR
#define VECTOR

#include <iostream>
#include <initializer_list>
using namespace std;

template <typename object> class Vector
{
public:
	//default constructor;
	Vector() = default;

	//constructor, the only one argument is size of the class to construct.
	//using explicit to prohibit automatical conversion, it can only be used to initialize directly.
	explicit Vector(int initsize) :theSize(initsize), theCapacity(initsize), objects(new object[theCapacity]()) {}

	//constructor, the argument is a list.
	Vector(initializer_list<object> lst) :theSize(lst.size()), theCapacity(lst.size()), objects(new object[theCapacity])
	{
		auto beg = lst.begin();
		for (int i = 0; i != theSize; ++i)	//copy contents of the assignment.
			objects[i] = *beg++;
	}

	//copy constructor using copy assignment operator.
	Vector(const Vector &rhs) { *this = rhs; }

	//move constructor using move assignment operator.
	Vector(Vector &&rhs) { *this = std::move(rhs); }

	//destructor to release the memory.
	~Vector() { delete[]objects; }

	//copy assignment operator.
	Vector& operator=(const Vector &rhs)
	{
		if (this != &rhs)	//judge whether if both the operands are the same. 
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

	//move assignment operator.
	Vector& operator=(Vector &&rhs)
	{
		if (this != &rhs)
		{
			delete[]objects;
			theSize = std::move(rhs.theSize);
			theCapacity = std::move(rhs.theCapacity);
			objects = rhs.objects;	//using memory of right operand directly.

			rhs.theSize = 0;
			rhs.theCapacity = 0;
			rhs.objects = nullptr;	//having to do this.
		}
		return *this;
	}

	//reset size of the class.
	//if the new size if bigger than before, increase the size and initialize the new elements with zero.
	//or else, decrease the size abandoning the redundant elements directly.
	void resize(int newSize)
	{
		//if the new size is bigger than capacity of the class, ask for new memory.
		if (newSize > theCapacity)
			reserve(newSize * 2);
		theSize = newSize;
	}

	//reset capacity of the class.
	//if the new capacity is less than or equal to before, do nothing.
	void reserve(int newCapacity)
	{
		if (newCapacity > theCapacity)
		{
			//ask for new memory and copy the original contents, and then release the old memory.
			object *oldArray = objects;
			objects = new object[newCapacity]();
			theCapacity = newCapacity;
			for (int i = 0; i != theSize; ++i)
				objects[i] = oldArray[i];
			delete[]oldArray;
		}
	}

	//overload the operator [].There are two versions.
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

	//judge whether if the class is empty, return true if it's, return false if not.
	bool empty() const{ return theSize == 0; }

	//return size of the class.
	int size() const{ return theSize; }

	//return capacity of the class.
	int capacity() const{ return theCapacity; }

	//append new element at end of the class.
	void push_back(const object &obj)
	{
		if (theCapacity == 0)
			reserve(1);
		if (theSize == theCapacity)
			reserve(theCapacity * 2);
		objects[theSize++] = obj;
	}

	//erase the last element.
	void pop_back() { --theSize; }

	//return the last element.
	const object& back() { return objects[theSize - 1]; }

	typedef object * iterator;
	typedef const object * const_iterator;

	//insert an element after the given iterator.
	iterator insert(iterator p, const object &obj)
	{
		int pos = end() - p; //have to caculate the distance before calling the function reserve.
		if (theSize == theCapacity)
			reserve(2 * theCapacity);

		//move all the elements after the given iterator backward. 
		auto iter = end()-1;
		while (pos--)
		{
			*(iter + 1) = *iter;
			--iter;
		}

		//insert the element into correct position.
		*(++iter) = obj;
		theSize++;

		return iter;
	}

	iterator insert(iterator p, initializer_list<object> lst)
	{
		int pos = end() - p - 1, tmp = pos;
		int lst_size = lst.size();
		while (theSize + lst_size > theCapacity)
			reserve(2 * theCapacity);

		//move all the elements after the given iterator backward. 
		auto iter = end() - 1;
		while (tmp--)
		{
			*(iter + lst_size) = *iter;
			--iter;
		}

		//insert the lst into correct position.
		iter = end() - pos;
		for (auto &elem : lst)
		{
			cout << elem << " " << endl;
			*iter++ = elem;
		}
		theSize += lst.size();

		return iter - lst_size;
	}

	//erase the element that the given iterator refers to.
	iterator erase(iterator p)
	{
		//move all the elements after the given iterator farward.
		auto ret = p;
		while (p != end() - 1)
		{
			*p = *(p + 1);
			++p;
		}
		theSize--;
		return ret;
	}

	//erase all the elements from the first iterator to the second iterator.
	//not including the element that the second iterator refers to.
	iterator erase(iterator erase_beg, iterator erase_end)
	{
		auto ret = erase_beg;
		size_t pos = erase_end - erase_beg;
		while (erase_end != end())
			*erase_beg++ = *erase_end++;
		theSize -= pos;
		return ret;
	}

	//return iterator that refers to the first element or the position after the last element.
	//there are six versions applied to different situations.
	iterator begin() { return objects; }
	const_iterator begin() const { return objects; }
	const_iterator cbegin()const { return objects; }
	iterator end() { return &objects[theSize]; }
	const_iterator end() const { return &objects[theSize]; }
	const_iterator cend()const { return &objects[theSize]; }

private:
	int theSize = 0;
	int theCapacity = 0;
	object *objects = nullptr;
};

#endif
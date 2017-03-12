/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author:Gang Zhang
 *  Date:2017.3.11
 */

#ifndef STRING
#define STRING

#include <iostream>
using namespace std;

//The functions replace and find has not been implemented.
class String
{
public:
	class const_iterator
	{
	public:
		typedef int ptrdiff_t;

		//default constructor.
		const_iterator() = default;

		//overloaded operator *.
		const char& operator*()
		{
			if (s->str > ch || (ch - s->str >= int(s->theSize)))
			{
				cout << "Out of bounds." << endl;
				exit(1);
			}
			return *ch;
		}

		//increment operators.
		const_iterator& operator++()
		{
			ch += 1;
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
			ch -= 1;
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
			return const_iterator(ch + n, *s);
		}
		const_iterator& operator+=(int n)
		{
			ch += n;
			return *this;
		}
		const_iterator operator-(int n)
		{
			return const_iterator(ch - n, *s);
		}
		const_iterator& operator-=(int n)
		{
			ch -= n;
			return *this;
		}
		ptrdiff_t operator-(const_iterator rhs)
		{
			return ch - rhs.ch;
		}

		//comparision operators.
		bool operator==(const const_iterator &rhs)const
		{
			if (s != rhs.s)
			{
				cout << "Iterator is imcompatible!" << endl;
				exit(1);
			}
			return ch == rhs.ch;
		}
		bool operator!=(const const_iterator &rhs)const
		{
			return !(*this == rhs);
		}

	protected:
		char *ch = nullptr;
		const String *s = nullptr;

		//constructor.
		const_iterator(char *e, const String &_s) :ch(e), s(&_s) {}
		friend class String;
	};

	class iterator :public const_iterator
	{
	public:
		//default constructor.
		iterator() = default;

		//overloaded operator *.
		char& operator*()
		{
			if (s->str > ch || (ch - s->str >= int(s->theSize)))
			{
				cout << "Out of bounds." << endl;
				exit(1);
			}
			return *ch;
		}
		const char& operator*()const
		{
			if (s->str > ch || (ch - s->str >= int(s->theSize)))
			{
				cout << "Out of bounds." << endl;
				exit(1);
			}
			return *ch;
		}

		iterator& operator++()
		{
			ch += 1;
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
			ch -= 1;
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
			return iterator(ch + n, *s);
		}
		iterator& operator+=(int n)
		{
			ch += n;
			return *this;
		}
		iterator operator-(int n)
		{
			return iterator(ch - n, *s);
		}
		iterator& operator-=(int n)
		{
			ch -= n;
			return *this;
		}
		ptrdiff_t operator-(iterator rhs)
		{
			return ch - rhs.ch;
		}

	protected:
		iterator(char *e, const String &_s) :const_iterator(e, _s) {}
		friend class String;
	};

public:
	//default constructor.
	String() = default;

	//constructor, create a string which has cnt alike chars.
	String(int cnt, char ch = char())
	{
		insert(0, cnt, ch);
	}

	//constructor, the parameter is a list.
	String(initializer_list<char> lst)
	{
		*this = lst;
	}

	//constructor, the parameter is a sequence.
	template <typename Iter>
	String(Iter beg, Iter end)
	{
		insert(cend(), beg, end);
	}

	//constructor, the parameter is a pointer of a const character string.
	String(const char *s)
	{
		*this = s;
	}

	//copy constructor.
	String(const String &s)
	{
		*this = s;
	}

	//move constructor.
	String(String &&s)
	{
		*this = std::move(s);
	}

	//destructor.
	~String()
	{
		clear();
	}

	//assignment operator, parameter is pointer of a character string.
	String& operator=(const char *rhs)
	{
		if (rhs != nullptr)
		{
			size_t cnt = 0;
			while (rhs[cnt++] != '\0') {}

			theSize = cnt - 1;
			theCapacity = theSize + 1;
			str = new char[theCapacity + 1];
			for (int i = 0; i != theSize; ++i)
				str[i] = rhs[i];
			str[cnt] = '\0';
		}
		return *this;
	}

	//copy assignment operator.
	String& operator=(const String &rhs)
	{
		if (this != &rhs)
		{
			delete[]str;
			theSize = rhs.theSize;
			theCapacity = rhs.theCapacity;

			str = new char[theCapacity + 1];
			for (int i = 0; i != theSize; ++i)
				str[i] = rhs.str[i];
			str[theSize] = '\0';
		}
		return *this;
	}

	//move assignment operator.
	String& operator=(String &&rhs)
	{
		if (str != rhs.str)
		{
			delete[]str;
			str = rhs.str;
			theSize = rhs.theSize;
			theCapacity = rhs.theCapacity;

			rhs.theSize = 0;
			rhs.theCapacity = 0;
			rhs.str = nullptr;
		}
		return *this;
	}

	//copy assignment operator, the parameter is a list.
	String& operator=(initializer_list<char> lst)
	{
		delete[]str;
		theSize = lst.size();
		theCapacity = theSize + 1;
		str = new char[theCapacity + 1];
		char *iter = str;
		for (const auto &elem : lst)
			*iter++ = elem;
		*iter = '\0';
		return *this;
	}

	//swap chars of two strings.
	void swap(String &rhs)
	{
		size_t cnt = theSize;
		theSize = rhs.theSize;
		rhs.theSize = cnt;

		cnt = theCapacity;
		theCapacity = rhs.theCapacity;
		rhs.theCapacity = cnt;

		char *tmp = str;
		str = rhs.str;
		rhs.str = tmp;
	}

	//replace chars of the string with new chars.
	String& assign(int cnt, char ch)
	{
		clear();
		insert(0, cnt, ch);
		return *this;
	}
	String& assign(const String &s)
	{
		return *this = s;
	}
	String& assign(const String &s, size_t pos, size_t len)
	{
		clear();
		insert(0, s, pos, len);
		return *this;
	}
	String& assign(const char *cp, size_t len = 0)
	{
		clear();
		insert(0, cp, len);
		return *this;
	}
	String& assign(initializer_list<char> lst)
	{
		return *this = lst;
	}
	template <typename Iter>
	String& assign(Iter beg, Iter end)
	{
		clear();
		insert(cend(), beg, end);
		return *this;
	}

	//overloaded operator +=.
	String& operator +=(char c)
	{
		if (theSize + 1 == theCapacity)
			reserve(2 * theCapacity + 2);
		*(str + theSize++) = c;
		*(str + theSize) = '\0';
		return *this;
	}
	String& operator +=(const char *rhs)
	{
		return append(rhs);
	}
	String& operator +=(const String &rhs)
	{
		return append(rhs.str);
	}

	//overloaded operator +.
	String operator+(char c)const
	{
		String src = *this;
		return src += c;
	}
	String operator+(const char *rhs)const
	{
		String src = *this;
		return src += rhs;
	}
	String operator+(const String &rhs)const
	{
		String src = *this;
		return src += rhs;
	}

	//return (const) reference of the pointed character;
	char& operator[](size_t index) 
	{ 
		checkPos(index);
		return str[index]; 
	}
	const char& operator[](size_t index)const 
	{ 
		checkPos(index);
		return str[index]; 
	}

	//return (const) reference the first char.
	char& front()
	{
		checkEmpty();
		return str[0];
	}
	const char& front()const
	{
		checkEmpty();
		return str[0];
	}

	//return (const) reference the last char.
	char& back()
	{
		checkEmpty();
		return str[theSize - 1];
	}
	const char& back()const
	{
		checkEmpty();
		return str[theSize - 1];
	}

	//return copy of the string, including cnt chars from the pointed position.
	String substr(size_t pos = 0, size_t cnt = 0)
	{
		checkPos(pos);
		if (cnt ==0 || theSize - pos < cnt)
			cnt = theSize - pos;
		char *cp = new char[cnt + 2], *ret = cp;
		while (cnt--)
			*cp++ = str[pos++];
		*cp = '\0';
		return ret;
	}

	//convert string into const character string.
	const char* c_str()const { return str; }

	//check whether if the string is empty.
	bool empty()const { return theSize == 0; }

	//return size of the string.
	size_t size()const { return theSize; }

	//return capacity of the class.
	size_t capacity() const { return theCapacity; }

	// return different iterators.
	iterator begin()
	{
		return iterator(str, *this);
	}
	const_iterator begin() const
	{
		return const_iterator(str, *this);
	}
	const_iterator cbegin()const
	{
		return const_iterator(str, *this);
	}
	iterator end()
	{
		return iterator(&str[theSize], *this);
	}
	const_iterator end() const
	{
		return const_iterator(&str[theSize], *this);
	}
	const_iterator cend()const
	{
		return const_iterator(&str[theSize], *this);
	}

	//reset size of the string.
	void resize(size_t newSize, char ch = char())
	{
		while (newSize + 1 > theCapacity)
			reserve(theCapacity * 2 + 2);
		if (newSize > theSize)
		{
			int i = theSize;
			while(i != newSize)
				str[i++] = ch;
			str[i] = '\0';
		}
		theSize = newSize;
	}

	//reset capacity of the string.
	void reserve(size_t newCapacity)
	{
		if (newCapacity > theCapacity)
		{
			//apply for new memory and copy chars, and then release the old memory.
			char *old = str;
			str = new char[newCapacity + 1];
			theCapacity = newCapacity;
			for (int i = 0; i != theSize; ++i)
				str[i] = old[i];
			str[theSize] = '\0';
			delete[]old;
		}
	}

	//append new char at end of the string.
	void push_back(const char c)
	{
		if (theSize + 1 == theCapacity)
			reserve(theCapacity * 2 + 2);
		str[theSize++] = c;
		str[theSize] = '\0';
	}

	//erase the last char;
	void pop_back()
	{
		checkEmpty();
		--theSize;
	}

	//append chars at end of the string.
	String& append(size_t cnt, char c)
	{
		insert(theSize, cnt, c);
		return *this;
	}
	String& append(const String &s)
	{
		insert(theSize, s);
		return *this;
	}
	String& append(const String &s, size_t pos, size_t len)
	{
		insert(theSize, s, pos, len);
		return *this;
	}
	String& append(const char *cp, size_t len = 0)
	{
		insert(theSize, cp, len);
		return *this;
	}
	template <typename Iter>
	String& append(Iter first, Iter last)
	{
		insert(end(), first, last);
		return *this;
	}

	//insert chars at the pointed position given by iterator.
	iterator insert(const_iterator p, char c)
	{
		checkIterator(p);
		auto iter = moveBack(p, 1), ret = iter;
		*iter++ = c;

		return iterator(ret, *this);
	}
	iterator insert(const_iterator p, int cnt, char c)
	{
		checkIterator(p);
		if (cnt == 0)
			return iterator(p.ch, *this);
		
		//move chars backward and insert chars.
		auto iter = moveBack(p, cnt), ret = iter;
		int dist = cnt;
		while (dist--)
			*iter++ = c;
		return iterator(ret, *this);
	}
	iterator insert(const_iterator p, initializer_list<char> lst)
	{
		checkIterator(p);
		if (lst.size() == 0)
			return iterator(p.ch, *this);

		//move chars backward and insert chars.
		auto iter = moveBack(p, lst.size()), ret = iter;
		for (char ch: lst)
			*iter++ = ch;
		return iterator(ret, *this);
	}
	template <typename Iter>
	iterator insert(const_iterator p, Iter beg, Iter end)
	{
		checkIterator(p);
		if (beg == end)
			return iterator(p.ch, *this);

		//caculate number of the chars to insert.
		size_t cnt = 0;
		Iter tmp = beg;
		while (tmp++ != end)
			cnt++;

		//move chars backward and insert chars.
		auto iter = moveBack(p, cnt), ret = iter;
		while (beg != end)
			*iter++ = *beg++;
		return iterator(ret, *this);
	}

	//insert chars at the pointed position given by subsript.
	iterator insert(size_t pos, int cnt, char c)
	{
		checkPos(pos);
		if (cnt == 0)
		{
			if (theSize == 0)
				return iterator(str, *this);
			return iterator(&str[pos], *this);
		}

		size_t n = moveBack(pos, cnt), ret = n;
		while (cnt--)
			str[n++] = c;
		return iterator(&str[ret], *this);
	}
	iterator insert(size_t pos, const String &s)
	{
		checkPos(pos);
		if (s.theSize == 0)
		{
			if (theSize == 0)
				return iterator(str, *this);
			return iterator(&str[pos], *this);
		}

		size_t n = moveBack(pos, s.theSize), ret = n;
		for (char ch : s)
			str[n++] = ch;
		return iterator(&str[ret], *this);
	}
	iterator insert(size_t pos, const String &s, size_t p, size_t len)
	{
		checkPos(pos);
		if (len == 0 || s.theSize == 0)
		{
			if (theSize == 0)
				return iterator(str, *this);
			return iterator(&str[pos], *this);
		}
			

		size_t min = s.theSize - p >= len ? len : s.theSize - p;
		size_t n = moveBack(pos, min), ret = n;
		for (int i = p; i != p + min; ++i)
			str[n++] = s.str[i];
		return iterator(&str[ret], *this);
	}
	iterator insert(size_t pos, const char *cp, size_t len = 0)
	{
		checkPos(pos);
		if (cp == nullptr)
		{
			if (theSize == 0)
				return iterator(str, *this);
			return iterator(&str[pos], *this);
		}

		size_t sz = 0;
		while(cp[sz++] != '\0'){}

		size_t min = sz - 1;
		if (len != 0 && len < min)
			min = len;
		size_t n = moveBack(pos, min), ret = n;
		for (int i = 0; i != min; ++i)
			str[n++] = cp[i];
		return iterator(&str[ret], *this);
	}

	//erase the pointed chars.
	iterator erase(const_iterator p)
	{
		checkEmpty();
		checkIterator(p);
		if(p == cend())
		{
			cout << "out of bounds." << endl;
			exit(1);
		}

		//move chars forward.
		char *cur = p.ch;
		while (cur != &str[theSize - 1])
			*cur++ = *(cur + 1);
		theSize--;
		return iterator(p.ch, *this);
	}
	iterator erase(const_iterator beg, const_iterator end)
	{
		checkEmpty();
		checkIterator(beg);
		checkIterator(end);
		if(beg == end)
			return iterator(end.ch, *this);

		//caculate number of the chars to erase.
		char *lc = beg.ch, *rc = end.ch;
		size_t cnt = rc - lc;

		////move chars forward.
		while (rc != &str[theSize])
			*lc++ = *rc++;
		*lc = '\0';
		theSize -= cnt;

		//the string is empty or the end is cend().
		if (theSize == 0 || end.ch == &str[theSize])
			return iterator(&str[theSize], *this);
		return iterator(beg.ch, *this);
	}
	String& erase(size_t pos, size_t len = 0)
	{
		checkPos(pos);
		if (len == 0 || len > theSize - pos)
			len = theSize - pos;

		//move chars forward.
		if (pos + len < theSize)
		{
			size_t rc = pos + len - 1;
			while (rc != theSize)
				str[++pos] = str[rc++];
		}
		str[pos] = '\0';
		theSize -= len;
		return *this;
	}

	//clear all chars.
	void clear()
	{
		if (str != nullptr)
			delete str;
		str = nullptr;
		theSize = 0;
		theCapacity = 0;
	}

	//comparision operators.
	bool operator==(const char *rhs)const
	{
		if (str == nullptr && rhs == nullptr)return true;
		if (str == nullptr || rhs == nullptr)return false;
		const char *lc = str, *rc = rhs;
		while (*lc != '\0' || *rc != '\0')
			if (*lc++ != *rc++)
				return false;
		return true;
	}
	bool operator!=(const char *rhs)const
	{
		return !(*this == rhs);
	}
	bool operator>(const char *rhs)const
	{
		if (str == nullptr)
			return false;
		if (rhs == nullptr)
			return true;
		const char *lc = str, *rc = rhs;
		while (*lc != '\0' && *rc != '\0')
		{
			if (*lc > *rc)
				return true;
			if (*lc++ < *rc++)
				return false;
		}
		if (*rc == '\0' && *lc != '\0')
			return true;
		return false;
	}
	bool operator>=(const char *rhs)const
	{
		return !(*this < rhs);
	}
	bool operator<(const char *rhs)const
	{
		if (rhs == nullptr)
			return false;
		if (str == nullptr)
			return true;
		const char *lc = str, *rc = rhs;
		while (*lc != '\0' && *rc != '\0')
		{
			if (*lc < *rc)
				return true;
			if (*lc++ > *rc++)
				return false;
		}
		if (*rc != '\0' && *lc == '\0')
			return true;
		return false;
	}
	bool operator<=(const char *rhs)const
	{
		return !(*this > rhs);
	}

	bool operator==(const String &rhs)const
	{
		return *this == rhs.str;
	}
	bool operator!=(const String &rhs)const
	{
		return *this != rhs.str;
	}
	bool operator<(const String &rhs)const
	{
		return *this < rhs.str;
	}
	bool operator<=(const String &rhs)const
	{
		return *this <= rhs.str;
	}
	bool operator>(const String &rhs)const
	{
		return *this > rhs.str;
	}
	bool operator>=(const String &rhs)const
	{
		return *this >= rhs.str;
	}

private:
	size_t theSize = 0;
	size_t theCapacity = 0;
	char *str = nullptr;

	//move chars backward from the char the iterator refers to.
	char* moveBack(const_iterator p, size_t cnt)
	{
		//check whether if the memory is enough.
		int dist = cend() - p + 1;	//have to use type int.
		while (theSize + cnt + 1> theCapacity)
			reserve(2 * theCapacity + 2);

		//empty string.
		if (theSize == 0)
		{
			theSize = cnt;
			return str;
		}

		//move chars backward.
		char *iter = &str[theSize];
		theSize += cnt;	//this sentence must be there.
		while (dist--)
		{
			*(iter + cnt) = *iter;
			--iter;
		}
		return iter + 1;
	}

	//move chars backward from the char the pos refers to.
	size_t moveBack(size_t pos, size_t cnt)
	{
		//check whether if the memory is enough.
		while (theSize + cnt + 1 > theCapacity)
			reserve(2 * theCapacity + 2);

		//empty string.
		if (theSize == 0)
		{
			theSize = cnt;
			return 0;
		}

		//move chars backward.
		int n = theSize;
		theSize += cnt;
		while (n + 1 != pos)
		{
			str[n + cnt] = str[n];
			n--;
		}
		return pos;
	}

	//check whether if the iterator is valid.
	void checkIterator(const_iterator p)const
	{
		if (p.s != this)
		{
			cerr << "Iterator is imcompatible." << endl;
			exit(1);
		}
	}

	//check whether if the pos is out of bounds.
	void checkPos(size_t pos)const
	{
		if (pos > theSize)
		{
			cout << "out of bounds." << endl;
			exit(1);
		}
	}

	//check whether if the string is empty.
	void checkEmpty()const
	{
		if (theSize == 0)
		{
			cout << "The string is empty." << endl;
			exit(1);
		}
	}

	friend void Swap(String &lhs, String &rhs);
};

//overloaded io operators.
istream& operator>>(istream &cin, String &str);
ostream& operator<<(ostream &cout, const String &str);

//swap chars of two strings.
void Swap(String &lhs, String &rhs);

#endif
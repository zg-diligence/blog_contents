#ifndef STRING
#define STRING

#include <iostream>
using namespace std;

class String
{
public:
	//default constructor.
	String() = default;

	//constructor, parameter is a pointer of a character string.
	String(const char *str);

	//copy constructor.
	String(const String &str);

	//move constructor.
	String(String &&str);

	//destructor.
	~String();

	//assignment operator, parameter is pointer of a character string.
	String& operator=(const char *str);

	//copy assignment operator.
	String& operator=(const String &str);

	//move assignment operator.
	String& operator=(String &&str);

	//comparision operators.
	bool operator==(const char *rhs)const;
	bool operator==(const String &rhs)const;
	bool operator!=(const char *rhs)const;
	bool operator!=(const String &rhs)const;

	//overloaded operator +.
	String operator+(const char c)const;
	String operator+(const char *rhs)const;
	String operator+(const String &)const;

	//overloaded operator +=.
	String& operator +=(const char *);
	String& operator +=(const char);
	String& operator +=(const String &);

	//reset the string and release the memory.
	void makeEmpty();

	//return size of the string.
	int size()const { return _size; }

	//check whether if the string is empty.
	bool empty()const { return _size == 0; }

	//return reference of the character at the given position.
	char& operator[](int n)const { return chars[n]; }

	//convert string into const character string.
	const char* c_str()const { return chars; }

private:
	int _size = 0;
	char *chars = nullptr;
};

//overloaded io operators.
istream& operator>>(istream &cin, String &str);
ostream& operator<<(ostream &cout, const String &str);

#endif
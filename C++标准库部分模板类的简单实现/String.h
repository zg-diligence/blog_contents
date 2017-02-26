#ifndef STRING
#define STRING

#include <iostream>
using namespace std;

class String
{
public:
	String() :_size(0), _string(nullptr) {}
	String(const char *);
	String(const String &);
	~String();

	String& operator=(const char *);
	String& operator=(const String &);

	bool operator==(const char *rhs)const;
	bool operator==(const String &rhs)const;
	bool operator!=(const char *rhs)const;
	bool operator!=(const String &rhs)const;

	String operator+(const char *rhs)const;
	String operator+(const char c)const;
	String operator+(const String &)const;
	String &operator +=(const char *);
	String &operator +=(const char);
	String &operator +=(const String &);

	void makeEmpty();
	int size()const { return _size; }
	bool empty()const { return _size == 0; }
	char &operator[](int n)const { return _string[n]; }
	char* c_str()const { return _string; }
private:
	int _size;         //Size of the string;
	char *_string;     //Point to the first char fof the string;
};

size_t strSize(const char *s);
void copyStr(char *destination, const char *source);
istream& operator >> (istream &cin, String &str);
ostream& operator<<(ostream &cout, const String &str);

#endif
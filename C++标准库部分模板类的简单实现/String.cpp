#include "String.h"

String::String(const char *str):_size(0),_string(nullptr)
{
	if (str != nullptr)
		operator=(str);
}
String::String(const String &str):_size(0), _string(nullptr)
{
	if (str._size != 0)
		operator=(str._string);
}
String::~String()
{
	makeEmpty();
}

String& String::operator=(const char *rhs)
{
	if (_string != rhs)           //self-assignment;
	{
		makeEmpty();
		_size = strSize(rhs);
		_string = new char[4096];//Pay attention to this!!
		copyStr(_string, rhs);
	}
	return *this;
}
String& String::operator=(const String &rhs)
{
	return operator=(rhs._string);
}

bool String::operator==(const char *rhs)const
{
	if (_string == nullptr && rhs == nullptr)return true;
	if (_string == nullptr || rhs == nullptr)return false;
	char *ls = _string, *rs = const_cast<char*>(rhs);
	while (*ls != '\0'&& *rs != '\0')
		if (*ls++ != *rs++)return false;
	return true;
}
bool String::operator!=(const char *rhs)const
{
	return !(*this == rhs);
}
bool String::operator==(const String &rhs)const
{
	if (_size != rhs._size)return false;
	if (_size == 0 && rhs._size == 0)return true;
	char *ls = _string, *rs = rhs._string;
	while (*ls != '\0')
		if (*ls++ != *rs++)return false;
	return true;
}
bool String::operator!=(const String &rhs)const
{
	return !(*this == rhs);
}

String String::operator+(const char *rhs)const
{
	String ret_str = *this;
	ret_str += rhs;
	return ret_str;
}
String String::operator+(const char c)const
{
	String ret_str = *this;
	ret_str += c;
	return ret_str;
}
String String::operator+(const String &rhs)const
{
	String ret_str = *this;
	ret_str += rhs;
	return ret_str;
}

String& String::operator +=(const char *rhs)
{
	if (_string != rhs)
	{
		if (rhs == nullptr)return *this;
		if (_string == nullptr)return *this = rhs;

		int i = 0;
		char *tail = _string + _size;
		for (; *rhs != '\0'; ++i)
			*tail++ = *rhs++;
		*tail = '\0';
		_size += i;
	}
	return *this;
}
String& String::operator +=(const char c)
{
	char *rc = new char[2];
	*rc = c; *(rc + 1) = '\0';
	operator+=(rc);
	return *this;
}
String& String::operator +=(const String &rhs)
{
	return operator+=(rhs._string);
}

void String::makeEmpty()
{
	if (_string != nullptr)
		delete _string;
	_size = 0;
}

size_t strSize(const char *s)
{
	if (s == nullptr)return 0;
	int str_size = 0;
	while (*s++ != '\0')
		str_size++;
	return str_size;
}
void copyStr(char *destination, const char *source)
{
	if (source == nullptr)return;
	while (*source != '\0')
		*destination++ = *source++;
	*destination = '\0';
}
istream& operator>>(istream &cin, String &str)
{
	char pre_str[4096];
	int i = 0;
	char c = getchar();
	while (c != ' ' && c != '\n')
	{
		pre_str[i++] = c;
		c = getchar();
	}
	pre_str[i] = '\0';
	str = pre_str;
	return cin;
}
ostream& operator<<(ostream &cout, const String &str)
{
	if (str.size() == 0)return cout;
	return cout << str.c_str();
}
#include "String.h"

String::String(const char *str)
{
	if (str != nullptr)
		operator=(str);
}

String::String(const String &str)
{
	if (str._size != 0)
		operator=(str);
}

String::String(String &&str)
{
	if (str._size != 0)
		operator=(std::move(str));
}

String::~String()
{
	makeEmpty();
}

String& String::operator=(const char *rhs)
{
	if (chars != rhs)	//check self-assignment.
	{
		makeEmpty();
		if (rhs != nullptr)
		{
			chars = new char[4096]; //set limit of the string size.
			char *dest = chars;
			const char *src = rhs;
			while (*src != '\0')
			{
				*dest++ = *src++;
				_size++;
			}
			*dest = '\0';
		}
	}
	return *this;
}

String& String::operator=(const String &rhs)
{
	return operator=(rhs.chars);
}

String& String::operator=(String &&str)
{
	if (chars != str.chars)
	{
		makeEmpty();
		_size = str._size;
		chars = str.chars;
		str._size = 0;
		str.chars = nullptr;
	}
	return *this;
}

bool String::operator==(const char *rhs)const
{
	if (chars == nullptr && rhs == nullptr)return true;
	if (chars == nullptr || rhs == nullptr)return false;
	const char *ls = chars;
	while (*ls != '\0' || *rhs != '\0')
		if (*ls++ != *rhs++)return false;
	return true;
}

bool String::operator!=(const char *rhs)const
{
	return !(*this == rhs);
}

bool String::operator==(const String &rhs)const
{
	if (_size != rhs._size)return false;
	return operator==(rhs.chars);
}

bool String::operator!=(const String &rhs)const
{
	return !(*this == rhs);
}

String String::operator+(const char c)const
{
	String src = *this;
	return src += c;
}

String String::operator+(const char *rhs)const
{
	String src = *this;
	return src += rhs;
}

String String::operator+(const String &rhs)const
{
	String src = *this;
	return src += rhs;
}

String& String::operator +=(const char c)
{
	if (chars == nullptr)
		chars = new char[4096];
	*(chars + _size++) = c;
	*(chars + _size) = '\0';
	return *this;
}

String& String::operator +=(const char *rhs)
{
	if (rhs == nullptr)return *this;
	if (chars == nullptr)return *this = rhs;

	char *dest = chars + _size;
	if (chars == rhs)	//self-assignment.
	{
		int tmp = _size;
		while (tmp--)
			*dest++ = *rhs++;
		_size *= 2;
	}
	else
	{
		while (*rhs != '\0')
		{
			*dest++ = *rhs++;
			_size++;
		}
	}
	*dest = '\0';
	return *this;
}

String& String::operator +=(const String &rhs)
{
	return operator+=(rhs.chars);
}

void String::makeEmpty()
{
	if (chars != nullptr)
		delete chars;
	_size = 0;
	chars = nullptr;
}

istream& operator>>(istream &cin, String &str)
{
	char *tmp = new char[4096], *dest = tmp;
	char c = getchar();
	while (c != ' ' && c != '\n')
	{
		*dest++ = c;
		c = getchar();
	}
	*dest = '\0';
	str = tmp;
	return cin;
}

ostream& operator<<(ostream &cout, const String &str)
{
	if (str.size() == 0)return cout;
	return cout << str.c_str();
}
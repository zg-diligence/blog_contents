#include "String.h"

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

void Swap(String &lhs, String &rhs)
{
    int cnt = lhs.theSize;
    lhs.theSize = rhs.theSize;
    rhs.theSize = cnt;

    cnt = lhs.theCapacity;
    lhs.theCapacity = rhs.theCapacity;
    rhs.theCapacity = cnt;

    char *tmp = lhs.str;
    lhs.str = rhs.str;
    rhs.str = tmp;
}
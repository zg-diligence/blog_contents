#ifndef ITERATOR
#define ITERATOR

class base_iterator
{
public:
    virtual bool operator==(const base_iterator &rhs) = 0;
    virtual bool operator!=(const base_iterator &rhs) = 0;
private:
    int num;
};
#endif // !


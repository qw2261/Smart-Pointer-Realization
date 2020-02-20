#include <iostream>
using namespace std;

template<typename T>
class Auto_ptr3
{
public:
    Auto_ptr3(T* ptr = nullptr):m_ptr(ptr){}

    Auto_ptr3(const Auto_ptr3& rhs) = delete;

    Auto_ptr3(Auto_ptr3&& rhs):m_ptr(rhs.m_ptr)
    {
        rhs.m_ptr = nullptr;
    }

    Auto_ptr3& operator=(const Auto_ptr3& rhs) = delete;

    Auto_ptr3& operator=(Auto_ptr3&& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        swap(m_ptr, rhs.m_ptr);
        return *this;
    }

    virtual ~Auto_ptr3()
    {
        delete m_ptr;
    }

    T& operator*(){return *m_ptr;}
    T* operator->(){return m_ptr;}

    bool isNull() const {return m_ptr == nullptr;}

private:
    T* m_ptr;
};
#include <iostream>
using namespace std;

template<typename T>
class Auto_ptr3
{
public:
    Auto_ptr3(T* ptr = nullptr) : m_ptr(ptr){}

    Auto_ptr3(const Auto_ptr3& rhs) = delete;

    


private:
    T* m_ptr;
};
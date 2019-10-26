#include <iostream>

template<typename T>
class Auto_ptr1
{
public:
    Auto_ptr1(T* ptr = nullptr):m_ptr{ptr}
    {}
    
    virtual ~Auto_ptr1()
    {
        delete m_ptr;
    }

    T& operator*(){return *m_ptr;}
    T* operator->(){return m_ptr;}

private:
    T* m_ptr;
};

class Resource
{
public:
  Resource(){std::cout << "Resource Acquired!" << std::endl;}
  virtual ~Resource(){std::cout << "Resource Destoryed!" << std::endl;}
};

int main()
{
  {
    Auto_ptr1<Resource> res(new Resource);
  }
  
  
  std::cin.ignore(10);
  return 0;
}

// The code below will fail
/*
int main()
{
  {
    Auto_ptr1<Resource> res1(new Resource);
    Auto_ptr1<Resource> res2(res1);
  }

  std::cin.ignore(10);
  return 0;
}
*/

// The code below will also fail
/*
void passByValue(Auto_ptr1<Resource> res){}

int main()
{
  {
    Auto_ptr1<Resource> res1(new Resource);
    passByValue(res1);
  }

  std::cin.ignore(10);
  return 0;
}
*/
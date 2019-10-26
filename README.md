# C++ Smart Pointer Realization

## Preview

Memory managenment is a general source of bugs in C++. Actually the smart pointer is designed to keep the memory clean automatically.

```c++
void simpleFunction()
{
  Resource* ptr = new Resource;
  
  delete ptr;
}
```

The function can new a dynmic memory and use this part to store resource, but there will be another situation like below:

```c++
void simpleFunction()
{
  Resource* ptr = new Resource;
  
  int x;
  std::cout << "Enter an integer: ";
  std::cin >> x;
  
  if (x == 0) return;
  if (x < 0) throw;
  
  // This part will not be runnable
  delete ptr;
}
```

So, we can design a simple smart pointer:

```c++
// Auto_ptr1.cpp

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

```

The output is shown below:

```bash
wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ g++ -std=c++11 Auto_ptr1.cpp -o main
wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ ./main 
Resource Acquired!
Resource Destoryed!
```

This simple smart pointer can store the dynamic memory and when we leave the working space, it will call destructor to destroy the pointer.

However, it might fail when we consider the case:

```c++
int main()
{
  {
    Auto_ptr1<Resource> res1(new Resource);
    Auto_ptr1<Resource> res2(res1);
  }

  std::cin.ignore(10);
  return 0;
}
```

This happens because the default copy method is shallow copy and the memory will be destoryed for more than once.

Similarly,

```c++
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
```

So, to maintain the running safely, we should reconstruct the copy method.

The revised smart pointer is shown below:

```c++
#include <iostream>
using namespace std;

template<typename T>
class Auto_ptr2
{
public:
    Auto_ptr2(T* ptr = nullptr):m_ptr{ptr}{}

    virtual ~Auto_ptr2()
    {
        delete m_ptr;
    }

    Auto_ptr2(Auto_ptr2& rhs)
    {
        m_ptr = rhs.mptr;
        rhs.mptr = nullptr;
    }

    Auto_ptr2& operator=(Auto_ptr2& rhs)
    {
        if (&rhs == this) return *this;
        delete m_ptr;
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
        return *this;
    }

    T& operator*(){return *m_ptr;}
    T* operator->(){return m_ptr;}
    bool isNull() const {return m_ptr == nullptr;}
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
      Auto_ptr2<Resource> res1(new Resource);
      Auto_ptr2<Resource> res2;

      cout << "res1 is " << (res1.isNull()? "null\n" : "not null\n");
      cout << "res2 is " << (res2.isNull()? "null\n" : "not null\n");

      res2 = res1;

      cout << "Ownership transferred\n";

      cout << "res1 is " << (res1.isNull()? "null\n" : "not null\n");
      cout << "res2 is " << (res2.isNull()? "null\n" : "not null\n");

    }
    
    cin.ignore(10);
    return 0;
}
```

The output is shown below:

```bash
wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ g++ -std=c++11 Auto_ptr2.cpp -o main
wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ ./main 
Resource Acquired!
res1 is not null
res2 is null
Ownership transferred
res1 is null
res2 is not null
Resource Destoryed!
```

This method can do the memory management, but there are some drawbacks:

1. $=$ is not real assignment but ownership transfer. It makes the original pointer empty.
2. It uses ```delete m_ptr;```, which cannot work for variable array (vector) but fixed-size array.

Based on the considerations above, we can use C++11 new features to revise our code:


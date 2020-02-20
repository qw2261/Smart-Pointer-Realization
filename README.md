# C++ Smart Pointer Realization

## Preview

Memory managenment is a general origin of bugs in C++. Actually the smart pointer is designed to keep the memory clean automatically.

```c++
void simpleFunction()
{
  Resource* ptr = new Resource; // Resource here is a class or struct
  
  // Can use this ptr here
  
  delete ptr;
}
```

The function can new a dynmic memory and use this part to store resource, but there will be another unsafe situation like below:

```c++
void simpleFunction()
{
  Resource* ptr = new Resource;
  
  int x;
  std::cout << "Enter an integer: ";
  std::cin >> x;
  
  if (x == 0) return;
  if (x < 0) throw;
  
  // This part will not be processed
  delete ptr;
}
```

In piece of code above, the if part will return or throw to end the program and pointer cannot be safely released. In summary, there is no automated mechanism to manage the memory and address. But in our opinion, we can design a class and have a memory release action in destructor.

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
        delete m_ptr; // Delete the pointer in destructor
    }

    T& operator*(){return *m_ptr;} // Rewrite * to return a reference
    T* operator->(){return m_ptr;} // Rewrite -> to return a pointer

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

This happens because the default copy method is shallow copy (only with a default copy constructor) and the memory will be destoryed for more than once (res1 and res2).

Similarly,

```c++
void passByValue(Auto_ptr1<Resource> res)
{}

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

This happens because of the same reason that there is only a default copy constructor.

So, to guarantee the program running safely, we should revise the copy method and "=".

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
        m_ptr = rhs.m_ptr;
        rhs.m_ptr = nullptr;
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

<<<<<<< HEAD
1. ```=​``` is not real assignment but ownership transfer. It makes the original pointer empty. When user uses the empty pointer, the program will crash down.
2. It uses ```delete m_ptr;```, which cannot work for variable array (vector) but fixed-size array. So, it is incompatible with STL.
=======
1. <a href="https://www.codecogs.com/eqnedit.php?latex==" target="_blank"><img src="https://latex.codecogs.com/gif.latex?=" title="=" /></a> is not real assignment but ownership transfer. It makes the original pointer empty.
2. It uses ```delete m_ptr;```, which cannot work for variable array (vector) but fixed-size array.
>>>>>>> 8081e56d1679546dc91a27887c5742e6bb21254f

Based on the considerations above, we can use C++11 new features to revise our code:

```c++
#include <iostream>
using namespace std;

template<typename T>
class Auto_ptr3
{
public:
    Auto_ptr3(T* ptr = nullptr) : m_ptr(ptr){}

    Auto_ptr3(const Auto_ptr3& rhs) = delete;

    Auto_ptr3(Auto_ptr3&& rhs) : m_ptr(rhs.m_ptr)
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
```

In this piece of code, the function overloading and operator overloading is not allowed but "move" is allowed. So it can only transfer right handside value.



In C++11, there are 4 smart pointers: std::auto_ptr (Don't use), std::unique_ptr (similar to Auto_ptr3), std::shared_ptr abd std::weak_ptr.



## ```std::unique_ptr```

```std::unique_ptr``` is a replacement of ```std::auto_ptr``` but is appliable to shared memory mangement with multiple objects but only one.

```c++
#include <iostream>

class Fraction
{
private:
    int m_numerator = 0;
    int m_denominator = 1;

public:
    Fraction(int numerator = 0, int denominator = 1):m_numerator(numerator), m_denominator(denominator)
    {

    }  

    friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
    {
        out << f1.m_denominator << "/" << f1.m_denominator;
        return out;
    }

};

int main()
{
    std::unique_ptr<Fraction> f1{ new Fraction{3, 5} };
    std::cout << *f1 << '\n'; // output: 3/5

    std::unique_ptr<Fraction> f2; // initialize to nullptr

    // f2 = f1 is not allowed since left assignment is not allowed
    f2 = std::move(f1); // f1 transfers ownership to f2, f1 becomes nullptr

    // C++14 can use make_unique
    // auto f3 = std::make_unique<Fraction>(2, 7);
    // std::cout << *f3 << '\n'; // output: 2/7

    // Deal with array but not suggest to do, use vector, array
    // auto f4 = std::make_unique<Fraction[]>(4);
    // std::cout << f4[0] << '\n'; // output: 0/1

    std::cin.ignore(10);
    return 0;
}
```

If compiler supports ```make_unique```, try the best to use this method. If not support, we can write a simple code as a replacement:

```c++
// Unable to deal with arrays
template<typename T, typename ... Ts>
std::unique_ptr<T> make_unique(Ts ... args)
{
  return std::unique_ptr<T> {new T{ std::forward<Ts>(args) ... }}
}
```

Copying the ```unique_ptr``` requires "move":

```c++
#include <iostream>

using namespace std;

class Resource
{
public:
    Resource(){cout << "Resource acquired!" << endl;}
    virtual ~Resource(){cout << "Resource destroyed!" << endl;}

    friend std::ostream& operator<<(std::ostream& out, const Resource &res)
    {
        out << "I am a resource" << endl;
        return out;
    }

};

void useResource(const std::unique_ptr<Resource>& res)
{
    if (res)
    {
        cout << *res;
    }
}

int main()
{
    {
        std::unique_ptr<Resource> ptr{new Resource()};
        useResource(ptr);
        cout << "Ending" << endl;
    }

    cin.ignore(10);
    return 0;
}
```

The output is shown below:

```bash
(base) wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ g++ -std=c++11 Ex_unique_ptr_move.cpp -o main
(base) wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ ./main 
Resource acquired!
I am a resource
Ending
Resource destroyed!
```

As you can see, use "reference" in the function will not affect the ownership transfer.

```c++
void takeOwnerShip(std::unique_ptr<Resource>&& res) // 也可以用 std::unique_ptr<Resource> res
{
    if (res)
    {
        cout << *res;
    }
}

int main()
{
  {
    auto ptr = std::make_unique<Resource>();
    // takeOwnerShip(ptr); // Ilegal
    takeOwnerShip(std::move(ptr)); // Must pass r-value
    cout << "Ending" << endl;
  }
  // output
  // Resource acquired
  // I am a resource
  // Resource destroyed
  // Ending
  cin.ignore(10);
	return 0;
}
```

The common errors of ```std::unique_ptr```:

```c++
// Multiple initializing a single resource
Resource *res = new Resource;
std::unique_ptr<Resource> res1(res);
std::unique_ptr<Resource> res2(res);

// Mix common pointer and unique pointer
Resource *res = new Resource;
std::unique_ptr<Resource> res1(res);
delete res;

```

## ```std::shared_ptr```

```std::shared_ptr``` is similar to ```std::unique_ptr```. There is ```make_shared()``` method in ```C++11```. The main difference between them is ```shared_ptr``` has ```count``` to refer the real object of smart pointer. 

```c++
#include <iostream>

using namespace std;

class Resource
{
public:
    Resource() { cout << "Resource acquired!" << endl; }
    virtual ~Resource() { cout << "Resource destroyed!" << endl; }

    friend std::ostream &operator<<(std::ostream &out, const Resource &res)
    {
        out << "I am a resource" << endl;
        return out;
    }
};

int main()
{
    Resource *res = new Resource;
    std::shared_ptr<Resource> ptr1{res};
    cout << ptr1.use_count() << endl; // output: 1
    {

        std::shared_ptr<Resource> ptr2{res}; // Initialize memory for multiple times

        cout << ptr1.use_count() << endl; // output: 1
        cout << ptr2.use_count() << endl; // output: 1
    }
    // ptr2 and ptr3 are destructed, output：Resource destroyed
    cout << ptr1.use_count() << endl; // output: 1

    cin.ignore(10);
    return 0;
}
```

## ```std::weak_ptr```

To solve a problem in ```shared_ptr```:

```c++
#include <iostream>

using namespace std;

class Person
{
public:
    Person(const string &name) : m_name{name}
    {
        cout << m_name << " created" << endl;
    }

    virtual ~Person()
    {
        cout << m_name << " destoryed" << endl;
    }

    friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
    {
        if (!p1 || !p2)
        {
            return false;
        }
        p1->m_partner = p2;
        p2->m_partner = p1;

        cout << p1->m_name << " is now partenered with " << p2->m_name << endl;
        return true;
    }

private:
    string m_name;
    std::shared_ptr<Person> m_partner;
};

int main()
{
    {
        auto p1 = std::make_shared<Person>("Lucy");
        auto p2 = std::make_shared<Person>("Ricky");
        partnerUp(p1, p2);
    }

    cin.ignore(10);
    return 0;
}
```

The output is abnormal:

```bash
(base) wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ g++ -std=c++11 Ex_shared_problem.cpp -o main
(base) wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ ./main 
Lucy created
Ricky created
Lucy is now partenered with Ricky
```

So we need a "outsider", which is the reason we should have ```weak_ptr```.

```c++
#include <iostream>

using namespace std;

class Person
{
public:
    Person(const string& name):
        m_name{name}
    {
        cout << m_name << " created" << endl;
    }

    virtual ~Person()
    {
        cout << m_name << " destoryed" << endl;
    }

    friend bool partnerUp(std::shared_ptr<Person>& p1, std::shared_ptr<Person>& p2)
    {
        if (!p1 || !p2)
        {
            return false;
        }

        p1->m_partner = p2;  
        p2->m_partner = p1;

        cout << p1->m_name << " is now partenered with " << p2->m_name << endl;
        return true;
    }

private:
    string m_name;    
    std::weak_ptr<Person> m_partner;
};

int main()
{
    {
        auto p1 = std::make_shared<Person>("Lucy");
        auto p2 = std::make_shared<Person>("Ricky");
        partnerUp(p1, p2);
    }
    cin.ignore(10);
    return 0;
}
```

The output is here:

```bash
(base) wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ g++ -std=c++11 Ex_shared_problem_solution.cpp -o main
(base) wangqideMacBook-Pro:C-SmartPointerRealization wangqi$ ./main 
Lucy created
Ricky created
Lucy is now partenered with Ricky
Ricky destoryed
Lucy destoryed
```


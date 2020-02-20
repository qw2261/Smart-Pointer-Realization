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
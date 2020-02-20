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
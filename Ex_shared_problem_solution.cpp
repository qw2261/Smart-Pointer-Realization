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
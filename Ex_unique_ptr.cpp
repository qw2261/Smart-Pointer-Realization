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
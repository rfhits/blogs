# include <iostream>
class Complex {
private:
    double re, im;
public:
    Complex(double r, double i): re(r), im(i) {}
    double real() const
    {
        return re;
    }
};

int main(void)
{
    Complex c(1,2);
    std::cout << c.real() << std::endl;
    // std::cout << real(c) << std::endl;
}
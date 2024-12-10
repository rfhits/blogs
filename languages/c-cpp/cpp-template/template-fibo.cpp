template <int x> constexpr int fib() { return fib<x - 1>() + fib<x - 2>(); }

template <> constexpr int fib<0>() { return 1; }

template <> constexpr int fib<1>() { return 1; }

template <int x> struct fib_t {
    static int const value = fib_t<x - 1>::value + fib_t<x - 2>::value;
};

template <> struct fib_t<0> {
    static int const value = 1;
};

template <> struct fib_t<1> {
    static int const value = 1;
};

#include <iostream>

int main() {
    std::cout << fib<5>() << std::endl;
    std::cout << fib_t<6>::value << std::endl;
    return 0;
}
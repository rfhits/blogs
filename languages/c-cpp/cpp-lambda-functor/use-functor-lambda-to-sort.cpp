#include <algorithm>
#include <vector>
#include <iostream>
using std::vector;

bool less_function(int const &a, int const &b)
{
    return a < b;
}

struct Less
{
    bool operator()(int const &a, int const &b) const
    {
        return a < b;
    }
};

auto less_lambda = [](int const &a, int const &b)
{
    return a < b;
};

Less less_functor{};

int main()
{
    vector<int> v = {1, 3, 5, 7, 2, 4, 6};

    std::sort(v.begin(), v.end(), less_lambda);
    std::sort(v.begin(), v.end(), less_function);
    std::sort(v.begin(), v.end(), less_functor);

    for (auto &i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
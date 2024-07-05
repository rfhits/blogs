#include <algorithm>
#include <array>
#include <assert.h>
#include <cstring>
#include <deque>
#include <cassert>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::cerr;
using std::cin;
using std::clog;
using std::cout;
using std::map;
using std::set;
using std::shared_ptr;
using namespace std::placeholders;
using std::array;
using std::bind;
using std::deque;
using std::endl;
using std::forward_list;
using std::getline;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::list;
using std::ofstream;
using std::ostream;
using std::pair;
using std::queue;
using std::stack;
using std::stof;
using std::stoi;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using LL = long long;

struct add_x
{
    add_x(int val) : x(val) {} // Constructor
    int operator()(int y) const { return x + y; }

private:
    int x;
};

struct output {
    void operator()(const int & x) {
        std::cout << x << " ";
    }
};

int main()
{
    vector<int> x{0, 1, 2, 3};
    vector<int> y(x.size());
    // Now you can use it like this:
    add_x add_42(42);            // create an instance of the functor class
    int i = add_42(8);           // and "call" it
    std::cout << i << std::endl; // 50

    // Pass a functor to std::transform, which calls the functor on every element
    // in the input sequence, and stores the result to the output sequence
    std::transform(x.begin(), x.end(), y.begin(), add_42); // y[i] = x[i] + 42
    std::for_each(x.begin(), x.end(), output()); 

    for (auto &i : y)
    {
        std::cout << i << " ";
    }
    return 0;
}
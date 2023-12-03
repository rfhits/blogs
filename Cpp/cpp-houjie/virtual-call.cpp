#include <algorithm>
#include <array>
#include <assert.h>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
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
using std::cout;
using std::clog;
using std::map;
using std::set;
using namespace std::placeholders;
using std::array;
using std::bind;
using std::deque;
using std::endl;
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
class Base {
public:
    int bx = 0;
    virtual void print() {
        std::cout << "bx: " << bx << std::endl;
    };
};

class Derived : public Base {
    int dx = 1;
    void print() {
        std::cout << "dx: " << dx << std::endl;
    }
};
int main()
{
    Base* pb = new Derived();
    pb->print(); // dx: 1
    delete pb;
    return 0;
}
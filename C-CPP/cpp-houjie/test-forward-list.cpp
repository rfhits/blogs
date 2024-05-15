#include <algorithm>
#include <array>
#include <assert.h>
#include <cstring>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <forward_list>
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
forward_list<int> l;
int main()
{
    
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    for (auto &i : l)
    {
        std::cout << i << std::endl;
    }
    std::cout << *l.begin() << std::endl;
    l.pop_front();
    std::cout << *l.begin() << std::endl;
    return 0;

}
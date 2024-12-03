#include <atomic>
#include <iostream>
using std::atomic;
int main() {

  atomic<int> x;
  x.store(1);

  auto old = x.load();
  std::cout << old << std::endl;
  x.store(2);
  x.compare_exchange_weak(old, 3);
  std::cout << old << std::endl;
  return 0;
}
#include <stdatomic.h>
#include <stdio.h>

_Atomic int count = 0;

int main() {
  int old = atomic_load(&count);
  old += 1;
  int update = 100;
  int cas_res = atomic_compare_exchange_weak(&count, &old, update); // must fail
  printf("cas_res: %d, count: %d, old: %d\n", cas_res, count, old);
  return 0;
}
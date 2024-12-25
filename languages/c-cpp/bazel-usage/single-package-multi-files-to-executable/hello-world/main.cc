#include <iostream>
#include "hello-world/echo.h"
using namespace std;
int main() {
    std::cout << "hello world" << std::endl;
    echo("echo hello world");
    return 0;
}
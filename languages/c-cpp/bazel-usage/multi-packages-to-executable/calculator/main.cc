#include <iostream>
#include "echo.h"
#include "math/math.h"
using namespace std;

int main()
{
    std::cout << "hello world" << std::endl;
    echo("echo(hello world)");
    int x = add(1, 2);
    cout << "add(1,2): " << x << endl;
    return 0;
}
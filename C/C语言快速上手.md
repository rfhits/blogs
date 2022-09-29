C语言快速上手

[toc]

本文旨在快速回顾C语言语法知识

# 数据类型

char，short，int，long，long long

参考[C数据类型](https://www.runoob.com/cprogramming/c-data-types.html)，不需要死记硬背，但是 `sizeof()`这个函数一定得知道。

1. 32位和64位程序上，int的长度不一样
2. Windows和Linux上，对64位程序，long的长度不一样：同一份代码，不同平台编译后运行结果不同
3. long如果不够大，就用long long这个数据类型

```c
#include<stdio.h>

int main() {
	printf("size of long: %d", sizeof(long));
	return 0;
}
```

这段代码在64位的Windows和Linux下，编译运行的结果是不同的。

# 运算符

算术运算符、逻辑运算符和关系运算符

我认为比较重要的就是通过强制类型转换使得除法的商为小数，这个东西比较容易忘记。

```c
#include<stdio.h>
int main() {
    int a = 20, b = 3;
    double q;
    q = a/b;
    printf("%f\n", q); // q is 6
    q = (double)a/b;
    printf("%f\n", q); // q is 6.6...7 
    return 0;
}
```

# 输入输出

学完以后，大概率只能记住 `%d`和 `%s`了，实际的输出格式如下：

`%[flags][width][.precision][length]specifier`

参考[printf-cpp-reference](https://cplusplus.com/reference/cstdio/printf/)

# C++ 模板

-   created: 2024-03-24T20:24+08:00
-   modified: 2024-12-10T11:36+08:00
-   published: 2024-11-17T16:37+08:00
-   categories: c-cpp

[toc]

> 在模板中，我们可以声明三种类型的形参（Parameters），分别是：
>
> 1. 非类型模板形参（Non-type Template Parameters）
> 2. 类型模板形参（Type Template Parameters）
> 3. 模板模板形参（Template Template Parameters）
>
> ——[C\+\+模板元编程（零）：前言 \- 知乎](https://zhuanlan.zhihu.com/p/378355217) 的笔记，但是原文有些地方还是不太明白。

也就是说，定义模板的时候，参数不止可以提供类型 `typename T`，还可以提供 `int` 之类的，如：

```cpp
#include <iostream>

template <int x>
void print_template_arg(void) {
  std::cout << x << std::endl;
}

int main() {
  print_template_arg<5>();
  return 0;
}
```

## 特化

特化的意思是：当传入的类型参数为 T 时候，我希望有一个自己重写的实现，如：

```cpp
#include <iostream>
template <typename T>
void print(T x)
{
    std::cout << x << std::endl;
}

template <>
void print<int>(int x)
{
    std::cout << "print<int>(" << x << ")" << std::endl;
}

template <>
void print(double x)
{
    std::cout << "print<double>(" << x << ")" << std::endl;
}

int main()
{
    print(1);
    print(1.1);
}
```

偏特化意思是，如果模板有多个类型参数，我希望当部分参数为 T1、T2……时候，有一个自己的实现。

## 类模板

类模板基础的用法略过，类模板 C++11 不支持参数推导，所以使用时候一定要提供模板参数。
它的神奇之处在于：

1. 支持偏特化
2. 支持引用的偏特化，引用类型偏特化的语法也是独树一格：

```cpp
// example from https://zhuanlan.zhihu.com/p/384826036
template <typename T> struct is_reference      { static constexpr bool value = false; };    // #1
template <typename T> struct is_reference<T&>  { static constexpr bool value = true; };     // #2
template <typename T> struct is_reference<T&&> { static constexpr bool value = true; };     // #3

std::cout << is_reference<int>::value << std::endl;    // 0
std::cout << is_reference<int&>::value << std::endl;   // 1
std::cout << is_reference<int&&>::value << std::endl;  // 1
```

更多用法参考：[C\+\+模板元编程（三）：从简单案例中学习 \- 知乎](https://zhuanlan.zhihu.com/p/384826036)

## 函数模板

首先要破除一些关于函数模板的迷思：

1. 函数模板的模板参数虽然可以通过函数的参数推导，但是也可以直接指定，像 `foo<int>()` 就是在直接指定，并非语法错误。
2. 函数模板只支持全特化。  
   所以特化的模板一定要依赖于主模板，语法是把 `template<typename T, ...>` 中尖括号 `<>` 中**所有的模板参数**都列出来，一个都不能少。

函数和函数模板可以放在一起重载，看起来很烧脑，
那么发生了一次函数调用，如何确定使用哪一个函数呢？按照我的理解：

1. 函数调用时候，如果提供了模板参数列表，则显式实例化，如 `foo<int>(1)` 提供了 `<int>`，
    1. 先查找是否有特化的函数模板，类似这个格式：`template<> foo<int>()`
    2. 没有就找函数主模板
2. 如果没有提供模板的参数列表，则说明需要根据函数参数推导类型
3. 从全特化的模板中查找符合条件的
4. 找不到就用主模板

严谨表述参考：[C\+\+模板元编程（四）：深入模板 \- 知乎](https://zhuanlan.zhihu.com/p/390783543)

```CPP
template<typename T>
void bar(T v) {
    std::cout << "1 template param" << std::endl;
    return;
}

template<>
void bar(int v) {
    std::cout << "1 template param specialization" << std::endl;
    return;
}

template<typename T, typename U>
void bar(T v) {
    std::cout << "2 template param" << std::endl;
    return;
}

int main()
{
    bar<int, float>(1);     // 2 template param
    bar<float>(1);          // 1 template param
    bar(1);                 // 1 template param specialization
    return 0;
}
```

`bar<int, float>` 显式提供了模板参数，故实例化第三个 `bar`
`bar<float>` 也显式提供了模板参数，确定类型 `T` 为 float，并且没有找到对应的特换版本的函数，故实例化第一个
`bar(1)` 只提供了一个函数参数，只需要检查函数签名，对于第三个模板，无法推导出类型 `U`；使用第二个模板。

[函数模板 \| 现代 C\+\+ 模板教程](https://mq-b.github.io/Modern-Cpp-templates-tutorial/md/%E7%AC%AC%E4%B8%80%E9%83%A8%E5%88%86-%E5%9F%BA%E7%A1%80%E7%9F%A5%E8%AF%86/01%E5%87%BD%E6%95%B0%E6%A8%A1%E6%9D%BF#%E5%8F%AF%E5%8F%98%E5%8F%82%E6%95%B0%E6%A8%A1%E6%9D%BF)

## 形参包

[形参包 \(C\+\+11 起\) \- cppreference\.com](https://zh.cppreference.com/w/cpp/language/parameter_pack)

-   语法：我的记忆方法是，把 `...` 写在类型和包名中间，这样可以避免忘记写 `...`
    ```cpp
    template<typename ...Args>
    print(const Args& ...args) {
      int _[]{ (std::cout << args << ' ' ,0)... };
    }
    ```
-   包展开：和中文语法下省略号的用法一致

## Questions

### 我记不住模板的语法，尤其是偏特化的语法，怎么办？

以 `remove_reference` 为例：

```cpp
template <typename T> // line 1
struct remove_ref
{
    typedef T type;
};

template <typename T> // line 1
struct remove_ref<T &>
{
    typedef T type;
};
```

让我们忽略 template 那一行，可以得到如下代码：

```CPP
struct remove_ref // primary template does not need args
{
    typedef T type;
};


struct remove_ref<T &&> // specialization
{
    typedef T type;
};
```

1. 主模板不需要写 `<args...>`
2. specialization 写上自己需要实例化的时候，对应主模板的形式，最后在第一行补上未知的类型参数

template 那一行表示的是，下面要用到的未知的类型参数。

## 面试问题

### 利用类模板和函数模板实现编译器计算斐波那契数列

见 [./template-fibo.cpp](./template-fibo.cpp)

注意：类模板成员变量要用 `static const` 修饰

-   static 允许 `fib<5>::value` 这样形式访问
-   `const static` 允许编译器进行计算

### 模板的声明和定义为什么不能分开写，要想分开写该怎么做

因为是 C++ 是分离式编译。

```cpp
// foo.h
template<typename T>
void foo(T a);

// foo.cpp
template<typename T>
void foo(T a) {
    std::cout << a << std::endl;
}

// main.cpp
#include"foo.h"
int main() {
    foo<int>();
}
```

编译到 `main.cpp` 中 `foo<int>` 时候，`main.o` 里会有一个 `call foo_int` 的指令，等待链接的时候补上。  
但是编译到 `foo.cpp` 的时候，他不知道 `main.o` 里面调用了 `foo<int>`，所以不会实例化 `foo_int` 这个函数。

可以直接把 `foo.h` 和 `foo.cpp` 合并到一个文件解决这个问题，叫做 `hpp`。  
[stackoverflow 上有两种方法](https://stackoverflow.com/a/495056/14380036)：

1. 在 `foo.h` 最后加上一行 `#include"foo.cpp"`，也就是合并到一块写。我看来不是从根本上解决这个问题。
2. 问题的关键是，分离式编译的时候，如何让 `foo.cpp` 得知需要实例化 `foo<int>`，修改为：

    ```CPP
    // foo.cpp
    template<typename T>
    void foo(T a) {
        std::cout << a << std::endl;
    }

    template void foo<int>; // tell the compiler to instantiation this function
    ```

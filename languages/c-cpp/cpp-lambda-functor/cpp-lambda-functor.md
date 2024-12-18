# C++ lambda 表达式与「函数对象」（functor）

-   created: 2024-06-27T16:29+08:00
-   published: 2024-11-17T17:01+08:00
-   modified: 2024-11-23T19:52+08:00
-   categories: C-CPP

[toc]

## functor(Function Object)

首先要介绍的是 functor——一个重载了 `operator()` 的类，该类的**实例**可以使用 `operator()`。

[What are C\+\+ functors and their uses? \- Stack Overflow](https://stackoverflow.com/questions/356950/what-are-c-functors-and-their-uses)

stackoverflow 上的例子很好，改一下放在这里：

```cpp
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
```

阅读 `std::for_each()` 这类函数的函数签名，最终只是调用了传入参数的 `()` 运算符，所以可以传入一个 functor。

```cpp
template <typename _InputIterator, typename _Function>
_GLIBCXX20_CONSTEXPR
    _Function
    for_each(_InputIterator __first, _InputIterator __last, _Function __f)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
        __glibcxx_requires_valid_range(__first, __last);
    for (; __first != __last; ++__first)
        __f(*__first); // <--- 看这里
    return __f; // N.B. [alg.foreach] says std::move(f) but it's redundant.
}
```

只是简单地将传入参数 `_f` 作用于 element。

对于 `std::sort()`，其函数签名为 `sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)`，也只是简单地使用 `__comp()`，所以只要传入的 `__comp` 支持 `()` 运算就可以。

## lambda

[C++ 中 Lambda 表达式（匿名函数）与传统的函数指针有什么本质区别？ - 雨乐的回答 - 知乎](https://www.zhihu.com/question/41940148/answer/2440283721)

lambda 实际上代码生成出了一个 functor，而 capture list 中的参数就是 functor 中的成员变量。  
默认引用捕获的成员变量是只读的，可以使用 mutable 来改变。

可以使用 [C\+\+ Insights \- Iterator tests](https://cppinsights.io/) 看 lambda 生成的中间代码。

## `<functional>`

`std::less<int> less_than;` less_than 就是一个 functor，相对地还有 `std::greater<T>`。

```cpp
#include <functional>
#include <iostream>

int main()
{
    std::less<int> less_than;
    std::cout << less_than(1, 2) << std::endl; // 1
    std::cout << less_than(2, 1) << std::endl; // 0
    return 0;
}
```

## 面试问题

#### 使用 lambda 实现函数对象

> 让介绍一下 C++ 里的 lambda。给了 `struct A {int x;}; std::vector<A> v;`，让用 `std::sort` 来排序：`std::sort(v.begin(), v.end(), [](A &a, A &b) {return a.x < b.x;});`。
> 指出 lambda 是一种语法糖，要我把它用函数对象实现。我当时不太会函数对象。
> 问了这里第三个参数 [](A &a, A &b) {return a.x < b.x;} 是什么类型。问了 std::sort 的函数签名是什么。当时不会。参见 理解 STL —— 迭代器与函数对象。
>
> ——[咸鱼暄的代码空间](https://xuan-insr.github.io/interviews/quant/#2-c-%E7%9B%B8%E5%85%B3)

一个用 functor 排序的例子见：<a href="./use-functor-lambda-to-sort.cpp" target="_blank">./use-functor-lambda-to-sort.cpp</a>

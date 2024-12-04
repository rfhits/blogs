# 左值、右值、移动语义、完美转发

-   created: 2024-06-25T15:20+08:00
-   published: 2024-06-26T10:40+08:00
-   categories: c-cpp

[C\+\+ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_01.html)

## 「右值引用」类型

先不讨论什么是移动语义、完美转发等概念，而是介绍一种新的类型，叫做「右值引用」类型，用法是可以引用到没法使用 & 取得地址的表达式。
例子如下：

```cpp
struct A
{
    int v{1};
};
A new_A() { return A(); }

int foo() { return 1; }

int main()
{
    // code in comment can't pass complication
    int &&x = 1; // int & x = 1;
    int &&y = foo(); // int & y = foo();
    A &&z = new_A(); // int & z = new_A();

    // now we can change x y z
    x = 2;
    y = 3;
    z.v = 4;
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z.v << std::endl;
    return 0;
}
```

在编译器的实现上，右值引用也是用指针实现的，参考：[通过汇编浅析 C\+\+ 右值引用 \- 知乎](https://zhuanlan.zhihu.com/p/389978619)
相当于引用到了一个栈上的匿名变量（「匿名」二字是自己取的）。

```cpp
A foo(); // a function returns an A object

A x = foo(); // copy a temp A to x
A& y = foo(); // compile error
A & const y = foo(); // const works
A&& z = foo(); // z points to a temp variable
```

这里需要强调：

1. 类型是类型，表达式是表达式。`int, int&, int&&` 都是类型，`int&` 叫做「左值引用类型」，`int&&` 叫做「右值引用类型」
2. 表达式分为：左值、纯右值和将亡值。左值可以被取地址，lvalue 意思是 locator value
    ```cpp
    int a = 2, b = 3;
    int c = a * b; // c is lvalue
    a * b = 42; // error, a * b is not locatable
    int && c = a * b; // c point to an integer
    ```
3. 右值引用类型的变量只能绑定到右值表达式。如 `int&& x = 1` 可以，但是 `int var = 1; int&& x = var;` 不行

### 右值引用类型作为函数参数

右值引用类型的变量作为表达式是一个左值。

```cpp
void foo(int&);
void foo(int&&);
int && a = 1;
a=2;
int * ptr = &a; // now a is a locator value

foo(a); // foo(int&) called. a 可以取地址，真的表示一个对象，是一个左值，foo(int&) called
foo(1); // foo(int&&) called
```

右值引用类型的变量 `a` 实际上是一个左值，它已经指向了一个对象，所以 foo(a) 会调用 `foo(int&)`。

### 小结

想一想右值引用这个名字起的还不错，首先是它一个引用，不是一份拷贝，然后引用的是那些没法被取地址的值。
因为普通的左值引用大部分都是引用以及存在的变量，现在通过加入了「右值引用」这种数据类型，从而引用到那些临时变量/右值表达式。

通过引入「右值引用」这种数据类型，解决了两大问题：

1. 移动语义
2. 完美转发

## 移动语义

[Page 2 of: C\+\+ Rvalue References Explained \(Move Semantics\)](http://thbecker.net/articles/rvalue_references/section_02.html)

有了这种类型，就可以提供函数的重载：

> By overloading a function like this:
> `void foo(X& x);` // lvalue reference overload
> `void foo(X&& x);` // rvalue reference overload
> you can branch at compile time on the condition "is foo being called on an lvalue or an rvalue?" The primary (and for all practical purposes, the only) application of that is to overload the copy constructor and copy assignment operator of a class for the sake of implementing move semantics. If and when you do that, make sure to pay attention to exception handling, and use the new noexcept keyword as much as you can.
> [Page 10 of: C\+\+ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_10.html)

## 手写 `std::move`

首先要知道 std::move 有什么用，我们常常看到：

`A a(std::move(b))`，调用

move 只是一个 `static_cast<T&&>`，让 move 的函数参数变成右值引用类型，从而调用那些接受右值引用类型的函数，比如移动构造函数等。

下面的例子中，move 的作用就和 `static_cast<T&&>` 一样，只是转换参数的类型，从而实现走不同的函数。

```cpp
struct A
{
    int v{1};
};
A new_A() { return A(); }

void foo(A &a)
{
    std::cout << "foo(A&) called" << std::endl;
    return;
}

void foo(A &&a)
{
    std::cout << "foo(A&&) called" << std::endl;
    return;
}

int main()
{
    A a;
    foo(a);                    // foo(A&) called
    foo(std::move(a));         // foo(A&&) called
    foo(static_cast<A &&>(a)); // foo(A&&) called
    return 0;
}
```

### `remove_reference`

remove_reference 是类模板，意思是接受一个类型参数，移除其引用。
为了实现 move 我们先实现 remove_reference，这个用到了类模板的特化。

参考阅读：[C\+\+模板元编程（零）：前言 \- 知乎](https://zhuanlan.zhihu.com/p/378355217)

> 注意，这个类模板的重载和完美转发容易混淆，完美转发是针对「函数模板」的类型推导规则，而 remove_reference 使用的是「类模板」。

```cpp
/// remove_reference
template<typename _Tp>
struct remove_reference
{ typedef _Tp   type; };

template<typename _Tp> // specialization for lref type
struct remove_reference<_Tp&>
{ typedef _Tp   type; };

template<typename _Tp>
struct remove_reference<_Tp&&> // specialization for rref type
{ typedef _Tp   type; };
```

remove_reference 真的很短，我们可以直接使用 `remove_reference<int&>::type` 表示 `int` 类型。

```cpp
int main()
{
    std::cout << sizeof(std::remove_reference<int&>::type) << std::endl;
    std::cout << sizeof(int) << std::endl;
    return 0;
}
```

### `move`

move 作为一个函数模板，如果参数是左值，就把它 cast 为右值，如果它是右值，还是把它 cast 为右值。

这个函数模板既可以接受左值，又可以接受右值，就涉及到「万能引用（universal references）」的概念，这个术语更好叫做 forwarding references，参考 [c\+\+ \- Is there a difference between universal references and forwarding references? \- Stack Overflow](https://stackoverflow.com/questions/39552272/is-there-a-difference-between-universal-references-and-forwarding-references)

> The first of the remaining two rules for rvalue references affects old-style lvalue references as well. Recall that in pre-11 C++, it was not allowed to take a reference to a reference: something like A& & would cause a compile error. C++11, by contrast, introduces the following reference collapsing rules1:
> A& & becomes A&
> A& && becomes A&
> A&& & becomes A&
> A&& && becomes A&&
> Secondly, there is a special template argument deduction rule for function templates that take an argument by rvalue reference to a template argument:
> template<typename T>
> void foo(T&&);
> Here, the following apply:
> When foo is called on an lvalue of type A, then T resolves to A& and hence, by the reference collapsing rules above, the argument type effectively becomes A&.
> When foo is called on an rvalue of type A, then T resolves to A, and hence the argument type becomes A&&.
> [Page 8 of: C\+\+ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_08.html)

然后再看 std::move 的实现，首先通过 一个 universal reference 来作为参数，然后 move 的返回类型就是 remove_reference 后再 &&，也就是 `remove_reference<T>::type&&`，最后生效的只是一个 static_cast。

要注意的是因为使用了嵌套类型，所以有 `typename` 关键字。

```cpp
template<class T>
typename remove_reference<T>::type&&
std::move(T&& a) noexcept
{
  typedef typename remove_reference<T>::type&& RvalRef;
  return static_cast<RvalRef>(a);
}
```

## 完美转发

[Page 7 of: C\+\+ Rvalue References Explained \(Perfect Forwarding\)](http://thbecker.net/articles/rvalue_references/section_07.html)

我们首先要知道完美转发解决了什么问题，

### 保持右值引用类型作为参数在函数调用链中类型不变

```cpp
void foo(int &) { std::cout << "lvalue" << std::endl; }

void foo(int &&x)
{
    std::cout << "rvalue" << std::endl;
    foo(x);
}

int main()
{
    foo(1); // rvalue \n lvalue
    return 0;
}
```

foo(int&&) 接受了 1 为参数后，就无法再调用 `foo(int&&)`，转而调用 `int&`，因为临时的 `1` 已经被 x 接受，具有了名字，是一个左值。
我们希望在函数的调用链中，让参数保持为 `int&&` 类型，把代码改成下面这样，就可以保持参数类型为 `int&&`，现在 `foo(int&&)` 是一个递归函数：

```cpp
void foo(int &) { std::cout << "lvalue" << std::endl; }

void foo(int &&x)
{
    std::cout << "rvalue" << std::endl;
    foo(std::move(x)); // recursive
}

int main()
{
    foo(1);
    return 0;
}
```

### 完美转发的实现原理

博客里提出的例子是工厂模式：

```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Arg&& arg)
{
  return shared_ptr<T>(new T(std::forward<Arg>(arg)));
}
```

[Page 8 of: C\+\+ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_08.html)

博客里称万能引用的推导规则为 "a special template argument deduction rule"，在我看来就是 corner case，而所有需要 corner case 的地方，就是常理 cover 不到的地方，需要我们特地去记忆。

博客讲得很清楚，这里就不赘述了。

如果从一个更高的角度看，为了保证函数参数类型在传递的过程中保持不变，于是使用万能引用：`template<typename T> f(T&& arg)`
万能引用作为函数模板，虽然它长得很像一个只能接受右值的函数模板，但是它是 special template，既可以接受左值，也可以接受右值，
根据引用折叠的规则，区分的关键记录在了自动推导的参数类型 T 里面：如果 `f(A&)`，T 类型就是 `A&`，如果 `f(A&&)`，T 类型就是 `A`。
为了让参数类型在下一次调用的时候保持不变，就传递给 `forward<T>`，让 forward 根据 T 的类型，决定要 static_cast 成左值引用还是右值引用。
所以使用 forward 的时候，我们必须要告诉它模板参数是什么，所以传入了一个 `<T>`。

因为 forward 在函数的内部被调用，只是用于转发外层函数的参数，所以 forward 的参数类型必为「左值引用类型」。

```cpp
outer(T&& arg) { return forward<T>(arg);}
```

forward 要做的就是，当 `<T>` 为 `A&` 时候，`return static_cast<A&>(arg)`，当 `<T>` 为 `A&&` 时候，`return static_cast<A&&>(arg)`，
利用引用折叠的规则，直接写作 `return static_cast<T&&>(arg)` 就可以了。

```cpp
template<class S>
S&& forward(typename remove_reference<S>::type& a) noexcept
{
  return static_cast<S&&>(a);
}
```

### why use `remove_reference` in forward

把 forward 的参数写成 remove_reference 类型，可以强制我们在使用 forward 的时候提供模板参数。
如果写成：

```cpp
template<class S>
S&& forward(S& a) noexcept
{
  return static_cast<S&&>(a);
}
```

效果也是一样的，但是这个时候 `int i = 1; forward(i);` 可以编译通过，我们不希望这样，forward 不应该自己推导模板参数，一定要人指定模板参数。
如果使用了 remove_reference，那么 `forward(i)` 编译就没法通过了。

```cpp

template <typename T>
T &&fwd_with_rmref(typename std::remove_reference<T>::type &arg)
{
    return static_cast<T &&> arg;
}

template <typename T>
T &&fwd_without_rmref(T &arg)
{
    return static_cast<T &&> arg;
}

int main()
{
    int i = 1;
    fwd_with_rmref(i);      // can't pass compling, we have to supply tempate argument, let's what we expect
    fwd_with_rmref<int>(i); // pass compling
    fwd_without_rmref(i);   // pass compling, but forward should not deducte argument type itself
    return 0;
}
```

### 例子

最后提提一个例子，`bar(1)` 成功保证了传递给 foo 的参数为 int&&：

```CPP
void foo(int&) { std::cout << "lvalue" << std::endl; }
void foo(int&&) { std::cout << "rvalue" << std::endl; }
template<typename T> void bar(T&& x) { foo(std::forward<T>(x)); }

int main() {
    int i = 1;
    int&& rref = 1;
    bar(i);  // output: lvalue
    bar(rref);  // output: lvalue! because rref is a lvalue
    bar(1);  // output: rvalue
}
```

## 右值引用的汇编分析

右值通过在栈上分配一个变量实现。

汇编验证网址：https://godbolt.org/z/8nrxx3KoW

```cpp
struct Student
{
    char name[100];
};

Student return_temp_student()
{
    return Student();
}

int main()
{
    Student &&x = return_temp_student();
    return 0;
}
```

`main()` 的栈中会有一个 `Student`，`x` 是一根指向栈上这个 `Student` 的指针。
下面汇编中 `sub     rsp, 112` 表明了这一点。

```asm
return_temp_student():
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        mov     rax, QWORD PTR [rbp-8]
        mov     rcx, rax
        mov     eax, 100
        mov     esi, 0
        mov     QWORD PTR [rcx], rsi
        mov     edx, eax
        add     rdx, rcx
        add     rdx, 8
        mov     QWORD PTR [rdx-16], rsi
        lea     rdx, [rcx+8]
        and     rdx, -8
        sub     rcx, rdx
        add     eax, ecx
        and     eax, -8
        shr     eax, 3
        mov     ecx, eax
        mov     rdi, rdx
        mov     rax, rsi
        rep stosq
        mov     rax, QWORD PTR [rbp-8]
        pop     rbp
        ret
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 112
        lea     rax, [rbp-112]
        mov     rdi, rax
        call    return_temp_student()
        lea     rax, [rbp-112]
        mov     QWORD PTR [rbp-8], rax
        mov     eax, 0
        leave
        ret
```

## 总结

1. 类模板针对引用类型特化的写法很特别，代表是 remove_reference
2. 函数模板针对引用类型特化的写法很特别，代表是 move
3. 万能引用是函数模板的一种特化写法，既可以接收左值参数，也可以接收右值参数
4. move 通过万能引用接收不同类型的参数，统一 remove reference 后 cast 成右值
5. `forward<T> (remove_ref<T>&)` 总是收到左值引用类型，希望通过形参推导出原有类型并返回

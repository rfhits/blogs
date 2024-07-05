

Created: 2024-07-02T20:22+08:00
Published: 2024-07-03T17:01+08:00

[toc]

我们需要区分 new-expressions 和 new operators。
new 有不同的用法：
1. new single object
2. new an array: 
3. placement new 一个对象
4. placement new 对象数组

# single object

new a single object 后 delete 流程：
1. `A* ptr = new A();`，使用的是 new-expression
   1. 调用 `void* operator new(sizeof(A))` 获取内存指针 `void p_mem`
      1. 类内部定义了的话就用类内部的
      2. 用户自定义了就用自定义的
      3. 否则使用全局的 `::operator new(size_t)`
   2. 在 `p_mem` 上调用构造函数
2. `delete ptr;`
   1. 析构 `ptr->~A();`
   2. 调用 `delete(void*)` 
      1. 类内部定义了的话就用类内部的
      2. 用户自定义了就用自定义的
      3. 否则使用全局的 `operator new`

```cpp
#include <iostream>

class A
{

public:
    int x, y, z;
    A()
    {
        std::cout << "Constructor called!" << std::endl;
    }

    ~A()
    {
        std::cout << "Destructor called!" << std::endl;
    }

    void *operator new(size_t size)
    {
        std::cout << "Custom new operator called! size:" << size << std::endl;
        return ::operator new(size);
    }

    void operator delete(void *ptr)
    {
        std::cout << "Custom delete operator called!" << std::endl;
        ::operator delete(ptr);
    }
};

int main()
{
    A *ptr = new A(); // 使用 new-expression 创建对象
    delete ptr;       // 释放内存
    return 0;
}

// output:
// Custom new operator called! size:12
// Constructor called!
// Destructor called!
// Custom delete operator called!
```

# array form

1. `A* ptr = new A[5];`，这是 array form new-expression
   1. 寻找并调用 `void* operator new[](sizeof(A) * 5 + extra_memory_for_count)` 得到一大块内存，可以直接使用 `::operator new(size_t)` 封装，extra_memory_for_count 用于统计有多少个 A，所以需要的内存大小多于 `count * sizeof(A)`
      1. 类内部定义了的话就用类内部的
      2. 用户自定义了就用自定义的
      3. 否则使用全局的 `::operator new[](size_t)`
   2. 逐个调用 Constructor
2. `delete[] ptr;` ptr 此时就是 operator new[] 得到的指针，指向内存，并且包括 new 的计数
   1. 调用 de-constructor
   2. 寻找并调用 `void operator delete[](void*)` 释放内存

```bash
heap:
low address ----------------------------------------------> high address

        | count | memory for constructed objects |
        ^       ^
        |       |
        |       A* ptr = new A[count];
        |       delete[] ptr
        |
void* p_mem = new[](sizeof(A) * count + extra_memory_for_count);
delete[] (p_mem);
```

```cpp
#include <iostream>

class A
{
public:
    int x, y, z;
    A()
    {
        std::cout << "Constructor called!" << std::endl;
    }

    ~A()
    {
        std::cout << "Destructor called!" << std::endl;
    }

    void *operator new[](size_t size)
    {
        std::cout << "Custom new[] operator called! size: " << size << std::endl;
        return ::operator new(size);
    }

    void operator delete[](void *ptr)
    {
        std::cout << "Custom delete[] operator called!" << std::endl;
        std::cout << "delete *ptr content: " << *(size_t*)(ptr) << std::endl;
        ::operator delete(ptr);
    }
};

int main()
{
    A *arr = new A[5]; // 使用 new[] 创建对象数组
    size_t *ptr = (size_t *)arr;
    std::cout << "count in extra memory ahead objects array: " << *(ptr - 1) << std::endl;
    delete[] arr; // 释放内存
    return 0;
}

// output:
// Custom new[] operator called! size: 68
// Constructor called!
// Constructor called!
// Constructor called!
// Constructor called!
// Constructor called!
// count in extra memory ahead objects array: 5
// Destructor called!
// Destructor called!
// Destructor called!
// Destructor called!
// Destructor called!
// Custom delete[] operator called!
// delete *ptr content: 5
```

# placement new/delete

## usage

placement new 意思是可以在 new expression 中指定其他参数，如果该参数是指针的话，编译器有自己的 placement new 实现，在该指针指向的内存区域构造对象。
要管理这部分的内存，需要手动调用析构函数后，再自定义回收流程。

```CPP
#include <iostream>

class A {
public:
    int x, y, z;

    A() {
        std::cout << "Constructor called!" << std::endl;
    }

    ~A() {
        std::cout << "Destructor called!" << std::endl;
    }

    void* operator new(size_t size, void* ptr) {
        std::cout << "Custom placement new operator called!" << std::endl;
        return ptr;
    }
};

int main() {
    // 分配内存
    void* mem = operator new(sizeof(A));

    // 在已分配的内存上构造对象
    A* obj = new (mem) A();

    // 手动调用析构函数
    obj->~A();

    // 释放内存
    operator delete(mem);

    return 0;
}
```

一提到 placement new 就认为提供的参数是指针，这是不对的，Stroustrup 自己也提供了例子：

```CPP
class Arena {
public:
        void* allocate(size_t);
        void deallocate(void*);
        // ...
};

void* operator new(size_t sz, Arena& a)
{
        return a.allocate(sz);
}

Arena a1(some arguments);
Arena a2(some arguments);

X* p1 = new(a1) X;
Y* p2 = new(a1) Y;
Z* p3 = new(a2) Z;
```

## 没有 placement delete expression

> The reason that there is no built-in "placement delete" to match placement new is that there is no general way of assuring that it would be used correctly.
> [Stroustrup: C\+\+ Style and Technique FAQ](https://www.stroustrup.com/bs_faq2.html#placement-delete)

因为担心人们用错，所以就不提供了。

## 但是有 placement delete function

因为 placement new function throw exception 后，会找对应函数签名的 placement delete，此时要正确释放内存，这次的 placement delete function 是可以保证参数的格式正确的。

[Effective C\+\+ 52：写了 placement new 就要写 placement delete \| Harttle Land](https://harttle.land/2015/09/22/effective-cpp-52.html)

# 试题

#### `new` 和 `malloc` 的区别

new 是关键字，调用 operator new 函数，得到内存后会调构造函数。
`malloc()` 返回 `void*`，可以作为 operator new 的内在实现方式。

#### `operator new`/`placement new`

`new` 会调 `operator new`，`operator new` 也是返回 `void*`，可以直接用 `malloc` 充当。
传入的参数就是一个 instance 的大小。

```CPP
class Base {
public:
    char c;
    int i;
    void* operator new(std::size_t size) {
        std::cout << "Base::new()" << std::endl;
        std::cout << "param size: " << size << std::endl; // 8. actual size of Base instance
        return malloc(size);
    }
};



int main()
{
    Base* b = new Base;
    return 0;
}
```

`placement new` 是一种重载形式，C++ 有内置的 placement new 的实现。

```cpp
class Base {
public:
    char c = 'a';
    int i = 42;

    void* operator new(std::size_t size) {
        std::cout << "Base::new()" << std::endl;
        std::cout << "param size: " << size << std::endl; // 8. actual size of Base instance
        return malloc(size);
    }
};

int main()
{
    char buff[100];
    // Base* b1 = new Base;
    Base* b2 = ::new (buff) Base; // search placement new implement in global space
    std::cout << buff[0] << std::endl; // a
    std::cout << *((int*)&buff[4]) << std::endl; // 42
    b2->~Base();
    return 0;
}
```

[placement new 机制 \- 知乎](https://zhuanlan.zhihu.com/p/228001107)
[详解 C\+\+重载 new, delete \- 知乎](https://zhuanlan.zhihu.com/p/354046948)

#### `delete` 和 `free` 区别

delete 也是一种操作符，先调用析构函数，再调用 `void operator delete` 将内存返回给管理者，`free()` 是一种实现方法。
最简单的实现方法就是直接在 `operator delete(void* ptr)` 中直接调用 `free(ptr)`。

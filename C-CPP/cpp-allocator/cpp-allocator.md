allocator、polymorphic allocator 与 memory_resource
cpp allocator

Created: 2024-07-04T10:59+08:00
Published: 2024-07-05T11:27+08:00
Categories: C-CPP

# custom allocator

`std::allocator` 是无状态的，实测最简单的 allocator 只需要：

1. value_type
2. allocate
3. deallocate

rebind 目的是实现 `rebind(allocator<TypeA>, TypeB) == allocator<TypeB>`
C++11 已经使用 allocator_traits 实现了这种想法[^1]，并且 C++17 就抛弃了以前把 rebind 放到 allocator 内部实现的方法。

还有两个函数 `construct` 和 `destroy`，如果提供，就会使用我们自定义的，不提供也没有问题，allocator_traits 提供了默认的实现，
container 本身就是通过 allocator_traits 来使用 construct 和 destroy。

1. construct：在 allocate 后调用，在分配的内存上初始化对象
2. destroy: 在 deallocate 前调用，在内存上析构分配的对象

以下是一个直接包装了 `::new` 和 `::delete` 的 allocator 实现。

```CPP
#include <vector>
#include <iostream>
#include <memory>

using std::vector;

struct point
{
    int x{1};

    point(int x_) : x(x_) {}

    ~point()
    {
        std::cout << "detor, x:" << x << std::endl;
    }
};

template <typename T>
class MyAllocator
{
public:
    using value_type = T;
    T *allocate(size_t n)
    {
        std::cout << "myallocator.allocate: " << n << std::endl;
        return (T *)::operator new(n * sizeof(T));
    }

    void deallocate(T *p, size_t n)
    {
        std::cout << "myallocator.deallocate: " << n << std::endl;
        return ::operator delete(p);
    }

    // template <typename _Up, typename... _Args>
    // void construct(_Up *__p, _Args &&...__args) noexcept(noexcept(::new((void *)__p) _Up(std::forward<_Args>(__args)...)))
    // {
    //     std::cout << "construct called" << std::endl;
    //     // 表示在 地址 _p 上调用对象 _Up的构造函数
    //     // 其中，__args是构造函数的参数
    //     ::new ((void *)__p) _Up(std::forward<_Args>(__args)...);
    // }

    // template <typename _Up>
    // void destroy(_Up *__p) noexcept(noexcept(__p->~_Up()))
    // {
    //     std::cout << "destroy called" << std::endl;
    //     __p->~_Up();
    // }
};

int main()
{
    point a{2};
    vector<point, MyAllocator<point>> vp{3,4,5};
    vp[0].x = 100;

    for (auto& i: vp) {
        std::cout << i.x << std::endl;
    }

    return 0;
}
```

# 多态内存资源（PMR, polymorphic memory resource）

## 使用 PMR 原因

为什么需要 PMR 呢，因为[^2]：

1. allocator 是模板签名的一部分。不同 allocator 的容器，无法混用。
2. c++11 以前，allocator 无状态；c++11 以后，可以有状态，然而 allocator 类型复杂难用。
3. allocator 内存对齐无法控制，需要传入自定义 allocator。

以上三点、特别是第一点，造成 stl 无法成为软件接口 (interface) 的一部分。
难以将 memory arena、memory pool 用于 stl 容器。

比如自定义的 allocator 没法和 stl 默认的 allocator 通用：

```cpp
vector<int, MyAllocator<int>> vi {1,2,3};
vector<int, std::allocator<int>> vi_copy = vi; // error!
```

## memory_resource 和 polymorphic_allocator

PMR 就说，好吧，那我们把 allocator 固定下来，全都使用 `polymorphic_allocator<T>`，`polymorphic_allocator<T>` 持有一根 `memory_resource` 的指针[^3]，分配策略通过 memory_resource 实现。

> The class template std::pmr::polymorphic_allocator is an Allocator which exhibits different allocation behavior depending upon the std::pmr::memory_resource from which it is constructed. Since memory_resource uses runtime polymorphism to manage allocations, different container instances with polymorphic_allocator as their static allocator type are interoperable, but can behave as if they had different allocator types.
> All specializations of polymorphic_allocator meet the allocator completeness requirements.
> [std::pmr::polymorphic_allocator \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/polymorphic_allocator)

`memory_resource` 提供对原始内存的管理接口，类似 malloc 和 free：

```cpp
memory_resource:
+ allocate: 提供给 polymorphic_allocator，调用 do_allocate
+ deallocate：提供给 polymorphic_allocator，调用 do_deallocate
# do_allocate： 内部分配内存的方法，像 malloc
# do_deallocate: 内部回收内存的方法，像 free
```

`polymorphic_allocator<T>` 只是在原始内存 `memory_resource` 上提供具体类型的抽象，比如需要 n 个类型为 T 的对象，底层调用 memory_resource 获取原始内存。

> `std::pmr::polymorphic_allocator<T>::allocate`:
> Allocates storage for n objects of type T using the underlying memory resource. Equivalent to return static_cast<T*>(resource()->allocate(n * sizeof(T), alignof(T)));.
> [std::pmr::polymorphic_allocator<T>::allocate \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/polymorphic_allocator/allocate)

construct 和 destroy 通过 allocator_traits 实现：

```CPP
/// Partial specialization for std::pmr::polymorphic_allocator
  template<typename _Tp>
    struct allocator_traits<pmr::polymorphic_allocator<_Tp>>
```

## 现成的 memory_resource

上面提到的分离接口可以实现不同 allocator 之间的通用，但是具体要让内存分配快起来需要高效的 memory_resource 实现，C++17 提供了五种[^5]:

| **内存资源**                   | **行为**                                               |
| ------------------------------ | ------------------------------------------------------ |
| `new_delete_resource()`        | 返回一个调用`new`和`delete`的内存资源的指针            |
| `synchronized_pool_resource`   | 创建更少碎片化的、线程安全的内存资源的类               |
| `unsynchronized_pool_resource` | 创建更少碎片化的、线程不安全的内存资源的类             |
| `monotonic_buffer_resource`    | 创建从不释放、可以传递一个可选的缓冲区、线程不安全的类 |
| `null_memory_resource()`       | 返回一个每次分配都会失败的内存资源的指针               |

这三种比较重要：

1. [std::pmr::monotonic_buffer_resource \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource)
2. [std::pmr::synchronized_pool_resource \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/synchronized_pool_resource)
3. [std::pmr::unsynchronized_pool_resource \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/unsynchronized_pool_resource)

在接口调用中提到了 upstream 的概念：如果当前 memory_resource 内存不足，则调用 upstream memory_resource 的 allocate 方法[^4]。
其实是有一个默认的 memory_resource 的，就是默认的 `::operator new` 和 `::operator delete` 管理内存。
而且 memory_resource 必须要有 upstream，看 `monotonic_buffer_resource` 的源码：

```CPP
    monotonic_buffer_resource(memory_resource* __upstream) noexcept
    __attribute__((__nonnull__))
    : _M_upstream(__upstream)
    { _GLIBCXX_DEBUG_ASSERT(__upstream != nullptr); }
```

## 用法

[std::pmr::monotonic_buffer_resource \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource)
[Cpp17/markdown/src/ch29\.md at master · MeouSker77/Cpp17](https://github.com/MeouSker77/Cpp17/blob/master/markdown/src/ch29.md#2913-%E8%AF%A6%E8%A7%A3%E6%A0%87%E5%87%86%E5%86%85%E5%AD%98%E8%B5%84%E6%BA%90)

# 总结

1. stl 容器需要 `allocator<T>`，
2. `allocator_traits` 规定访问 allocator 成员的标准接口[^6]。可以实现 rebind 等操作，以及对象的 construct 和 destroy 也在 traits 中有默认实现
3. 但是不同的 allocator 没法通用、无状态、难以实现自定义的分配策略，所以用 polymorphic_allocator 和 memory_resource 出现了
4. `polymorphic_allocator<T>` 内部持有 memory_resource 指针，统一了 allocator 接口，只封装了一层要管理的类型 T
5. memory_resource 内部来实现分配策略，和分配的具体对象无关
6. 提供了五种特别的 memory_resource 实现

[^1]: [std::allocator_traits \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/allocator_traits)
[^2]: [游戏引擎开发新感觉！\(6\) c\+\+17 内存管理 \- 知乎](https://zhuanlan.zhihu.com/p/96089089)
[^3]: [std::pmr::memory_resource \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/memory_resource)
[^4]: [std::pmr::monotonic_buffer_resource \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource)
[^5]: [Cpp17/markdown/src/ch29\.md at master · MeouSker77/Cpp17](https://github.com/MeouSker77/Cpp17/blob/master/markdown/src/ch29.md#2913-%E8%AF%A6%E8%A7%A3%E6%A0%87%E5%87%86%E5%86%85%E5%AD%98%E8%B5%84%E6%BA%90)
[^6]: [std::allocator_traits \- cppreference\.com](https://en.cppreference.com/w/cpp/memory/allocator_traits)

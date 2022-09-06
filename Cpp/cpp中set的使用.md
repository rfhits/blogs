cpp中set的使用

set是key-value型的有序集合，key通过 `compare<(element)`导出。

[toc]

# 常用接口

cpp reference的[std::set](https://en.cppreference.com/w/cpp/container/set)写得很不错了，就不搬运了。

## 定义与初始化

只介绍简单的方法，更多方法参考[Different Ways to Initialize an Set in C++](https://www.geeksforgeeks.org/different-ways-to-initialize-an-set-in-cpp/)这篇文章。

```cpp
set<int>New_set({4, 3, 9, 2, 0, 6});
```

## crud

只罗列部分

- create
  - [insert](https://en.cppreference.com/w/cpp/container/set/insert)
- read
  - [lookup](https://en.cppreference.com/w/cpp/container/set#:~:text=public%20member%20function)
  - [find](https://en.cppreference.com/w/cpp/container/set/find)
- update: 因为set靠element提供的key进行排序，所以不要尝试去更改决定key的成员
  - 直接find后，erase再insert
  - [将可更改的部分声明为mutable](https://stackoverflow.com/questions/7340434/how-to-update-an-existing-element-of-stdset)
  - [shared ptr](https://blog.csdn.net/qq_35976351/article/details/85331368)
- delete
  - [erase](https://en.cppreference.com/w/cpp/container/set/erase)

## 集合操作：交并差

只能存到支持 `push_back()`的容器里。

[并集](https://www.cnblogs.com/s1124yy/p/5849553.html)

[STL中set求交集、并集、差集的方法](https://www.cnblogs.com/Jadon97/p/8320926.html)

并集：`set1.insert(set2.begin(), set2.end())`

## 转vector

[C++ Vector转Set与Set转Vector](https://www.cnblogs.com/xwxz/p/13323712.html)

## set of stuct

[set集合容器自定义比较函数](https://blog.csdn.net/chijianxingfeng/article/details/8891078)

# set如何保证互异性？

set和我们平时理解的“集合”不太一样。从外表上看，我们从初中学习的“集合”这一[概念](https://baike.baidu.com/item/%E9%9B%86%E5%90%88/2908117)，它有如下特点

- 无序性
- 确定性
- 互异性

根据cpp reference的介绍，它是有序的，根据全序关系定义的。

> Everywhere the standard library uses the [Compare](https://en.cppreference.com/w/cpp/named_req/Compare "cpp/named req/Compare") requirements, uniqueness is determined by using the equivalence relation. In imprecise terms, two objects `a` and `b` are considered equivalent if neither compares less than the other: `!comp(a, b) && !comp(b, a)`.

所以我们重载 `<`这个运算符，具体的例子见[std::set自定义排序比较函数](https://blog.csdn.net/janeqi1987/article/details/107245566)。

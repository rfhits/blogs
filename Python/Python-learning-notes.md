Python 学习笔记

Datetime: 2022-12-12T20:18+08:00

Categories: Python

学习自 [廖雪峰的 Python 教程](https://www.liaoxuefeng.com/wiki/1016959663602400)，以及自己整理的一些笔记。

[toc]

# 前言

Python 是有不同的解释器的，我们从官方网站上下载的叫做 Cpython，下面列出了其他几种解释器：

-   PyPy: 使用了 JIT 技术
-   Jython: 运行在 Java 平台上
-   ...

下面这句话，真的看不懂：

> 要与不同平台交互，最好的办法是通过“网络调用”。

# Python 基础

## 字符串

不像 C/CPP，用 `'` 和 `"` 包起来，都是字符串。没有 `char` 这个概念

-   使用转义符号，如换行：`'\n'`
-   可以使用 `r` 不转义，`r'\n'` 将直接输出 \n
-   使用 `'''` 包含一段，可以允许断行
-   上述可以配合使用

```python
s = r'''what u see\n
is what u get
'''
print(s)
```

首先，有 `r` 开头，说明不要转义，遇到 `\n` 直接输出。然后因为是 `'''`，所以这个字符串允许换行。

所以，`r'''<content>'''`，写啥就是啥。

`str` 是不可变对象。

## 编码

编码的本质是建立**字符**到**比特流**的双射

ASCII 编码最简单，也包含的字符集最少，Unicode 一般使用 2 个字节编码 1 个字符，UTF-8 是可变长编码，兼容 ASCII。内存使用 Unicode，硬盘和传输使用 UTF-8。

Python 内存里面也是用 Unicode，可以自行想象函数调用栈里，一个字符串变量用 Unicode 编码并存储的场景。

所以 Python 有 `bytes` 这种数据类型，就是表示 01 串。有不同的方法可以表示 `bytes`，如

1. `b'ABC'`，就是把字符串用 ASCII 编码成 `bytes` 对象。因为默认是 ASCII，所以 `b'中文'` 就会报错
2. `b'\xe4\xb8\xad\xe6\x96\x87'`，就是 `中文` 的编码，用 x 表示十六进制（he `<u>`X `</u>`），而且两个十六进制刚好是一个字节。所以 `\x##` 就表示内存里一个字节的 01 串。

`encode()` 和 `decode()` 函数分别提供了到 `bytes` 和 `str` 的转换。

如果要把内存里的东西传输到网络上，就要调用 `encode()` 函数

开头那两行注释啥意思

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
```

> 第一行注释是为了告诉 Linux/OS X 系统，这是一个 Python 可执行程序，Windows 系统会忽略这个注释；
>
> 第二行注释是为了告诉 Python 解释器，按照 UTF-8 编码读取源代码，否则，你在源代码中写的中文输出可能会有乱码。

### 格式化

用 `%` 来格式化，自己看吧。

## list 和 tuple

`list` 是有序列表，里面的元素可以是任何数据类型，比如 `[1, "str", b"abc"]`。

`list` 可以使用负数索引。`-1` 表示倒数第一个，使用负数也要担心别越界。

`list.append(<elem>)`: 用于在尾部追加元素

`list.insert(<index>, <elem>)`: 用于在指定位置插入元素

`list.pop()`: 直接删除最后一个元素

`list.pop(<index>)`: 删除指定位置元素

`tuple` 就是在定义的时候，内部索引无法更改的有序列表。

## dict 和 set

`dict` 是 `dictionary` 的缩写，语法和 JavaScript 一样，定义一个 `dict` 是用 `{}` 包起来，支持嵌套。

```python
d = {
    1:2,
    "a":3,
    "d": {
        1:"str"
    }
}

print(d["d"][1]) // str
```

和 CPP 一样，`dict` 的 `key` 是不可变的，但是可以通过索引到 key，修改 value

遍历一个 `dict`：

```python
d = {
    "name": "LiHua",
    "age": 10
}

for key in d:
    print(key)

for value in d.values():
    print(value)

for k, v in d.items():
    print(k, ": ", v)
```

`set` 就是 `key` 的集合，和数学上的集合概念一样，也支持交并补等操作。

```python
s1 = {1,2,3}
s2 = {2,3,4}
print(s1&s2)
```

# 函数

Python 内置了许多函数，如 `abs()` 和 `hex()` 等。

有一类函数是类型转换函数，比如 `int()`，常见场景是把输入转换为 `int` 类型的数据，如 `int(input())`

## 定义函数

使用 `def` 定义函数，使用 `instance()` 函数可以做类型检查。

```python
def my_abs(x):
    if not isinstance(x, (int, float)):
        raise TypeError("type of argument should be int or float")
    else:
        if x>0:
            return x
        else:
            return -x
```

函数返回多个值，此时返回的是一个 `tuple`

函数支持嵌套定义。

## 调用函数

和 Java 一样，和 CPP 不一样，Python 函数调用，传递的参数是**引用**，而不是拷贝：

```python
def f(ref):
    ref.append("hello")

l = ["world"]
f(l)
print(l) # ["world", "hello"]
```

## 默认参数

```python
def Person(name, age, city="Beijing", gender="F"):
    return {
        "name": name,
        "age": age,
        "city": city,
        "gender": gender
    }

p1 = Person("LiHua", 10)
p2 = Person("LiHua", 10, gender="M")
```

默认参数有一个坑，看下面这个有默认参数的函数：

```python
def f(l = []):
    l.append("END")
    return l

print(f(["BEGIN"])) # ["BEING", "END"]
f() # call f
l = f() # call f again
print(l) # ["END", "END"]
```

在定义函数的时候，就确定了默认参数指向的对象，每次调用函数，都会在内部修改这个对象。当多次调用函数，这个对象就被修改了多次。

我们可以做一个实验，获取这个在函数定义就生成的对象，再修改它：

```python
def f(l=[]):
    return l

l = f()
l.append("hello")
print(f()) # ['hello']
```

所以这样函数就不具备”幂等性“，每次调用，相同的参数竟然返回不一样的结果。

教程上说，默认参数必须要指向不可变对象，这样才不会出错。

那么要如何实现呢？方法是使用 `None` 这个不可变对象，其实只要在函数内部判断用户是否传入了对应的参数，如果传入，就使用之，否则，每次新创建一个对象就行。

```python
def add_end(L=None):
    if L is None:
        L = []
    L.append('END')
    return L
```

## 可变参数

传入任意数量的参数，使用 `def f(*arg):` 表示用 `arg` 作为一个 `tuple`，接收在调用时传入的许多参数。

将可迭代对象 `o` 通过 `*o` 的方式，”展开“成”很多参数“

```python
def sigma(func=None, *r):
    if func is None:
        def f(x):
            return x
        func = f
    sum = 0
    for i in r:
        sum += func(i)
    return sum


def my_pow(x):
    return x*x


my_range = list(range(1, 10))
sum = sigma(my_pow, 1, 2, 3)  # 14
sum = sigma(None, 1, 2, 3)  # 6
sum = sigma(my_pow, *my_range)
sum = sigma(my_pow, *range(1, 10))
print(sum)
```

## 关键字参数

使用 `def f(**kw)` 的形式，将传入的参数收集到叫做 `kw` 的字典里；使用 `call(**d)` 的形式，将一个字典的键值对通过关键字的形式”解“出来并传入函数，函数内部获得的是一份拷贝。

```python
def Person(name, age, **other):
    return {
        "name": name,
        "age": age,
        "other": other
    }

d = {
    "name": "LiHua",
    "age": 10
}

o = {
    "city": "Beijing",
    "gender": "M"
}

print(Person(**d, **o)) # Person(name="LiHua", age=10, city="Beijing", gender="M")
```

## 命名关键字参数

需求：调用者需要显式地写出每个传入参数要初始化哪一个函数的参数

调用形式有两种：

1. 通过 `*` 来告诉函数 `*` 后面的参数是”命名关键字参数“。
2. 如果函数定义中已经有了一个可变参数，后面跟着的命名关键字参数就不再需要一个特殊分隔符 `*`了：

```python
# 调用者必须以 city=xxx, gender=xxx 的形式调用
def Person(name, age, *, city, gender):
    return {
        "name": name,
        "age": age,
        "city": city,
        "gender": gender
    }


d = {
    "name": "LiHua",
    "age": 10
}


# print(Person(**d), "bei", 'M') # TypeError
print(Person(**d, city="Beijing", gender="M"))
```

## 组合参数

> 参数定义的顺序必须是：必选参数、默认参数、可变参数、命名关键字参数和关键字参数。

可变参数后面的参数肯定只能通过 `arg_keyword=value` 传入，否则会和组装成的 `tuple` 混淆。

命名关键字参数肯定在关键词参数后面，否则都被当成关键字参数传入了，`**kw` 捕获到了一切。

# 高级特性

和 JavaScript 太像了

## 切片

首先要知道可迭代对象，切片（Slice）是针对可迭代对象的，形式为

`obj[begin=0:end=<last_index>:step=1]`

```python
r = range(0, 10)
for i in r[1:9:3]:
    print(i, end=' ') # 1 4 7
```

对于负数索引，迭代的顺序也是从左往右：

```python
r = range(0, 10)
for i in r[-9::3]: # 表示从倒数第 9 个到最后一个
    print(i, end=' ') # 1 4 7
```

如果迭代对象里存储的是 `int` 这类数据，切片会拷贝。比如：

```python
lst = list(range(10))
copy = lst[:] # 从头到尾的切边，是一个新的 lst
```

如果存储的是引用，则拷贝的是引用：

```python
l = [[]]
for i in l[:]:
    i.append("hello")

for i in l:
    print(i) # ['hello']
```

## 迭代

对于可迭代对象 `obj`，都可以 `for i in obj: <do sth>`

> 通过 `collections.abc` 模块的 `Iterable` 类型判断一个对象是不是可迭代的

```python
from collections.abc import Iterable
s = "123"
r = range(1, 10)
d = {"k1": "v1", "k2":"v2"}

print(isinstance(d, Iterable)) # True

def print_iterable(obj):
    for i in obj:
        print(i)

print_iterable(s)
print_iterable(r)
print_iterable(d)
```

Python 内置的 `enumerate()` 函数可以把一个可迭代对象变成索引 - 元素对，这样就可以在 `for` 循环中同时迭代索引和元素本身：

```python
def print_iterable(obj):
    for i, v in enumerate(obj):
        print(i, v)
```

## 列表生成式

简单看起来是这样：`[<expr> for <i> in <obj> (if expr)?]`

当然可以使用嵌套 `for`

```python
[i for i in range(1, 11)] # [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
[i*i for i in range(1, 11) if i%2==0] # [4, 16, 36, 64, 100]
l = [m * n # 2, 6, 4, 12
     for m in range(1, 5) if m % 2 == 0 # 2 4
     for n in range(1, 5) if n % 2 == 1] # 1 3
```

## 生成器（generator）

和 JavaScript 中概念一致

两种方法定义一个生成器：

1. 列表生成式的 `[]` 改为 `()`
2. 有 `yield` 关键字的函数

两种方法获取迭代器每次产生的值：

1. 调用 `next(obj)`，`obj` 是一个生成器对象
2. 使用 `for i in obj:`，`obj` 是一个生成器对象

使用 `()` 获得一个生成器对象：

```python
g = (i for i in range(10)) # g 是一个生成器对象
print(next(g)) # 0
print(next(g)) # 1

for i in g:
    print(i) # i begins at 2
```

下面是一个使用函数生成斐波那契数列迭代器的例子：

```python
def fib(end=10):
    i = 1
    a = 1
    b = 1
    yield a
    yield b
    while(i < end):
        b = a + b
        a = b - a
        yield b
        i+= 1
    return 0


g = fib() # 调用一次函数，就生成一个迭代器

for i in g:
    print(i)
```

如何获取函数 `fib()` 最后的返回值呢？

> 但是用 `for`循环调用 generator 时，发现拿不到 generator 的 `return` 语句的返回值。如果想要拿到返回值，必须捕获 `StopIteration` 错误，返回值包含在 `StopIteration` 的 `value` 中

## 迭代器

迭代对象和迭代器是不同的概念哦

> 这些可以直接作用于 `for` 循环的对象统称为可迭代对象：`Iterable`。
>
> 可以被 `next()` 函数调用并不断返回下一个值的对象称为迭代器：`Iterator`。

# 函数式编程

函数本身也是对象，可以作为变量来传递。

> 把函数作为参数传入，这样的函数称为高阶函数，函数式编程就是指这种高度抽象的编程范式。

## map/reduce

`map()` 函数接收两个参数，一个是函数，一个是 `Iterable`，`map` 将传入的函数依次作用到序列的每个元素，并把结果作为新的 `Iterator` 返回。

```python
def square(x):
    return x*x
l = list(map(square, range(1, 11)))
print(l) # [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

`reduce` 的作用大概就是把函数调用得到的结果再作为函数的参数，这个函数必须接收两个参数。

```
reduce(f, [x1, x2, x3, x4]) = f(f(f(x1, x2), x3), x4)
```

一个例子：

```python
from functools import reduce


def l_shift(x, y):
    return x * 10 + y
num = reduce(l_shift, [1,2,3,4])
print(num)
```

## filter

和 `map()` 类似，`filter()` 也接收一个函数和一个序列。和 `map()` 不同的是，`filter()` 把传入的函数依次作用于每个元素，然后根据返回值是 `True` 还是 `False` 决定保留还是丢弃该元素。

我觉得最神奇的地方在于，可以不断地对一个 `iterator` 进行 `filter`

```python
def possible_primes(): # 等待筛选的序列
    yield 2
    yield 3
    n = 3
    while True:
        n += 2
        yield n

def not_div(d): # 筛选条件，让无法整除 d 的数字通过这个筛子
    return lambda x: x%d != 0

def primes():
    it = possible_primes() # it 是一串等待筛选的数序列
    while True:
        p = next(it)
        yield p
        it = filter(not_div(p), it) # 不断生成一个素数，并对此序列 filter

plist = primes()
for i in range(10):
    print(next(plist))
```

## 返回函数/闭包

就是闭包。

让一个函数 A 返回一个函数 B，那么在 B 的内部就可以使用 A 中定义的变量。

一个简单的例子，就用网站上的 `lazy_sum` 这个例子：

```python
def lazy_sum(*args):
    def f():
        sum = 0
        for i in args:
            sum += i
        return sum
    return f

f = lazy_sum(*range(11))
print(f())
```

闭包（Closure）的概念是通用的，JavaScript 也有闭包。我知道 JavaScript 闭包的实现是通过上下文环境和垃圾回收，但是廖雪峰可没在教程里说 Python 怎么实现的。

使用闭包的时候有几个点需要注意：

1. 闭包返回的函数没有理解被调用，会在调用的时候计算
2. 闭包内部定义的变量是函数局部的，同名变量要告诉内部函数使用外部函数的变量

### 不要使用可能变化的变量

```python
def out():
    fs = []
    for i in range(1, 4):
        def f():
            return i * i
        fs.append(f)
    return fs

fs = out()
for f in fs:
    print(f()) # 9 9 9
```

在调用 `f()` 的时候，才会执行 `return i*i` 这段代码，此时，`i` 已经变成了 3。

所以不要在闭包函数中使用循环变量或者后续可能发生变化的变量。

那要怎样才能输出 `1 4 9`，关键就是把这个时候的 `i` 给截胡下来，让 `f` 接收参数 `i`，这样获得的就是值而不是引用，再在 `f` 内部定义一个函数，使用这个值拷贝的 `i`。

```python
def out():
    fs = []
    for i in range(1, 4):
        def f(i):
            def g():
                return i * i
            return g
        fs.append(f(i))
    return fs
```

### nonlocal

```python
def out():
    x = 1
    def f():
        # nonlocal x
        x = 2
    f()
    return x

print(out()) # 1
```

为了告诉 `f()` 要使用外部的变量，需要加上一句 `nonlocal <var>`。这个和 CPP 中的 lambda 表达式的“捕获”很类似。

## 匿名函数

lambda 表达式，语法为 `lambda <args>: <expr>`，`expr` 就是要返回的值，等价于

```python
def func(<args>):
	return <expr>
```

一个例子：

```python
l = list(map(lambda x: x*x, range(1, 11)))
print(l) # [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

## 装饰器（decorator）

-   装饰器的作用：接收函数作为参数，在函数调用前后加上自己的操作
-   装饰器可以使用 `@`
-   使用 `functools.wraps` 将装饰器返回的函数的 `__name__` 还原

装饰器属于闭包的应用，很多高级特性都使用到了装饰器。

装饰器可以对某个函数的功能进行增强，比如已经有了一个函数 `f`，我们希望调用函数 `f` 时候，马上输出函数名字：

```python
def log(f):
    def wrap(*args, **kw):
        print("call:", f.__name__)
        return f(*args, **kw)
    return wrap


def sum(*arg):
    sum = 0
    for i in arg:
        sum += i
    return sum


wrap_sum = log(sum)  # wrap sum
s = wrap_sum(*list(range(1, 11)))
print(s)
```

输出是：

```
call: sum
55
```

这个 `log` 就是一个装饰器，可以使用类似于注解的方式，这样就不用手动调用 `log` 并接受返回值：

```python
@log
def sum(*arg):
    sum = 0
    for i in arg:
        sum += i
    return sum


print(sum(*list(range(11))))
```

相当于 `now = log(now)`。

最后，为了保证返回的 `__name__` 属性不变，使用 `functools.wrap`

```python
import functools

def log(text):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            print('%s %s():' % (text, func.__name__))
            return func(*args, **kw)
        return wrapper
    return decorator
```

这样 `@log(text)` 来注解一个函数，就相当于 `func = @log(text)(func)`。

## 偏函数

就是 CPP 中的 `bind`

> 所以，简单总结 `functools.partial`的作用就是，把一个函数的某些参数给固定住（也就是设置默认值），返回一个新的函数，调用这个新函数会更简单。

```python
>>> import functools
>>> int2 = functools.partial(int, base=2)
>>> int2('1000000')
64
>>> int2('1010101')
85
```

# 模块

一个 `.py` 文件就是一个模块，使用 `package` 来管理模块。一个 `package` 就是一个有 `__init__.py` 文件的文件夹。`package` 允许嵌套。`__init__.py` 也是一个模块，它的名字就和文件夹的名字一样。

```
mycompany
|- __init__.py
|- web
    |- __init__.py
    |- abc.py
    |- xyz.py
|- abc.py
|- xyz.py
```

> ```python
> import sys
> ```
>
> 导入 `sys`模块后，我们就有了变量 `sys`指向该模块，利用 `sys`这个变量，就可以访问 `sys`模块的所有功能。

# 面向对象编程

我觉得还是先知道各种下划线：

-   `__*__`: 双下划线开头和结尾，是**特殊变量**，可以直接访问
-   `__*`: 双下划线开头，约定是 `private` 变量，Python 解释器可能会修改这个成员的名字。比如 `Student` 类的私有属性 `__name` 可能被修改为 `_Student__name`
-   `_*`: 单下划线开头，虽然不是私有变量，但是大家约定将其视为私有变量，不要随意修改它

> 如果要让内部属性不被外部访问，可以把属性的名称前加上两个下划线 `__`，在 Python 中，实例的变量名如果以 `__` 开头，就变成了一个私有变量（private），只有内部可以访问，外部不能访问，所以，我们把 Student 类改一改

下面是一个我自己编写的 `Student` 类，十分简陋，但是足以体现封装的思想。

> 使用函数的好处之一是可以进行类型检查

```python
class Student(object):
    def __init__(self, name, age):
        self._check_name(name)
        self._check_age(age)
        self.__name = name
        self.__age = age

    def _check_age(self, age):
        if not isinstance(age, int):
            raise TypeError
        if age <= 0:
            raise ValueError
        return

    def _check_name(self, name):
        if not isinstance(name, str):
            raise TypeError
        return

    def get_name(self):
        return self.__name

    def set_name(self, name):
        self._check_name(name)
        self.__name = name
        return self

    def get_age(self):
        return self.__age

    def set_age(self, age):
        self._check_age(age)
        if age <= 0:
            print("age should greater than 0")
        else:
            self.__age = age
        return self
```

使用之：

```python
s = Student("Alice", 18)
s.set_age(19).set_name("Bob")
print(s.get_age()) # 19
```

## 获取对象信息

### 类型

使用 `type()` 函数获取对象信息

```python
import types # 有很多内置的 types
def fn():
    pass


print(type(123))  # <class 'int>
print(type(123) == int)  # True
print(type('123'))  # <class 'str'>
print(type(fn)) # <class 'function'>
print(type(fn) == types.FunctionType) # True
```

`isinstance(obj, type)` 也可以用来判断对象所属的类型，是沿着继承链一直判断下去

```python
b = isinstance(123, object)
print(b) # True
```

### 属性

这一部分和 JavaScript 很像。

对于动态语言，一个问题是，如果属性是个变量，如何获取？或者说应该如何提供一种类似于“反射”的机制。

```python
def get_attr(obj, k):
    return ? # 不能直接写 obj.k，JavaScript 可以写作 obj[k]
```

在 Python 中，对象可不是 `map`。

```python
class C(object):
    def __init__(self, name):
        self.name = name


c = C("hello")
attr = 'name'
# c['name'] = "world" # Error
setattr(c, attr, "world")
print(c.name) # 'world'
```

-   `dir(obj)`: 以 `list[str]`的形式返回 `obj` 的所有属性
-   `setattr(obj, attr, value)`: 通过 `attr` 设置 `obj` 对应属性的值，优点就是，`attr` 是一个字符串变量
-   `getattr(obj, attr)`: 获取 `obj` 的 `attr` 属性

## 实例属性和类属性

```python
class Student(object):
    grade = '1st'


s = Student()
print(s.grade) # '1st'
s.grade = '2nd'
print(s.grade) # '2nd'
del s.grade
print(s.grade) # '1st'
```

所有的 `student` 实例都可以访问到类内部定义的 `grade` 属性。

不要让实例属性和类属性同名，否则访问的时候会优先访问到实例属性。

# 面向对象高级编程

可以给类的实例绑定方法，也可以给类绑定方法。

```python
class Student(object):
    def __init__(self):
        pass


def set_age(self, age):
    self.age = age


s = Student()
s.age = 18
print(s.age) # 18
s.set_age = set_age
s.set_age(s, 19)
print(s.age) # 19


def set_name(self, name):
    self.name = name
    return self


Student.set_name = set_name

s.set_name("Alice")
print(s.name) # Alice

```

虽然不规范，用上 linter 各种报错，但是可以 work。

可以使用 `__slots__` 来限制类的属性范围，就好像 `Student` 这个类，只提供了几个插槽，只能把插槽对应的类添加到类的属性里。

```python
class Student(object):
    __slots__ = ("name")

    def __init__(self):
        pass


def set_age(self, age):
    self.age = age


s = Student()
s.name = 'Alice'
print(s.name)
s.age = 18 # AttributeError: 'Student' object has no attribute 'age'
```

> `__slots__` 可以理解为给某个类预设属性清单，所有根据此类创建的实例只允许绑定清单内的属性，不能绑定其他属性。
>
> 但该清单只对本类有效，对其子类无效（除非其子类也有 `__slots__`）。
>
> 若其子类也有 `__slots__`，则子类实例可绑定的属性为父类清单和子类清单的**并集**。

## 使用 `@property`

https://www.liaoxuefeng.com/wiki/1016959663602400/1017502538658208

## 多重继承

-   Python 中没有“接口”的概念，只有“类”。
-   Python 允许多继承

如果要像 Java 那样，一个类 A 单继承一个类 B，同时还实现很多接口 C、D，在 Python 中只能把接口 C、D 也定义为类，让 A 继承自 B、C 和 D。

```python
class Animal(object):
    pass


class Runnable(object):
    def run(self):
        print("is running")


class Dog(Animal, Runnable):
    pass


d = Dog()
d.run()
```

MixIn 是一种设计，我理解为就是让一个类在数据上保持单继承，行为上可以混入其他类。一般来说混入的类命名为 `*MixIn`。

```python
class RunnableMixIn(object):
    def run(self):
        print("is running")


class Dog(Animal, RunnableMixIn):
    pass
```

## 定制类

浅浅地了解下一些背后的逻辑。

### `__str__` 和 `__repr__`

`print(obj)` 打印的是 `obj.__str__()` 返回值，直接 obj 也是有返回值的，返回的是 `obj.__repr__()` 返回值。

```python
class Student(object):
    def __init__(self, name, age):
        self.__name = name
        self.__age = age

    def __str__(self):
        return 'student %s, age: %s' % (self.__name, str(self.__age))

    __repr__ = __str__


s = Student('Alice', 18)
print(s)   # student Alice, age: 18
```

### `__iter__` 和 `__next__`

> 如果一个类想被用于 `for ... in` 循环，就必须实现一个 `__iter__()` 方法，该方法返回一个迭代对象，然后，Python 的 for 循环就会不断调用该迭代对象的 `__next__()` 方法拿到循环的下一个值，直到遇到 `StopIteration` 错误时退出循环。

下面是一个 `SnowFlake` 的例子：

```python
class Snow(object):
    def __init__(self, num):
        self.__num__ = num

    def __iter__(self):
        return self

    def __next__(self):
        if self.__num__ == 0:
            raise StopIteration
        if self.__num__ % 1 == 1:
            self.__num__ *= 3
            self.__num__ += 1
        else:
            self.__num__ //= 2
        return self.__num__


for i in Snow(100):
    print(i)
print(list(Snow(100))) # [50, 25, 12, 6, 3, 1, 0]
```

### `__getitem__`

可以用于下标索引和切片操作

直接看廖雪峰写的吧，我赶时间，api 而已。

[**getitem**](https://www.liaoxuefeng.com/wiki/1016959663602400/1017590712115904#:~:text=__getitem__)

### `__getattr__`

> 只有在没有找到属性的情况下，才调用 `__getattr__`，已有的属性，比如 `name`，不会在 `__getattr__` 中查找。

### `__call__`

允许对实例进行调用。

## 使用元类

### type

类对象是 `type` 类型的，所以可以使用 `type()` 来在脚本运行的过程中创建一个类。

```python
def hello(self):
    print("hello, I am a student")
    return self


Student = type('Student', (object,), dict(say_hello=hello))
s = Student()
s.say_hello()
```

`type(__name: str, __bases: tuple[type, ...], __dict: dict[str, Any], **kwds: Any) -> None`

1. `__name`: 创建的这个类名字叫啥
2. `__bases`: 创建的这个类的父类元组
3. `__dict`: 创建这个类的属性

### metaclass

我理解为是，`metaclass` 是直接继承自 `type` 类的子类的称呼。比如类 A 继承了 `type`，它就是一个 `metaclass`。

使用 `metaclass`，可以在自定义类的创建过程。比如类 B 继承了 类 A，我们可以在 B 中书写配置文件，然后在 A 中自定义 B 的创建过程。

https://www.liaoxuefeng.com/wiki/1016959663602400/1017592449371072#:~:text=metaclass

# 错误、调试和测试

## 单元测试

https://www.liaoxuefeng.com/wiki/1016959663602400/1017604210683936

一个 py 文件就是一个模块，要做单元测试，就是一个个测试模块。

假如编写了模块 A，我们要对它做测试

1. 新建一个 `.py` 文件，用于编写测试模块
2. 文件内导入 `unittest` 模块和待测试的模块 A
3. 编写一个类，继承自 `unittest.TestCase`
4. 类内部名字叫做 `test_*` 的方法将会被看作一个测试用例，方法内部可以调用 `self.assertTrue` 等函数
5. 另一类重要的断言是 `self.assertRaise()`，期待抛出异常

### `setUp` 和 `tearDown`

test 类内部定义的 `setUp` 和 `tearDown` 分别会在每一个测试方法被调用前后执行，可以在里面进行数据库连接和关闭连接等操作，而不需要在每一个测试方法内部编写同样的代码。

# 进程与线程

进程：process

线程：thread

## 多进程

`os` 模块可以用于查看进程的 `pid`

Unix 内核的操作系统可以使用 `os.fork()` 创建一个新的进程

### multiprocessing

每一个进程被抽象为 `Process` 对象，使用的时候，导入模块 `from multiprocessing import Process`

-   `Process(target, args)`: `target` 可以是一个函数，`args` 是一个元组，用来放置函数的参数
-   `p.start()`: 启动进程 `p`
-   `p.join()`: 等待 `p` 执行结束，再执行之后的代码，常用于进程之间的同步

Process 只有一个函数的执行，，让人感慨也太简单了。

```python
import os
from multiprocessing import Process


def psum(*args):
    print("process pid:", os.getpid())
    ans = 0
    for i in args:
        ans += i
    print("args:", args)
    print("ans:", ans)


if __name__ == '__main__':
    print("parent pid:", os.getpid())
    p = Process(target=psum, args=list(range(11)))
    p.start()
    p.join()
```

### Pool

`Pool` 表示进程池，可以同时运行多个进程。

`p=Pool(cnt)`，这个 `cnt` 应该是表示同时运行多少个进程

使用 `p.apply_async(target, args)` 在 `pool` 中创建一个进程

### 子进程

自行参考

https://www.liaoxuefeng.com/wiki/1016959663602400/1017628290184064

### 进程间通信

可以使用 `multiprocessing` 中的 `Queue` 模块通信，当然还可以用 `pipe` 等，但是没教。

创建进程的时候，需要函数和参数，把 `Queue` 当作函数的参数传入，这样两个或多个进程就都能访问到同一个 `Queue`，有的进程往里面写入，有的进程从里面读取，就实现了进程间的通信。

## 多线程

https://www.liaoxuefeng.com/wiki/1016959663602400/1017629247922688

`t = Thread(target, args)`，和进程一样的创建方式

Python 中的语句不一定是原子操作，使用 `lock = threading.Lock()` 得到一个 lock，每个线程通过 `lock.acquire()` 和 `lock.release()` 获取和释放锁，防止“乱掉”。

### GIL

多线程在 Python 中，因为要获得 GIL 才能执行，所以实际上是交替执行。所以一个线程至多占用一个 CPU 核心。要想实现多核运行 Python，要么通过其他库，要么创建多个进程。

## ThreadLocal

介绍 `ThreadLocal` 前，每个线程怎样有自己的上下文环境？两种方法：

1. 创建的时候通过 `args` 传递，每个线程一开始就创建变量，每调用一个子函数就传递这个对象
2. 使用全局变量，使用线程作为 key，每个线程的环境作为 value

最好的方法是使用 `tl = threading.local() # return a ThreadLocal obj`，将这个 `tl` 设置为全局，每个线程都可以访问 `tl`，每个线程内访问变量都是自己的。

```python
import threading

local_school = threading.local()

def student_go():
    print("student", local_school.student, "go to school")


def thread_student(name):
    local_school.student = name  # 每一个线程都创建一个对象上去
    student_go() # 线程内部调用自己的方法


t1 = threading.Thread(target=thread_student, args=('Alice',))
t2 = threading.Thread(target=thread_student, args=('Bol',))
t1.start()
t2.start()
t1.join()
t2.join()
```

## 进程 v.s. 线程

单线程的异步编程模型称为协程

### 分布式进程

https://www.liaoxuefeng.com/wiki/1016959663602400/1017631559645600#0

首先要知道多进程在不同的机器上，是如何管理的，答案是通过 master-worker 模式：让一个进程作为主进程，分配任务，其他进程通过和主进程交流，获取任务。

`multiprocessing` 包的 `BaseManager` 模块就是作为 `master`，有如下功能：

1. 绑定调用，通过 `<m_class>.register(str: <callname>, callable: <callable>)`，使得可以通过 `<m_instance.<callname>` 调用
2. 绑定端口：通过构造方法，绑定到本地的端口上，这样网络上的其他进程就能根据 `ip:port` 联系上这个 `manager` 进程

# 正则表达式

正则表达式的定义本身就是递归的，所以我们只要知道几个基本的表达方式，就能构造出所有的。

精确地表达一个字符：一个字符本身就是精确匹配一个字符，特殊字符转义表示

表达一个范围内的一个字符：

-   任意字符：`.`
-   数字：`\d`
-   字母或者数字：`\w`
-   空白符：`\s`
-   范围：`[0-9a-zA-Z\_]` 数字字母下划线，可随意组合；`[]` 内也可以之间填写字符，如 `[abc]` 表示 `a|b|c`。
-   或：`A|B` 可以匹配正则表达式 `A` 或者正则表达式 `B`

表示多个字符连在一起：

1. 拼在一起就是表达多个字符，如 `007` 只能匹配 `007`，`52\d` 可以匹配 `520`
2. 出现 $\ge0$ 次：`*`
3. 出现 $\ge1 $ 次：`+`
4. 出现 0 次或 1 次：`?`
5. 出现 n 次：`{n}`
6. 出现 a 次到 b 次之间：`{a,b}`，切记不能有空格，写作 `{a, b}` 就错了

匹配：`pattern` 能够在给定的 `str` 里找到符合条件的子串就叫做完成了匹配。

如果想要让 `pattern` 匹配整个 `str` 咋办，Python 提供了 `^` 和 `$`，分别用于要求开头匹配和结尾匹配。是的，它很灵活，没有说一定要匹配整个输入的字符串，而是分别提供开头和结尾的匹配，这样二者结合就能匹配上一整串了。比如 `$python`，就要求输入 `str` 必须以 `python` 开头。

正则表达式中，如果要表示转义，就需要使用 `\` 字符，很巧，Python 字符串本身也是用 `\` 来表示转义，这样就导致 `s = '\\d'` 才表示 `\d`。如果要匹配 `\` 字符，正则表达式就要写成 `s = '\\\\'`。所以为了方便，一般都使用 `r` 前缀，表示写出来的字符串不要转义。这样匹配一个数字写成 `\d`，匹配一个 `\` 符号写成 `\\` 就行。

## `re.match`

`re` 是 Python 模块，提供 `match(pattern, string [, flags])`，从 `string` 的**开始位置**进行匹配，相当于 `pattern` 已经加上了 `^`。匹配成功，返回一个 `Match` 对象，否则返回 `None`。

## 分组

在正则表达式中使用 `()` 表示组。

比如我随手写一个邮箱的正则：`'`

```python
import re
mail_s = r'([a-zA-Z][0-9a-zA-Z\_]{0,10})@(\w{0,10})\.(\w{0,10})'
m = re.match(mail_s, 'alice@gmail.com')
for i in range(4):
    print(m.group(i))
print(m.groups())
```

输出如下：

```
alice@gmail.com
alice
gmail
com
('alice', 'gmail', 'com')
```

`m.group(0)` 永远表示整个串，然后才是第一、第二个子串，`m.groups()` 就正好都是子串。

## 贪婪匹配

自己看

# 常用内建模块

## os

`os.path.exists(path_to_file/folder)`: 判断某个文件/文件夹是否存在，当然 Linux 哲学是「一切皆文件」。

## datetime

https://www.liaoxuefeng.com/wiki/1016959663602400/1017648783851616

`datetime` 我理解为 `date` + `time`，在 `datetime` 模块的 `datetime` 类。`datetime` 提供的对一个时间点的抽象，比如 2022 年 12 月 15 日 12 点 15 分 42 秒 250 微秒。

```python
from datetime import datetime

dt = datetime(2022, 12, 15, 10, 32, 42, 123456)
print(dt) # 2022-12-15 10:32:42.123456
```

最后的微秒精确到了 `10^-6`。

### timestamp

`timestamp` 意思是时间戳，表示全球一个统一的时刻

> 把 1970 年 1 月 1 日 00:00:00 UTC+00:00 时区的时刻称为 epoch time，记为 `0`（1970 年以前的时间 timestamp 为负数），当前时间就是相对于 epoch time 的秒数，称为 timestamp。

timestamp 在 Python 中是一个浮点数，类型就是 `class float`，`dt.timestamp()` 就返回之。

### timedelta

需要 `from datetime import timedelta`

`datetime` 相减返回 `timedelta` 对象，`datetime` 也可以加上 `timedelta`到另一个时间。

### UTC 时间

> 本地时间是指系统设定时区的时间，例如北京时间是 UTC+8:00 时区的时间，而 UTC 时间指 UTC+0:00 时区的时间。

`datetime` 本身具有一个属性 `tz_info`，表示这是哪一个时区的时间，默认值为 `None`，也就是不能区分在哪一个时区，表示本地时间。

我猜测，`datetime.utcnow()` 会根据本地电脑设定的时区，自动计算出 UTC 时间。

```python
from datetime import datetime
print(datetime.now()) # 2022-12-15 10:44:23.033685
print(datetime.utcnow()) # 2022-12-15 02:44:23.058681
```

但是获取到的两个 `datetime` 对象的 `tzinfo` 都是 `None`

要存储一个时间，最好使用 `timestamp`，因为是全球统一的。

## collections

提供了各种集合类，类似于 CPP 中的顺序容器和关联容器。

-   `namedtuple`: 我想定义一种二元组类型，名字叫 `Point`，`Point = namedtuple('Point', ['x', 'y'])`，这样就可以通过 `<point>.x` 返回第一个值
-   `deque`: 双向链表，解决 `list` 插入慢的问题
-   `defaultdict`: key 不存在的时候提供默认返回值
-   `OrderedDict`: 保证迭代的时候有序 `Dict`，是按照插入的顺序
-   `ChainMap`: 如其名，相当于一串 Map，`cm[key]` 时候，按照在 `chain` 上的 `Map` 一个个查找，适用于有优先级的查找

## contextlib

`with <a> open as obj:` 可以在资源结束后执行代码，比如 `with open(sth) as f:`

这样的效果是因为 `f` 实现了 `__enter__` 和 `__exit__` 两个函数

```python
class Ctx(object):
    def __init__(self, name):
        self.name = name

    def __enter__(self):
        print("enter")

    def __exit__(self, exc_type, exc_value, traceback):
        print("in exit")

with Ctx('hello') as c:
    print("new obj ctx")
```

### `@contextmanager`

观察 `with ... as obj: <body>`，相当于 `obj = ...`，这个 `...` 恰好是个能够返回一个对象的方法而已。

`contextmanager` 在 `contextlib` 模块中，使用装饰器 `@contextmanager` 装饰一个可以生成对象的函数，也就是 `yield` 对象的函数，`yield` 后的语句就会在被 `<body>` 后被执行。

> Typical usage:
>
> @contextmanager def some_generator(`<arguments>`):
> `<setup>` try:
> yield `<value>`
> finally:
> `<cleanup>`
>
> This makes this:
>
> with some_generator(`<arguments>`) as `<variable>`:
>
> <body>
>
> equivalent to this:
>
> `<setup>` try:
> `<variable>` = `<value>` `<body>`
> finally:
> `<cleanup>`

```python
from contextlib import contextmanager

@contextmanager
def gen_empty_list():
    print("before yield hello")
    print("setup")
    l = []
    try:
        yield l
    finally:
        print("finally clear the list")
        l.clear()

with gen_empty_list() as l:
    l.append("hello")
```

### `@closing`

`closing()` 是 `contextlib` 内的一个函数

```python
@contextmanager
def closing(thing):
    try:
        yield thing
    finally:
        thing.close()
```

接收一个对象，为这个对象提供了上下文，最后调用这个对象的 `close()` 方法

```python
from contextlib import closing
class Close(object):
    def __init__(self, name):
        self.name = name

    def close(self):
        print(self.name, "get closed")

with closing(Close("hello")) as clo:
    print("with body")
```

输出是：

```
with body
hello get closed
```

# 常用第三方模块

## Pillow

Pillow 是一个处理图像的第三方库，从 PIL 这个库发展而来。

PIL 全称是 Python Image Library，只支持 Python 2.x，年久失修，志愿者在其基础上开发了 Pillow。

Pillow 模块提供了 Image 这个类，是对图片的抽象，还有 ImageDraw、ImageFilter 等工具类。

`Image.open(path)` 函数意思是打开路径对应的图片，返回一个 `Image` 对象。然后就是 api 了，没啥好说的。

## requests

https://www.liaoxuefeng.com/wiki/1016959663602400/1183249464292448

处理 HTTP 请求的第三方库。

`requests.get()`、`requests.post()` 等方法发送一个 HTTP request，返回一个 `requests.models.Response` 对象。把响应作为一种抽象。简而言之，它很方便。

## 其他第三方库

[chardet](https://www.liaoxuefeng.com/wiki/1016959663602400/1183255880134144): 一个猜测 bytes 对象是何种编码方式的第三方库，是 character detect 的缩写。

[psutil](https://www.liaoxuefeng.com/wiki/1016959663602400/1183565811281984): ps 本身就是一个 Linux 命令，全称是 Process Status，但是这个第三方库的全称是 Process and System Utilities。这个第三方库可以用来看进程和系统的状态，支持跨平台。

---- 未完待续 ----

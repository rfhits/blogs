利用 Python 进行数据分析第三版读书笔记

书籍地址：https://wesmckinney.com/book/

[toc]

# Python 语法基础、IPython 和 Jupyter Notebook

## ipython

命令行中使用 `ipython` 就可以进入交互模式了，就像命令行中输入 `python` 就能进入 Python 的交互模式。

```
C:\Users\***>ipython
Python 3.9.12 (main, Apr  4 2022, 05:22:27) [MSC v.1916 64 bit (AMD64)]
Type 'copyright', 'credits' or 'license' for more information
IPython 8.6.0 -- An enhanced Interactive Python. Type '?' for help.

In [1]:
```

使用 tab 会罗列出可能的选项

### introspect

introspect 译为自省，在变量后使用问号可以显示变量的信息，比如类型等。两个问号可以显示函数的源代码：

```
In [2]: def fn():
   ...:     print("hello world")
   ...:

In [3]: fn()
hello world

In [4]: fn?
Signature: fn()
Docstring: <no docstring>
File:      c:\users\wysj6174\<ipython-input-2-2e0a2c23c269>
Type:      function

In [5]: fn??
Signature: fn()
Docstring: <no docstring>
Source:
def fn():
    print("hello world")
File:      c:\users\wysj6174\<ipython-input-2-2e0a2c23c269>
Type:      function
```

### 魔术命令

% 就是魔术命令

| 命令                | 说明                                                       |
| ------------------- | ---------------------------------------------------------- |
| %quickref           | 显示 IPython 的快速参考。                                  |
| %magic              | 显示所有魔术命令的详细文档。                               |
| %debug              | 在出现异常的语句进入调试模式。                             |
| %hist               | 打印命令的输入〔可以选择输出）历史。                       |
| %pdb                | 出现异常时自动进入调试。                                   |
| %paste              | 执行剪贴板中的代码。                                       |
| %cpaste             | 开启特别提示，手动粘贴待执行代码。                         |
| %reset              | 删除所有命名空间中的变量和名字。                           |
| %page OBJECT        | 美化打印对象，分页显示。                                   |
| %run script.py      | 运行代码。                                                 |
| %prun statement     | 用 CProfile 运行代码，并报告分析器输出。                   |
| %time statement     | 报告单条语句的执行时间。                                   |
| %timeit statement   | 多次运行一条语句，计算平均执行时间。适合执行时间短的代码。 |
| %who, %who_ls,%whos | 显示命名空间中的变量，三者显示的信息级别不同。             |
| %xdel variable      | 删除一个变量，并清空任何对它的引用。                       |

# NumPy Basics: Arrays and Vectorized Computation

NumPy 的全称是 Numerical Python。

NumPy 有这些功能：

- 提供了 `ndarray`
- 不用手动写 loop 也能快速操作整个 array
- 一些数学的库，比如线性代数、傅里叶变换等
- 一个 C API 用于连接 NumPy 和其他用 C、CPP 或者 FORTRAN 编写的库

我想，正是由于它提供了一个 C API，这样 Python 就能够使用其他语言，能够更加高效地处理数据。

`ndarray` 在内存中连续存放，也加快了读写的速度。

看完这一章以后，做一个总结：

1. `ndarray` 的初始化
2. `ndarray` 的索引，尤其是 axis 怎么用
3. 在 `ndarray` 上的各种操作，逐元素的操作
4. 概率和线性代数

下文全部默认导入了包

```python
import numpy as np
```

## ndarray

ndarray 是多维数组，允许使用对标量的语法对整个数组进行操作。

### 创建 ndarray

使用 `np.array(<array like object>)` 创建一个 ndarray

```python
In [3]: data = np.array(range(11))

In [4]: data
Out[4]: array([ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10])
```

二维和更多维度也不在话下：

```python
In [6]: data = np.array([[1,2,3], [4,5,6]])

In [7]: data
Out[7]:
array([[1, 2, 3],
       [4, 5, 6]])
```

还可以创建全 0 和 全 1 的数组：

- `np.zeros()`: 创建全是 0 的数组
- `np.ones()`: 创建全是 1 的数组
- `np.empty()`: 创建不初始化的数组，那么数组里的值就不确定了
- `np.arange()`: 创建类似 Python 中 `range` 方法得到的 array

参数不知道？没关系，使用 ipython 的 introspect 就行，用问号（question mark）问问 ipython。

```python
In [8]: np.zeros?
Docstring:
zeros(shape, dtype=float, order='C', *, like=None)

Return a new array of given shape and type, filled with zeros.

Parameters
----------
shape : int or tuple of ints
    Shape of the new array, e.g., ``(2, 3)`` or ``2``.
dtype : data-type, optional
    The desired data-type for the array, e.g., `numpy.int8`.  Default is
    `numpy.float64`.
order : {'C', 'F'}, optional, default: 'C'
    Whether to store multi-dimensional data in row-major
    (C-style) or column-major (Fortran-style) order in
    memory.
like : array_like, optional
    Reference object to allow the creation of arrays which are not
    NumPy arrays. If an array-like passed in as ``like`` supports
    the ``__array_function__`` protocol, the result will be defined
    by it. In this case, it ensures the creation of an array object
    compatible with that passed in via this argument.
```

可以动手试一试：

```python
In [10]: d1 = np.zeros((10))

In [11]: d1
Out[11]: array([0., 0., 0., 0., 0., 0., 0., 0., 0., 0.])

In [12]: d2 = np.ones((3,4))

In [13]: d2
Out[13]:
array([[1., 1., 1., 1.],
       [1., 1., 1., 1.],
       [1., 1., 1., 1.]])

In [16]: d3 = np.empty((4,3,2))

In [17]: d3
Out[17]:
array([[[6.23042070e-307, 3.56043053e-307],
        [1.60219306e-306, 7.56571288e-307],
        [3.22651328e-307, 8.45599366e-307]],

       [[7.56593017e-307, 1.11261027e-306],
        [1.11261502e-306, 1.42410839e-306],
        [7.56597770e-307, 6.23059726e-307]],

       [[1.42419530e-306, 7.56602523e-307],
        [1.29061821e-306, 1.37961234e-306],
        [9.34598926e-307, 1.33511562e-306]],

       [[9.34600963e-307, 8.45559303e-307],
        [8.06613465e-308, 6.89810244e-307],
        [1.22387550e-307, 2.22522596e-306]]])
```

`array.ndim`: 数组的维度个数

`array.shape`: 数组的 shape，返回的是 tuple 类型

### types for ndarray

`ndarray` 是内存中连续的一块区域，而且每一个元素的类型必须一样，可以使用 `array.dtype` 查看元素类型，这决定了是如何处理这个 chunk memory。

```
In [18]: d0 = np.array(range(11))

In [19]: d0.dtype
Out[19]: dtype('int32')

In [21]: d3.dtype
Out[21]: dtype('float64')
```

类型的命名方法也很好记忆，就是 `int/float/...` + 数字，当然 `ndarray` 也可以存储除了数字之外的其他类型，比如字符串、Object 等。

如果自己传入 `array-like object` 的话，类型会自动推断，使用 `np.zeros` 等方法，因为默认参数，所以是 `float64`

可以使用 `arr.astype(np.<typename>)` 返回一个类型转换后的数组，甚至可以直接做字符串到数字的转换：

```python
In [38]: str_l = [str(random.random()) for i in range(11)]

In [41]: str_arr = np.array(str_l)

In [42]: str_arr
Out[42]:
array(['0.9631762615469753', '0.8514241450325513', '0.3823361720118553',
       '0.7645060173662542', '0.9122788206302417', '0.5435130755772613',
       '0.6749292073749705', '0.32777036495717626', '0.222856565806545',
       '0.081660947528019', '0.45714997943060365'], dtype='<U19')

In [48]: f_arr = str_arr.astype(np.float64)

In [49]: f_arr
Out[49]:
array([0.96317626, 0.85142415, 0.38233617, 0.76450602, 0.91227882,
       0.54351308, 0.67492921, 0.32777036, 0.22285657, 0.08166095,
       0.45714998])
```

上面的例子中使用的类型是 Unicode。作者说要小心使用 `np.string_` 类型，因为 `NumPy` 里的 `string_` 大小固定，可能会悄悄地截断。

### ndarray 的算数运算

对标量（scalar）的操作可以直接运用到数组上，比如 `array *= 3`，那么 `array` 中每个元素就变成原来的三倍。这种免去写循环的操作叫做向量化 vectorization。我想，起这个名字可能是把整个 `array` 看成了一个向量。

这种运算符有很多，比如 `=`、`>` 等，所以看起来有点“不可思议”。

如果运算符是两个大小相等的 array，那么运算会作用于每个元素上。比如 `arr1 + arr2`，得到的结果就像是 $\vec{arr1} + \vec{arr2}$ 一样，每个分量相加。

```python
In [60]: d1 = np.ones((2, 3))

In [61]: d2 = np.ones((2, 3)) * 3

In [62]: d1
Out[62]:
array([[1., 1., 1.],
       [1., 1., 1.]])

In [63]: d2
Out[63]:
array([[3., 3., 3.],
       [3., 3., 3.]])

In [64]: d1 + d2
Out[64]:
array([[4., 4., 4.],
       [4., 4., 4.]])

In [66]: d1 > d2
Out[66]:
array([[False, False, False],
       [False, False, False]])
```

如果是在不同大小的 array 之间运算，这种操作叫做广播（broadcasting）。这本书在附录中做更加详细的讨论。

### 基础索引和切片

`ndarray` 也支持切片，不过切片得到的是原来的引用而不是值拷贝，叫做 view。view 这个概念和数据库的视图很像，这一点和 Python list 的切片不同。

作者认为这是因为 NumPy 被设计为处理大量的数据，所以使用引用这种对内存不贪心的方式。

如果要获取值拷贝，可以使用 `arr.copy()` 方法。

对于取出一个元素来说，索引语法和 Python 的多维 list 相似，`arr[idx1][idx2]...[idxn]`，语法糖是 `arr[idx1, idx2, ..., idxn]`。但是索引不止可以取出一个元素，这种索引方式和切片结合起来，可以灵活地得到自己想要的子集。

可以使用 `reshape(shape)` 改变数组的 shape，这样方便举例子：

```python
In [81]: d1 = np.array(range(12))

In [82]: d1 = d1.reshape(2,3,2)

In [83]: d1
Out[83]:
array([[[ 0,  1],
        [ 2,  3],
        [ 4,  5]],

       [[ 6,  7],
        [ 8,  9],
        [10, 11]]])
```

我觉得索引（index）就是一个“去括号”的过程

```python
In [84]: d1[1, 1:, 0]
Out[84]: array([ 8, 10])
```

`d1[1,...]` 这个 1 就表示要第一个维度的第 1 个元素，就是 2 行 3 列的 `[6...11]`，然后第二个索引是 `1:`，意思是从第二个唯独看，要第 1 个元素到最后一个元素，就是 `[[8, 9],[10, 11]]`，最后是 0，就是要挑出最后一个维度的第 0 个元素。

或许会疑惑，为什么最后一个 0 不是取 `[8,9]`，我感觉这个过程就像串羊肉串一样，比如 `d1[x, y, 0]`，x 和 y 的值随便取，得到值只可能在 0，2，4……10 之内。

所以，`d1[1, 1:, 0] != d1[1, 1:][0]`

```python
In [83]: d1
Out[83]:
array([[[ 0,  1],
        [ 2,  3],
        [ 4,  5]],

       [[ 6,  7],
        [ 8,  9],
        [10, 11]]])

In [84]: d1[1, 1:, 0]
Out[84]: array([ 8, 10])

In [85]: d2 = d1[1, 1:]

In [86]: d2
Out[86]:
array([[ 8,  9],
       [10, 11]])

In [87]: d2[0]
Out[87]: array([8, 9])
```

这种索引方式书中叫做 along the axis

![slice along the axis](https://wesmckinney.com/book/images/pda3_0402.png)

### Boolean Indexing

也叫做 mask，用 boolean list 索引

```python
In [88]: d1
Out[88]:
array([[[ 0,  1],
        [ 2,  3],
        [ 4,  5]],

       [[ 6,  7],
        [ 8,  9],
        [10, 11]]])

In [89]: m1 = [True, False]

In [90]: d1[m1]
Out[90]:
array([[[0, 1],
        [2, 3],
        [4, 5]]])

In [91]: m2 = [True, False, True]

In [92]: m3 = [False, True]

In [93]: d1[m1, m2, m3]
Out[93]: array([1, 5])
```

mask 说白了也是一个 `array-like` 的对象，可以使用 mask，来对 array 中符合某种条件的元素进行操作

```python
In [94]: d1[d1>3] = 0

In [95]: d1
Out[95]:
array([[[0, 1],
        [2, 3],
        [0, 0]],

       [[0, 0],
        [0, 0],
        [0, 0]]])
```

关于 Boolean 的操作符也能用进来，比如 `|` 表示“或”，`&` 表示“与”，`~` 表示“非”。

### Fancy Indexing

- 返回的是 copy
- 传入多个数组索引，返回的结果是一维的

利用数组索引，回想我们索引的方式，本质上就是在 axis 上挑选出符合条件的 row，比如 `[scalar]` 就是挑一个，`[begin:end]` 是挑选一个范围，`[<boolean array>]` 根据挑选真值挑选。既然如此，为什么不能直接指定要挑选哪些 row？比如 `[1,3,5]` 就表示我要第 1、3、5 个 row。

Fancy Index 允许使用 list 来索引，这个 list 还是有序的，意味着可以按照 list 的顺序索引到。但是 Fancy Index 不同于以往的一点是，当使用多维索引的时候，返回的结果是一维的。

```python
In [113]: d1
Out[113]:
array([[ 0,  1,  2],
       [ 3,  4,  5],
       [ 6,  7,  8],
       [ 9, 10, 11]])

In [114]: d1[[2, 1]]
Out[114]:
array([[6, 7, 8],
       [3, 4, 5]])

In [115]: d1[[2, 1], [0, 1]]
Out[115]: array([6, 4])
```

### Transposing and Swapping

中文是转置和轴交换。

装置也只是返回视图，有 `transpose` 方法的数组就有一个叫做 `T` 的属性。

```python
In [124]: d1
Out[124]:
array([[0, 1, 2],
       [3, 4, 5]])

In [125]: d1.T
Out[125]:
array([[0, 3],
       [1, 4],
       [2, 5]])

In [126]: d1.transpose()
Out[126]:
array([[0, 3],
       [1, 4],
       [2, 5]])
```

转置常用在矩阵运算中，矩阵的乘法使用的运算符是 `@`，或者使用 `np.dot(arr1, arr2)`。

转置就是一种特殊的轴交换，比如 `arr.swapexes(axis1, axis2)`，返回一个视图。

## 伪随机数生成

https://wesmckinney.com/book/numpy-basics.html#numpy_random

numpy 提供在不同分布函数上的随机数或者随机数组的生成，在 `np.random` 包内。

随机数生成是使用生成器，这个是可以配置的，`rng = np.random.default_rng(seed=123)` 就是用随机数种子 123 配置一个生成器，并把这个生成器赋值给 `rng` 变量。

这个生成器有很多 api 来方便我们生成一系列的随机数，比如：

`rng.integers(low, high, size)`: 通过 `discrete uniform` 采样，生成从 $[low, high)$ 范围内的，大小为 `size` 的随机数。这个参数选择很灵活，比如 size 不填，就生成一个随机数，如果 `size` 是一个数，就生成一维数组，里面每个数是随机采样的，如果 `size` 是一个 shape，返回的数组就是这个 shape。

## 通用函数

Element-Wise 意思是“逐元素”，虽然运算符可以直接作用于数组就像作用于标量那样，但是函数却不行，比如 `math.sqrt(arr)` 就会报错，不能像我们预期的那样给数组中的每个数开平方根。

通用函数逐个作用于数组中的元素，如：

```python
In [7]: arr
Out[7]: array([ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10])

In [8]: np.sqrt(arr)
Out[8]:
array([0.        , 1.        , 1.41421356, 1.73205081, 2.        ,
       2.23606798, 2.44948974, 2.64575131, 2.82842712, 3.        ,
       3.16227766])
```

有这些通用函数，当然，更详细的用法可以 introspect：

- `np.maximum(arr1, arr2)`: 逐元素取较大的那个，返回一个 `ndarray`
- `np.exp(arr)`: $arr^e$
- `np.modf(arr) -> remainder, whole_part`: 返回小数部分和整数部分

这些函数可以接收一个 out 参数，用来指明把结果返回给谁，类似于传入一个指针。

## 面向数组编程

### 扩充条件表达式作为数组运算

`np.where(cond, xarr, yarr)`: `cond` 是 mask，返回一个数组，逐元素，为真取 `xarr`，否则取 `yarr`。参数可以是数组也可以是标量。

比如把一个数组中小于 0 的部分给抹零：`np.where(arr<0, 0, arr)`

### 数学和统计学方法

数组的就是一堆数据，我们可以对它做统计，既可以使用 `np.<fn>`，也可以使用数组的实例方法

一类方法叫做 reduce，也叫 aggregate，回想 Python 中 reduce 的概念：

```python
reduce(f, [x1, x2, x3, x4]) = f(f(f(x1, x2), x3), x4)
```

就像给数据降维了

- 返回一维数组的平均值：`np.mean(arr)` 或者 `arr.mean()`
- 沿着 axis 对 axis 上逐元素取平均值：`arr.mean(axis=?)`

```python
In [30]: arr
Out[30]:
array([[0, 1, 2],
       [3, 4, 5],
       [6, 7, 8]])

In [31]: arr.mean(axis=0)
Out[31]: array([3., 4., 5.])

In [32]: arr.mean(axis=1)
Out[32]: array([1., 4., 7.])
```

还有一类方法是 cumulate，累积

- `arr.cumsum(axis=a)`: 沿着 axis 对逐元素累加

### 真值数组

真值数组的数字运算，被当作 0 和 1 处理，数字数组的真值运算，0 被当作 False，非零被当作 1

- `boolarr.sum()`:
- `boolarr.any()`: 有一个 True，就返回 True
- `boolarr.all()`: 有一个 False，就返回 False

### 排序

`arr.sort(axis=a)`: 沿着轴逐元素排序，注意排序后，原来在一行上的数可能就不在同一行了。

```python
In [38]: arr = np.arange(9)

In [39]: np.random.shuffle(arr)

In [40]: arr
Out[40]: array([3, 7, 6, 0, 1, 4, 8, 2, 5])

In [41]: arr = arr.reshape(3,3)

In [42]: arr
Out[42]:
array([[3, 7, 6],
       [0, 1, 4],
       [8, 2, 5]])

In [43]: arr.sort(axis=0)

In [44]: arr
Out[44]:
array([[0, 1, 4],
       [3, 2, 5],
       [8, 7, 6]])
```

### Unique and Other Set Logic

针对一维数组而言

- `arr.unique()`: 返回一个去重且**排好序**的数组，类似于 `sorted(set(arr-like))`
- `arr.in1d(arr, y)`: 返回一个 mask，`arr` 中逐元素在不在 y 中

## 文件 IO

`np.save(filename, arr)`: 将 `arr` 保存到 `filename` 的文件中，会自动添加 `.npy` 文件后缀

`np.savez(filename, a=arr, b=arr)`: 读取的时候返回一个字典，根据索引获取对应的 `arr`

`np.load(filename)`: 从文件名为 `filename` 的文件读取，根据文件内容返回对象，dict 或者是 array

`np.savez_compressed(...)`: 压缩保存

## 线性代数

矩阵的 api

矩阵的乘法，`arr1.dot(arr2)`，或者 `np.dot(arr1, arr2)`，或者 `arr1 @ arr2`

很多矩阵运算在 `numpy.linalg` 下，比如 `from numpy.linalg import inv`，那么就可以使用 `inv` 这个函数

- 矩阵的逆：`inv(arr)`
- 矩阵的迹：`trace(arr)`
- 矩阵 SVD 分解：`svd(arr)`

当然，还有很多，不一而足。`linalg` 是线性代数英文 Linear Algebra 的缩写

## 举例：随机漫步

随机漫步这个概念，可以在 Wikipedia 上找到，比如一个人在数轴上的原点上抛硬币，正面朝上的话就朝正方向走一个单位长度，否则往负方向走一个单位长度。我们用 `numpy` 模拟这个过程。

首先假设走 1000 步，记为 `step_cnt = 1000`，我们要生成一个取值只有二值且二值等概率的数组，可以使用随机数 generator

```python
step_cnt = 1000
rng = np.random.default_rng(seed=123)
steps = rng.integers(0, 2, step_cnt) # steps 是 01 数组
steps = np.where(steps == 0, -1, 1) # 0 全部变成 -1
walks = steps.cumsum() # 累加
walks.max() # 查询走到最远
walks.min() # 查询走到最近
```

查询第一次走到 `x=10` 的位置：

```python
(walks>=10).argmax()
```

`argmax` 返回第一个最大值的下标，`walks>=10` 返回的是一个真值数组，真值数组运算时候作为 0 1 数组处理。

如果想要同时模拟多个随机模拟，可以开二维数组，每一行都是一个模拟，根据 mask 来筛选满足自己条件的数组。

# Pandas

https://wesmckinney.com/book/pandas-basics.html

Pandas 设计用于处理表格化或者异构数据，以 NumPy 为基础。

```python
import numpy as np
import pandas as pd
import Series, DataFrame from pandas
```

## 数据结构

Series 和 DataFrame 两种数据结构。

### Series

Series 是对一维表格的抽象。可以理解为有 index 的 array，只有两列。

创建 Series 的一个例子：`obj = pd.Series([1,2,3], index=['a', 'b', 'c'])`，index 默认是 0 到 N-1 的数字。

Series 作为表格的抽象，有 index 属性，用于索引表格中的行，有 array 属性，一般是对 NumPy 数组的封装。

Series 可以看作一个有序字典，所以还可以使用 dict 来初始化一个 Series。

```python
In [9]: obj = pd.Series([1,2,3], index=['a', 'b', 'c'])

In [10]: obj
Out[10]:
a    1
b    2
c    3
dtype: int64

In [11]: obj.index
Out[11]: Index(['a', 'b', 'c'], dtype='object')

In [12]: obj.array
Out[12]:
<PandasArray>
[1, 2, 3]
Length: 3, dtype: int64
```

介绍 Series 的属性：

1. s.name: 表格的名字
2. s.index.name: 索引的名字

Series 上有和 NumPy 类似的操作，比如：

1. 索引，可以使用 mask 索引，也可以使用 index 索引，返回的还是 Series
2. 操作，对 Series 的操作会逐个作用于 `s.array` 中的元素，如有 `np.exp()`，`pd.isna()`，`pd.notna()`

Series 对缺失数据会填补 NaN。

### DataFrame

是对多列表格的抽象，可以看成多个 Series 靠在一起，是二维的。约定，竖下来的轴叫做 index，横着的轴叫做 columns。

创建 DataFrame，怎么理解呢？使用一个 dict，这个 dict 表示每一列（col）叫啥，几个 Series 横着拼在一起称为 DataFrame。

```python
In [18]: students = {'name': ['Alice', 'Bob', 'Can'],
    ...:             'age': [12, 13, 10],
    ...:             'sex': ['M', 'F', 'M']
    ...:             }

In [19]: sdf = pd.DataFrame(students)

In [20]: sdf
Out[20]:
    name  age sex
0  Alice   12   M
1    Bob   13   F
2    Can   10   M
```

从 DataFrame 中获取数据

- 从 DataFrame 中获取一列 Series：`df[column_name]` 或者 `df.column_name`
- 从 DataFrame 中获取一行：`df.loc(index)` 或者 `df.iloc(index)`

修改 DataFrame 中的数据

- `df[col] = scalar`: 一整列都被赋值成一样的
- `df[col] = array_like`: 按顺序被赋值
- `df[col] = series`: 按照 index 相等的方式赋值
- `df[nonexist_col] = sth`: 不存在的列将被创建

### Index Object

index 用来索引行，是一种类型。`series.index` 就可以获得一个 index，index 也可以在构造 series 的时候传入。

`df.columns` 也是 Index

## 基本功能

`reindex`: 交换不同 index 的顺序，或者创建新的 index，对应的值可以是插值获得，也可以是自动填充 NaN

`drop`: 返回一个删除指定 index 或者 columns 的新对象

索引：

例子如下：

```python
In [5]: data = pd.DataFrame(np.arange(16).reshape(4,4),
   ...:                     index=['O', 'C', 'U', 'N'],
   ...:                     columns=['one', 'two', 'three', 'four'])

In [6]: data
Out[6]:
   one  two  three  four
O    0    1      2     3
C    4    5      6     7
U    8    9     10    11
N   12   13     14    15
```

Series 可以像 NumPy 那样，使用 `s[]` 索引，如果 `[]` 中是整数，那么就会被当作默认的下标索引，而不是 index 的值索引。比如 index 里面是字符串，那么使用 `[integers]` 索引也有效。

另一种索引方式是使用 `s.loc[]`，`[]` 中的元素被当作 index 的值索引。`s.iloc[]` 就显式的声明使用 integer 下标索引。

注意，索引的区间是 $[begin, end]$。

DataFrame 的索引和 Series 不同。`df[col]`，使用的索引是 column，返回的是 Series。

`df[i1:i2]` 是通过 row 索引，返回的是 DataFrame

`df[[columns]]` 通过传入列名字的列表，返回 DataFrame

使用逻辑操作可以返回布尔型的 DataFrame，这个可以当作 mask，`df[mask]` 可以筛选符合条件的 row。比如 `df[df[col]>5]` 筛选 col 大于 5 的 row，`df[df>5]` 是把整个 DataFrame 看作 NumPy 的数组来得到一个同等 shape 的 mask。

`df.loc[]` 操作**完全相反**，默认使用 row 索引，我想是因为 `loc` 本身是根据 label 来索引，默认行的名字才叫 label。

`df.loc[row]`: 返回一行。这个要和 `df[col]` 区别开来。返回的结果是行的转置，变成列。

`df.loc[rows]`: 返回一个区域，这不会转置。比如 `df.loc[['row1', 'row2']]`

可以使用 `df.loc[<rows>, <cols>]` 可以返回一个区域。

`df.iloc[]`: 可以把 `df.loc[]` 里面所有的 row label 和 column label 替换成它们对应的数字。

请尽量使用 `loc` 和 `iloc` 来避免 index 是 integers 带来的 selection 的不确定性。

### 运算和数据对齐

两个 Series 相加，如果 index label 都存在且一样，就会被加到一块，否则对于 Missing Data 会填充 NaN，并且传播。
DataFrame 同理。

使用 `df1.add(df2, fill_value=fv)`，就可以使用 `fv` 作为默认的填充值而不是 NaN。类似的，只要函数或者 DataFrame 方法上有 `fill_value` 这个关键词参数，就可以用自定义的填充值。

Series 和 DataFrame 之间的运算涉及到“广播”这个概念。默认是使用 Series 的 index 和 DataFrame 的 columns 匹配，然后逐行操作。相当于 Series 转置以后逐行作用于 DataFrame。这叫做 broadcast across rows。如果想要逐列作用于 DataFrame，可以这样：`df1.sub(s1, axis='index')`，这样就会 broadcast across columns。

我立即为将 Series 的 index 和 DataFrame 的 axis 相匹配。

```python
In [32]: df = pd.DataFrame(np.arange(16).reshape(4,4),
    ...:                   index=['row'+str(i) for i in range(4)],
    ...:                   columns=['col'+str(i) for i in range(4)])

In [33]: df
Out[33]:
      col0  col1  col2  col3
row0     0     1     2     3
row1     4     5     6     7
row2     8     9    10    11
row3    12    13    14    15

In [36]: sr = pd.Series(np.arange(4), index=['row'+str(i) for i in range(4)])

In [37]: sr
Out[37]:
row0    0
row1    1
row2    2
row3    3
dtype: int32

In [43]: df.sub(sr, axis='index')
Out[43]:
      col0  col1  col2  col3
row0     0     1     2     3
row1     3     4     5     6
row2     6     7     8     9
row3     9    10    11    12
```

### 函数应用和映射

要对 Series 或者 DataFrame 逐元素操作，除了 NumPy 那样，还可以把执行接收一个 Series 参数的函数。

`sr.map(fn)`: 逐元素作用

`df.apply(fn, axis='index')`: `fn(series)` 将施加于一列上，along the 'index' axis

`df.applymap(fn)`: 作用于逐元素上，这么起名是因为 Series 可以 map

### 排序和排名

`df.sort_index(axis=a)`: 根据 index label 或者 column label 排序

`sr.sort_values`: 根据 Series 的值排序

`df.sort_values(columns)`: 根据指定 columns 里的 value 给 row 排序

`sr.rank()`: 这个值得注意，如何处理相等关系的值，比如有 n 个值相等，占据了排名 low 到 high，那么这 n 个值就会被分配相同的排名 $\frac{low+high}{n}$，当然这也是可以改的，比如用 `sr.rank(method='first')` 就可以改成严格按照 1 2 3 排序。

```python
In [53]: sr = pd.Series([1, 2, 2, 3, 3, 3, 5, 4])

In [54]: sr
Out[54]:
0    1
1    2
2    2
3    3
4    3
5    3
6    5
7    4
dtype: int64

In [55]: sr.rank()
Out[55]:
0    1.0
1    2.5
2    2.5
3    5.0
4    5.0
5    5.0
6    8.0
7    7.0
dtype: float64

In [61]: df = pd.DataFrame({'val':sr, 'rank':sr.rank()})

In [62]: df
Out[62]:
   val  rank
0    1   1.0
1    2   2.5
2    2   2.5
3    3   5.0
4    3   5.0
5    3   5.0
6    5   8.0
7    4   7.0
```

## 统计

对于 Series 和 DataFrame，pandas 提供了一些统计学方法。比如求和、求平均……

`df.describe()`: 如其名，描述一下数据，给出 count、mean 之列的数据

### 相关系数和协方差

`sr.corr(sr)`: 计算两个 Series 之间的相关系数，比如 `stock[company1].corr[company2]`

`sr.cov(sr)`: 计算两个 Series 之间的协方差

`df.corr()`: 直接返回一个相关系数矩阵

### 唯一值、值计数和 Membership

`sr.unique()`: 返回去重后的 series

`sr.value_counts()`: 对每个出现的 value 进行计数

`df.apply(pd.value_counts)`: 每一列的 value 进行统计。比如第一列，1 出现 2 次，2 出现 4 次。

`df.value_counts()`: 把每一行视为一个 tuple，每种 tuple 在整张表里出现了几次。

# 数据加载、存储和文件格式

## 读取或写入文本格式的数据

https://wesmckinney.com/book/accessing-data.html

有很多函数允许从不同格式的文件中读取数据，比如 `read_csv`，`read_excel`，`read_sql`

index: `name` 参数允许指定 `columns`，`index_col` 指定哪一个或哪几个 col 作为 Index

parse: `sep` 允许指定分隔符，可以用正则表达式，`na_values` 参数可以告诉 pandas 有哪些字符串作为 NaN 处理

`.to_csv(filename, sep)`: 将 DataFrame 写到文件里

太多了，自己看。

## 二进制格数数据

使用 Python 内置的 pickle 模块可以把数据序列化到文件上，`df.to_pickle(filename)` 就行。pickle 慎用，因为它不向前兼容。pandas 内置了转换成 parquet 等格式的支持。

### 读取 Microsoft Excel 文件

`xlsx = pd.ExcelFile(filename)`: 得到一个 Excel 文件的抽象

`xlsx.sheets_names`: 如其名

`xlsx.parse(sheet_name=s)`: 读取指定 sheet_name

### 使用 HDF5 格式

hierarchy data format

先 `conda install pytables`

`store = pd.HDFStore(filename)`: 返回一个 `HDFStore` 的对象，可以像 dictionary 那样工作

对于大量数据的存储，倾向于使用 HDF 格式。

## 与 Web API 交互

使用 request 包获取 response 对象，然后调用 response 对象的 json 方法，返回一个 json 对象，对 json 对象进行数据处理。

## 和数据库交互

Python 内置的 sql 驱动操作起来非常繁琐，可以安装并使用 `sqlalchemy` 这个包，`pandas` 可以用这个包的返回内容。

```python
In [149]: import sqlalchemy as sqla

In [150]: db = sqla.create_engine("sqlite:///mydata.sqlite")

In [151]: pd.read_sql("SELECT * FROM test", db)
Out[151]:
             a           b     c  d
0      Atlanta     Georgia  1.25  6
1  Tallahassee     Florida  2.60  3
2   Sacramento  California  1.70  5
```

# 数据清洗和预处理

https://wesmckinney.com/book/data-cleaning.html

- 缺失、重复数据的处理
- 字符串处理

## 处理缺失数据

缺失数据被叫做 Sentinel，哨兵，擎天柱的师傅也叫做 Sentinel。

float64 的缺失数据用 `np.nan` 表示，显示为 NaN。

`df.isna()` 和 `sr.isna()` 返回 mask，NaN、None 的地方为 True，这里的 NA 意思是 not available，来自 R 语言。

`dropna()`: 默认返回一个新的对象，丢弃含有或全是或有数个 NA 的 row 或者 col

`fillna()`: 使用单个值，或者 index/column - value 的键值对，或者指定插值方式填充 NA

`duplicated()`: 返回一个 Series，当前 row 是否和前面的某一 row 重复

`drop_duplicates()`: 丢掉重复的 row

`map(dict/fn)`: 数组中逐个元素被映射为其他元素

`replace()`: 替换元素

`pd.cut(data, ...)`: 把数据分到不同的 intervals 里，这个区间可以自己指定，也可以由 data 计算出来

`pd.qcut(data)`: 根据 data 中数据的数量，把不同的数分到 bins 里

`df.take(arr)`: 根据 arr 的顺序，对 row 或者 col 重排

`df.sample()`: 采样

`dummies()`: 构造哑变量矩阵

## 扩展数据类型

Pandas 在 NumPy 的基础上建立，但是为了效率之类的原因，有一些自己扩展的数据类型，比如字符串等。

比如 float64 的 NA 用 NaN 表示，Int64 的 NA 用 `<Int64>` 表示。

同时，有自己的字符串类型，可以更快地计算，这就需要使用第三方库了。

## 字符串处理

### pandas 中的字符串函数

用 `Series.str.<fn>`，这是专门设计的，同时也能跳过 NA 值。

```python
In [3]: sr = pd.Series(["alice@gamil.com", "bob@qq.com", np.nan])

In [4]: sr
Out[4]: 
0    alice@gamil.com
1         bob@qq.com
2                NaN
dtype: object

In [5]: sr.str.contains("gmail")
Out[5]: 
0    False
1    False
2      NaN
dtype: object
```

注意类型是 object，这就是“扩展类型”。

`sr.str.findall()`: 类似于 Python 中 str 的正则查找

## 分类数据

分类数据产生的背景：假如一个 Series 里存储了大量重复的数据，比如 `"apple", "orange"` 等，那么我们可以把 distinct 的 values 存储到一个 Series，这样每一个 distinct value 都有唯一的整数表示，让原来的 Series 存储整数。

```
t1:
i   fruit
---------
0   apple
1   apple
2   orange
3   apple
```

用如下的形式存储：

```
t1:
i   fruit-code
------------
0   0
1   0
2   1
3   0

t2:
fruit-code  fruit
-----------------
0           apple
1           orange
```

把一列数据分类，提供类别抽象和每个值的 code。

`Series.astype(category)`: 可以把一列中数据的格式（dtype）转换为 `category`，`Series.array` 是 Categorical 的实例。

`c.categories`: 这个 c 有哪些类别

`c.codes`: 每个 value 的取值，对应于 categories。

前文介绍的 `cut()` 和 `qcut()` 函数就返回了与 Category 有关的结果

```python
In [7]: rng = np.random.default_rng(seed=123)

In [8]: nums = rng.standard_normal(1000)

In [9]: nums[:5]
Out[9]: array([-0.98912135, -0.36778665,  1.28792526,  0.19397442,  0.9202309 ])

In [10]: bins = pd.qcut(nums, 5)

In [11]: bins
Out[11]: 
[(-3.299, -0.767], (-0.767, -0.228], (0.913, 3.058], (-0.228, 0.284], (0.913, 3.058], ..., (-3.299, -0.767], (-0.228, 0.284], (0.284, 0.913], (-3.299, -0.767], (-0.228, 0.284]]
Length: 1000
Categories (5, interval[float64, right]): [(-3.299, -0.767] < (-0.767, -0.228] < (-0.228, 0.284] <
                                           (0.284, 0.913] < (0.913, 3.058]]

In [12]: type(bins)
Out[12]: pandas.core.arrays.categorical.Categorical
```

one-hot 编码用可以用 Category 实现。

# 数据规整：聚合、合并和重塑

创建 DataFrame 或者 Series 的时候，允许使用多个 index

```python
In [14]: df = pd.DataFrame(np.arange(16).reshape(4,4),
    ...:         index=[['a', 'a', 'b', 'b'],
    ...:                [1, 2, 3, 4]],
    ...:         columns=[['w', 'x', 'y','z'],
    ...:                ['k', 'l', 'm', 'n']])

In [15]: df
Out[15]: 
      w   x   y   z
      k   l   m   n
a 1   0   1   2   3
  2   4   5   6   7
b 3   8   9  10  11
  4  12  13  14  15
```

`df.unstack()`: 降维，让 index 只有一列，多的 index 合并到 column 里

`df.stack()`: 从 column 里抽出一个 name 来做 index

`df.set_index(cols)`: 将 cols 拆下来作为 index

有多个 index 或者 column 可以更方便地索引区域

这么多 index，每一竖叫做一个 level，第一竖是 level0，第二竖是 level1，以此类推。

`swaplevel()`: 交换 index

`sort_index(level=l)`: 根据第 `l` 个 level 上的 index 的名字，对 row 进行排序

`pandas.merge(df1, df2)`: 根据相同的 column，把多个 df 沿着 column 的方向拼起来，类似于数据库的 join，默认是 inner join，取 df 的交集，这样能保证没有 NA。

`pd.concat()`: 类似于 stack，让 row 变多，也可以改变 axis 参数，让 col 变多

- `axis`: 沿着哪个方向堆叠
- `keys`: 区分堆叠后的前者

### 合并有重复的数据

拼在一起是根据 index 或者 column 的名字。还有一种合并的方式是，两个表，数据都是缺失的，但是可以重叠在一起，就不缺失了。这时候可以使用 `np.where(pd.isna(a), b, a)`，如果 a 的位置是 NA，就用 b 填补，这种做法无视了 index 的 label。

`df1.combine_first(df2)`: 根据 index 使用 df2 来填补 df1 中的 NA。

## 重塑与轴向旋转

stack 和 unstack，老生常谈了。把 column 的 label 作为值嵌入表格。

pivot 意为绕轴旋转，可以把纵向排序的 label 旋转到 column 上。

# 绘图与可视化

先安装 matplotlib 这个包：`conda install matplotlib`。

matplotlib 目的是在 Python 中像使用 matlab 那样创建图像。有很多衍生的包，比如 `seaborn`。

约定导入简称：

```python
In [13]: import matplotlib.pyplot as plt
```

在浏览器中启动：cmd 中输入 `jupyter notebook` 即可。可以根据配置改变打开的默认目录

这些命令有啥用：

https://ipython.readthedocs.io/en/stable/interactive/plotting.html#id1

只有 inline：无法内嵌显示，但是会返回

两条都不加：内嵌显示，无法交互

只加上 `%matplotlib notebook`: 什么是交互，就是可以显示鼠标在图片上的坐标之类的，需要 `%matplotlib notebook` 这是在指定渲染后端是啥，这样在 figure 上添加 axes 时候可以动态的显示，实时更新。

只加上 inline：啥也没有，和都不加无异

两条都加上：和只加上 inline 无异。

我的推测：

为了防止使用不好的后端渲染，所以 notebook，但是这样又会导致创建交互图片，所以使用 inlie 不要交互。

啥都不加照样用。

## 简短 API 入门

首先 matplotlib 是通过 figure 来组织图片的。figure 相当于一个窗口，可以往里面放置小图片

- `figure`: 相当于一个窗口
- `axes`: 带坐标轴的图表，就像 Excel 里插入的一张图表对象

`ax.plot()`: 在对象 `ax` 这个图表上绘制折线图

`ax.hist()`: 在对象 `ax` 这个图表上绘制直方图

`ax.scatter()`: 在对象 `ax` 这个图表上绘制散点图

`fig.add_subplot()`: 为 fig 这个 figure 对象新增 axes，会把 axes 马上绘制到 figure 上

```python
ax1 = fig2.add_subplot(2, 2, 1)
ax0 = fig2.add_subplot(2, 2, 1)
ax0 is ax1

False
```

可见是不同的 axis 对象

对于图例（legend）、刻度（tick）等，都是可以调节的。

`plt.rc()`: 重置设置，如配色方案、字体大小等

## 使用 Pandas 和 Seaborn 绘图

`df.plot()`: 相当于调用 `df.plot.line()`，DataFrame 和 Series 都有这个方法，默认绘制直线。

对于需要聚合或者累加处理的数据，seaborn 可以很好地处理。

`sns.pairplot()`: 绘制变量散点矩阵，如 `var1-10` 之间的关系可以通过 10 * 10 的矩阵图表示

`sns.factorplot()`: 根据变量的取值，绘制不同的图。

# 时间序列

就是元素表示时间的 Series，使用 Python 的 Datetime 类型制作的 index 会被自动转换为 Pandas 的 Timestamp 类型，并放在 DatetimeIndex 类型的 index 中。

## 选择、索引和子集

`sr['2021']` 选中 Series 中一年的数据

`sr['2021':]`: 选中 2021 年及其之后的数据

## 日期区间、频率和移位

可以使用 `pd.date_range()` 生成时间序列：

```python
In: pd.date_range(start="2012-04-01", periods=2)
Out: DatetimeIndex(['2012-04-01', '2012-04-02'], dtype='datetime64[ns]', freq='D')
```

有 start end periods 和 freq 需要指定。

```python
In [88]: pd.date_range("2000-01-01", periods=10, freq="1h30min")
Out[88]: 
DatetimeIndex(['2000-01-01 00:00:00', '2000-01-01 01:30:00',
               '2000-01-01 03:00:00', '2000-01-01 04:30:00',
               '2000-01-01 06:00:00', '2000-01-01 07:30:00',
               '2000-01-01 09:00:00', '2000-01-01 10:30:00',
               '2000-01-01 12:00:00', '2000-01-01 13:30:00'],
              dtype='datetime64[ns]', freq='90T')
```

freq 可以取固定长度的时间，比如 1 小时、或者 1 天，也可以用来表示一种“偏移”，这种偏移不是固定长度的，也没有什么好的术语，书中使用 anchored offset 表示。比如 `freq='M'`，就是每个月的最后一天，而不是 30 或者 31 天。`freq=BM` 就是每个月的最后一个工作日，这种日期之间没有固定的时间间隔。

## 时区处理

pandas 集成了 pytz 包进行时区管理

默认没有时区，所以要先给时间序列一个时区。

使用 `ts.tz_localize("UTC")` 可将 ts 的时间序列从 None 到 UTC 时区，使用 `ts.tz_convert()` 可进行不同时区的转换。

## Period

表示区间。`p = pd.Period("2011", freq="A-DEC")` 表示 2011-01-01，新年第一刻，到 2011 年 12 月的最后一天，具体时刻我也不知道。

我觉得就是从一个给定的时刻开始计算，比如 `2011`，时间开始往后走，看看满不满足 freq 表示的时间。freq 实际上是偏移，找到了满足 freq 的时刻以后，再根据 freq 表示的时间长度取出一个区间。

2011, freq='A-JUN' 表示的是 2010 年 7 月到 2011 年 6 月。

如果不知道这个 Period 表示的日期区间，可以通过 `p.asfreq("D", how="end/start")` 查看。

### 季度

参考 [Fiscal Year End](https://zhuanlan.zhihu.com/p/20572421)，中国人要理解的关键在于，财务报表季度不一定是 1-3，4-6 这样的，可能 2-4 才是第一个季度，或者说最后一个季度。

`pd.Period("2012Q4", freq="Q-JAN")` 是说：2012 年的第四个季度，要在一月份（January）最后一天结束。所以该季度的跨度是从 2011 年的 11 月 1 日到 2012 年的 1 月 31 日。

Period 和 Timestamp 之间可以相互转换

## 重采样和频率转换

高频到低频叫做 downsampling，需要聚合数据，一般 resample 之后还要再调用一个函数，形如 `ts.resample().<fn>()`。

重采样需要指定开闭方向和显示的结果要用左边还是右边。分别使用 `close` 和 `label` 表示。

低频到高频需要填值，可以指定插值方式。

## 滑动窗口

可以在 Series 上创建一个滑动的窗口，`sr.rolling(size)`，这很抽象，返回的是一个 Window 对象。

`sr.rolling(3).mean()` 就是三个为一组，比如 (0,1,2) 的 mean，(1,2,3) 的 mean，`(2,3,4)` 的 mean ....

size 可以是时间，更多用法查看 docstring。

除了在单个窗口上调用函数，可以对两个窗口上调用函数，比如求 corr，`sr1.rolling(size).corr(sr2)`，会自动匹配大小，从 sr1 中抽取一个个窗口，和 sr2 求相关系数。

最后，可以自定义函数，使用 `sr.rolling(size).apply(<fn>)`，`fn` 将作用于 array。

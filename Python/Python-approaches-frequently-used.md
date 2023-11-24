Python 常用方法

[toc]

# help 命令

以下来自 GPT：

在 Python 中，你可以使用`help()`函数来获取关于 Python 语法和模块的帮助信息。`help()`函数接受一个对象作为参数，它可以是函数、类、模块、关键字等。

以下是几种使用`help()`函数获取 Python 语法帮助信息的方式：

1. 获取内置函数的帮助信息：

```python
help(len)
```

这会打印出关于`len()`函数的帮助文档，包括函数的定义、参数说明和返回值等。

2. 获取模块的帮助信息：

```python
import math
help(math)
```

这会打印出关于`math`模块的帮助文档，包括模块的概述、函数和常量的说明等。

3. 获取关键字的帮助信息：

```python
help('for')
```

这会打印出关于`for`关键字的帮助文档，包括关键字的用法和说明。

4. 获取对象方法的帮助信息：

```python
my_list = [1, 2, 3]
help(my_list.append)
```

这会打印出关于`list`对象的`append()`方法的帮助文档，包括方法的定义、参数说明和返回值等。

通过`help()`函数，你可以获取各种 Python 语法元素的帮助文档，从而了解它们的使用方式和说明。请注意，帮助文档通常会在终端或交互式环境中显示，你可以向下滚动并按`q`键退出帮助信息的显示。

# basic type

## 近似判断 float 是不是 int

一般来说，判断是否整数：`num % 1`，1.5 对 1 取余就是 0.5，整数对 1 取余就是 0。
但是计算机使用二进制计算，当看起来结果为 1 时，
其实可能是 `1.00001`，也可能是 `0.9999`，还可能真的是 `1`，所以近似处理：

```python
>>> (33.8 - 1.8)/32 % 1
0.9999999999999999
>>> (33.8 - 1.8)/32 % 1 == 0
False
```

处理的函数：

```python
def is_int(f: float):
    if f%1 == 0:
        return True
    remain = abs(f)%1
    if remain <= 0.0001 or 1- remain <= 0.0001:
        return True
    else:
        return False
```

## 小数格式化输出

keywords: 精度，小数点后

其实也有 round 方法，但是一般都说「保留小数点后两位」，round 如果 ends with 0 再输出就不一定两位了，round 还是留着保留到整数吧。

比如保留两位小数输出：

```python
f = 1.234
print('%.2f'%f)
```

round 示例：

```python
>>> f = 1.235
>>> print('%.2f'%f)
1.24
>>> f = 1.20
>>> f = round(f, 2)
>>> print(f)
1.2
```

格式化字符串：

```python
num = 3.14159
digits = 3
formatted = f"The number is {num:.{digits}f}"
print(formatted)
# 输出：The number is 3.142
```

注意，保留不一定是四舍五入

## 利用 Decimal 实现高精度

```python
>>> from decimal import Decimal
>>> Decimal(0.1)
Decimal('0.1000000000000000055511151231257827021181583404541015625')

>>> Decimal('0.1')
Decimal('0.1')

>>> Decimal('0.1') + Decimal('0.2')
Decimal('0.3')

>>> Decimal('0.3') == 0.3
False

>>> float(Decimal('0.1'))
0.1
```

## 字符串技巧

1. `input().split()`
2. `join()`: `' '.join([1,2,3])`，快速格式化输出一组数，中间带空格，最后不带空格
3. `find(substr)`: 查找子串出现的位置
4. `if sub_str in s:`：判断是否存在子串
5. 反转: `s[::-1]`

```python
integer_list = [int(i) for i in input().split()]
```

# 输入

对于要结束符的输入，比如像 C++ 中，Windows 平台用 `^Z` 表示输入结束，可以 `if (cin >> i_val)` 判断是否读取到了末尾，python 用 try except，或者精准捕获 EOFError。

```python
try:
    s = input()  # 读取用户输入
except EOFError:
# except Exception as e: # 如果不小心忘记了具体的 Exception Error 叫啥可以使用这一行
    print("用户输入了文件结束符 (^Z)")
else:
    print("用户输入了：", s)
```

# methods of List

## index()

index() 函数用于从列表中找出某个值第一个匹配项的索引位置。

index() 语法：

```python
list.index(x[, start[, end]])
```

获取第一个最简单了，`list.index(x)`

请确保元素`x`在 list 中，否则会报错。

## sort()

`list.sort()`：将改变列表中的元素位置，返回值为 none，默认升序排列。

`sorted(list)`：不改变列表元素原有顺序，返回排好序的列表

### key 与自定义排序

要自定排序方法，可以用 lambda 表达式

```python
lst = [(2, 1, 3), (1, 2, 3), (4, 3, 2)]
lst.sort(key = lambda x: x[0], reverse = False )
```

`list.sort(key = function, reverse = boolean)`

本意是接受一个 function，这个 function 的参数是 list 这个“可迭代对象”中的对象，返回一个数作为比较的依据。

```python
lst = [(2, 1, 3), (1, 2, 3), (4, 3, 2)]
def index2(item):
    return item[2]
lst.sort(key = index2, reverse = False )
```

一定一定要把`key = xxx, reverse = xxx` 写上。

### 多级排序：让 key 返回一个 tuple

多依据排序装载自[此](https://blog.csdn.net/y12345678904/article/details/77507552)。
通过返回一个 tuple 直接进行多级排序

对 tuple 进行排序，先按照第一个元素升序，如果第一个元素相同，再按照第二个元素降序排列。

```python
L = [(12, 12), (34, 13), (32, 15), (12, 24), (32, 64), (32, 11)]
L.sort(key=lambda x: (x[0], -x[1]))
print(L)
```

## 列表的解析

`[expr for iter_var in list if cond_expr]`

按我个人理解：

`[func(i) for i in list if filter(i)]`

三个关键：expression、for-in、if

下面是样例：

### n 个 0 的列表

可以利用 list 的解析生成一个含有 n 个 0 的 list

`lst = [0 for i in range(n)]`

### 字符串数组转 int 数组

`[int(i) for i in s]`

### 利用字典转换 key_list

这就是利用了列表的解析

```python
dct = {'a':3, 'b':3,'c':5,'d':3}
key_list = ['c', 'd', 'a', 'b', 'd']
val_list = [dct[k] for k in key_list]
val_list = [5, 3, 3, 3, 3]
```

## 统计

可迭代对象（Iterable）有`count()`方法，可以统计从指定位置开始对指定的对象进行计数

`list.count(object,start,end)`，start 和 end 是可选参数

以统计 list 中元素出现个数并保存到字典中为例：

### 方法一

利用 collections 中的`Counter`这个类的构造方法，再强转为 dictionary 类型

```python
import collections
d = dict(collections.Counter(list))
```

### 方法二

```python
dic = {}
for i in lst:
	dic[i] = lst.count(i)
```

这样的缺点是遍历 lst 时会有重复，当然，也可以 set(list) 去重后再 count。

## 删除特定的元素

特别说明一个错误的方法：

```python
for i in list_A:
	if condition:
		list_A.remove(i)
```

删除一个元素后，list 的长度变化，两个连续要被删除的元素，后面那个躲过去了，没被删除。

### 方法一

使用`filter`这个类的**构造函数**，再将结果转换为 list 即可

```python
def is_odd(n):
    return n % 2 == 1

flt = filter(is_odd, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
newlist = list(flt)
print(newlist)
```

当然可以配合 lambda 函数

### 方法二

从列表的后面往前面删除特定的元素

### 方法三

遍历一个列表的拷贝，进行删除

```python
for i in lst[:]:
	if condition:
		lst.remove(i)
```

## 合并

1. `+`：如 `list_a + list_b`
2. `listA.extend(listB)`

# regular expression

有些概念经常忘记，所以有必要记录。
跳过大部分正则表达式语法的部分。

使用 re 库，就是一个 util 库

-   `re.match(pattern, string)`: 从字符串的开头开始匹配，相当于 pattern 已经开头被加上了 `^`
-   `re.search()`
-   `re.findall(pattern: str, text: str)`:

## 一点语法

### 字符集的补集

当 `^` 出现在 `[]` 内部，表示使用补集，不要和在开头的语义混淆

> [^0-9-] 是一个字符类（character class）的定义，用于在正则表达式中匹配除了数字字符和减号之外的任意字符。
> 具体解释如下：
> `[^...]` 表示字符类的否定形式，即匹配不在方括号内的任意字符。
> 0-9 表示数字字符的范围，即匹配从 0 到 9 的任意数字字符。
> `-` 表示减号字符本身。
> 因此，`[^0-9-]` 表示匹配除了数字字符和减号之外的任意字符。

### 出现次数

切记，`{7,  }` 和 `{7,}` 是不一样的，前者多了一个空格

## 捕获与非捕获

如果在 pattern 中使用了 `()`，那么 `findall` 就会**捕获**之，将 `()` 匹配的内容放到结果中，就不纯洁了。
如果不希望被捕获，可以使用非捕获组：`(?:...)`

```python
import re
p1 = "[0-9]+(\.\d+)?" # float
p2 = "[0-9]+(?:\.\d+)?"

s = "10.1"

print(re.findall(p1, s)) # ['.1']
print(re.findall(p2, s)) # ['10.1']
```

小数中，我们希望小数点 `.` 后面一定要跟着数字，于是把它们用括号放一起 `(\.\d+)?`，但是这样 `findall` 结果中又会把这个括号内匹配的结果返回给我们。
但是我们不需要这个结果，所以就使用 `(?:\.\d+)` 告诉它别匹配了，我只是要求其必须一起出现而已。
当 pattern 内没有指定任何捕获组时候，自己就是第一个捕获组，如果有至少一个捕获组，自己就不是捕获组。

```python
import re
p1 = r"[0-9]{3, }" # can't match because the space after '3'
p2 = r"[0-9]{3,}"
s = "1234"

print(re.match(p1, s) is not None) # False
print(re.match(p2, s) is not None) # True
```

## 引用捕获

比如匹配 html tag，`<div>.*</div>`

```python
m = r'(((ab*)c)d)e\3' #要求 ab*cde 后面跟着第三分组的内容
r = re.match(m,"abbbcdeabbbkfg") # 后面的 bbb 少一个 b 则不能匹配，因为第三分组是 abbb
```

## 指定长度/前瞻后顾

匹配是会消耗字符的，比如电话号码格式为 `(dd)-ddd`,像 `(12)-123` 就符合，`(12)-1234`因为是 `1234`有四位就不符合

因为只要三位数字，可以使用 `\d{3}[^0-9]`,表示三位数字后面不需要再一个数字。

但是对于 case: `(01)-123(01)-123` 里面有两个符合的电话号码，如果使用 `\(\d{2}\)-\d{3}[^0-9]` 会因为 `[^0-9]` 消耗掉 `3` 后面的 `(`,所以要 peak 一下前面

以下来自 ChatGPT:

> Python 的正则表达式支持前瞻（lookahead）和后顾（lookbehind）断言，它们允许在匹配时进行条件判断，但并不消耗字符串或作为匹配结果的一部分返回。

> 以下是前瞻和后顾断言的基本语法：

> 前瞻断言（Positive Lookahead）：(?=pattern)
> 前瞻断言用于在匹配位置之后，查看该位置后面的内容是否符合指定的模式 pattern。如果符合，则匹配成功，否则匹配失败。
> 负前瞻断言（Negative Lookahead）：(?!pattern)
> 负前瞻断言用于在匹配位置之后，查看该位置后面的内容是否不符合指定的模式 pattern。如果不符合，则匹配成功，否则匹配失败。
> 后顾断言（Positive Lookbehind）：(?<=pattern)
> 后顾断言用于在匹配位置之前，查看该位置前面的内容是否符合指定的模式 pattern。如果符合，则匹配成功，否则匹配失败。
> 负后顾断言（Negative Lookbehind）：(?<!pattern)
> 负后顾断言用于在匹配位置之前，查看该位置前面的内容是否不符合指定的模式 pattern。如果不符合，则匹配成功，否则匹配失败
> 需要注意的是，前瞻和后顾断言只是用于进行条件判断，它们本身并不消耗字符串或作为匹配结果的一部分返回。它们只影响匹配的成功或失败。

电话的应该是：`r'\(\d{2}\)-\d{3}(?=([^0-9]|$))'`

# weekday - calendar

使用 `calendar` 这个类来判断某年某月某日是周几：

关键方法：`calendar.weekday(year, mon, day)`，支持负数年份

```python
import calendar
n = int(input())

for i in range(n):
    year, mon, day = [int(j) for j in  input().split()]
    try:
        res = calendar.weekday(year, mon, day)
        weekdays = ["Monday","Tuesday","Wednesday","Thursday", "Friday","Saturday", "Sunday"]
        print(weekdays[res])
    except:
        print('Illegal')
```

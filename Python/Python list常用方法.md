Python list 常用方法

+ index
+ sort
+ 列表的解析
+ 统计：count()
+ 删除指定元素
+ 合并列表

# index()

index() 函数用于从列表中找出某个值第一个匹配项的索引位置。

index()方法语法：

```python
list.index(x[, start[, end]])
```

获取第一个最简单了，`list.index(x)`

请确保元素`x`在list中，否则会报错。

# sort()

`list.sort()`：将改变列表中的元素位置，返回值为 none，默认升序排列。

`sorted(list)`：不改变列表元素原有顺序，返回排好序的列表

要自定排序方法，可以用lambda表达式

```python
lst = [(2, 1, 3), (1, 2, 3), (4, 3, 2)]
lst.sort(key = lambda x: x[0], reverse = False )
```

`list.sort(key = function, reverse = boolean)`

本意是接受一个function，这个function的参数是list这个“可迭代对象”中的对象，返回一个数作为比较的依据。

```python
lst = [(2, 1, 3), (1, 2, 3), (4, 3, 2)]
def index2(item):
    return item[2]
lst.sort(key = index2, reverse = False )
```

一定一定要把`key = xxx, reverse = xxx` 写上。

多依据排序装载自[此](https://blog.csdn.net/y12345678904/article/details/77507552)。

对tuple进行排序，先按照第一个元素升序，如果第一个元素相同，再按照第二个元素降序排列。

L = [(12, 12), (34, 13), (32, 15), (12, 24), (32, 64), (32, 11)]
L.sort(key=lambda x: (x[0], -x[1]))
print(L)


# 列表的解析

`[expr for iter_var in list if cond_expr]`

按我个人理解：

`[func(i) for i in list if filter(i)]`

三个关键：expression、for-in、if

下面是样例：

## n个0的列表

可以利用list的解析生成一个含有n个0的list

`lst = [0 for i in range(n)]`

## 字符串数组转int数组

[int(i) for i in s]

## 利用字典转换key_list

这就是利用了列表的解析

```python
dct = {'a':3, 'b':3,'c':5,'d':3}
key_list = ['c', 'd', 'a', 'b', 'd']
val_list = [dct[k] for k in key_list]
val_list = [5, 3, 3, 3, 3]
```

# 统计

可迭代对象（Iterable）有`count()`方法，可以统计从指定位置开始对指定的对象进行计数

`list.count(object,start,end)`，start和end是可选参数

以统计list中元素出现个数并保存到字典中为例：

## 方法一

利用collections中的`Counter`这个类的构造方法，再强转为dictionary类型

```python
import collections
d = dict(collections.Counter(list))
```

## 方法二

```python
dic = {}
for i in lst:
	dic[i] = lst.count(i)
```

这样的缺点是遍历lst时会有重复，当然，也可以set(list)去重后再count。

# 删除特定的元素

特别说明一个错误的方法：

```python
for i in list_A:
	if condition:
		list_A.remove(i)
```

删除一个元素后，list的长度变化，两个连续要被删除的元素，后面那个躲过去了，没被删除。

#### 方法一

使用`filter`这个类的**构造函数**，再将结果转换为list即可

```python
def is_odd(n):
    return n % 2 == 1
 
flt = filter(is_odd, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
newlist = list(flt)
print(newlist)
```

当然可以配合lambda函数

#### 方法二

从列表的后面往前面删除特定的元素

#### 方法三

遍历一个列表的拷贝，进行删除

```python
for i in lst[:]:
	if condition:
		lst.remove(i)
```

# 合并

1. `+`
2. `listA.extend(listB)`
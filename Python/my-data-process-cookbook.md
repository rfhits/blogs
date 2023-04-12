我的数据处理 cookbook

Datetime: 2023-03-27T16:54+08:00

Categories: Python

从毕设开始，记录自己使用 pandas 等库的一点心得，给自己查阅，零零碎碎，不成文章。最佳阅读方法是使用 `Ctrl + F`。

[toc]

# 碎碎念

## 注释

当要使用 jupyter notebook 对数据进行分析时候，写上注释，或者使用 markdown 语法。我觉得 PyTorch 入门教程里那些 notebook 很有借鉴的经验。每一个 notebook 的第一个 cell 就是向读者介绍文件要做什么。

-   对于多个 cell 组成的 block，使用 markdown 表示当前进行到了哪一步，不然很容易忘记 notebook 在干嘛，第二次阅读是一场折磨。
-   对于单个 cell 内，也要写注释，如 `#region` 和 `#endregion`。

## 流程拆分

数据处理是 dirty work，有些数据非常棘手。在动手前先尽可能想好如何处理数据，我有时会写出如下几个文件：

-   data_process.ipynb：处理数据，如填充缺失值等
-   data_analysis.ipynb：分析数据，如数据分布等
-   data_process.py：处理数据的脚本，用来和 process.ipynb 对拍
-   encode_data.ipynb：数值化数据

这当然不是最佳实践，it depends ....

分析和处理本身就是交织进行的，也不见得每个 ipynb 都需要附带一个 python 脚本。
因为如果可以 ipynb 里面写 markdown，ipynb 跑完，数据处理好了，各种 graph 也画好了，比较漂亮。

## 流程的优化

处理的流程也可以有优化的空间，有时候把磁盘 io 写到 for 循环里了，实际上可以提取出来。

## 代码的简洁

我觉得 Python 有一个很好的语法糖：

```python
[fn(i) for i in range(10)]
```

这对数据处理简直就是福音，不需要在 for 循环里 append

# notation

X: DataFrame，表示训练用的 features: $\{feat_1, feat_2, ..., feat_n \}$

y: Series/DataFrame，表示最后要得到的值。

# index 操作

## read and save with index

index 很重要，我吃的第一个亏就是不管 index

`pd.read_csv(path_to_csv, index_col=index_col)`

`pd.save_csv(path_to_csv, index=True)`

## give index a name

`df.index.name = 'index_name'`

## set index to 0,1,2,...

这个操作叫做 `reset_index`，意思是让默认的 index 回来，原来的 index 变成一个 column 合并到原来的 DataFrame 中。

## union, diff, ...

类似集合的交并差，pandas 提供了相应的接口，如 [difference](https://pandas.pydata.org/docs/reference/api/pandas.Index.difference.html)、[union](https://pandas.pydata.org/docs/reference/api/pandas.Index.union.html) 等，可以不用自己手写 `isin()`

如 `index1.difference(index2)` 返回在 `index1` 中而不是 `index2` 中的元素

## reindex

这是一个非常重要的操作，`df1.reindex(new_index)`，让 df1 的 index 变成 new_index，可以用来改变顺序

1. 训练模型的时候，`y.reindex(X.index)`
2. 拆分数据子集，`sub_y = y.reindex(X_sub.index)`

# column 操作

## 起名与修改

起名：

-   请不要使用数字作为 column 的名字，如 1，0 等企图以类别 id 为 column 的名字
-   请不要使用 `sum`、`class`、`size` 等关键字作为 column 的名字，这叫做「[避讳](https://zh.wikipedia.org/zh-sg/%E9%81%BF%E8%AE%B3)」

改名：

`df.rename(columns={'id': 'user_id'}, inplace=True)`

注意 `inplace` 这个字段得是 `True`，或者找一个新的 DataFrame 来接收，不然到时候可能找不着修改后的名字然后报错

## 修改 columns

### 修改列的顺序

这个操作常常发生在 merge 之后对 columns 进行重新排序

参考 https://stackoverflow.com/a/58686641

```python
col = df.pop("Mid")
df.insert(0, col.name, col)
```

# 多表

## 联表（merge）

merge 有一个坑，就是如果使用 left_index 和 right_on 来合并，最后得到的 DataFrame 的 index 是 right 的 index

```ipython
In [3]: x = pd.DataFrame(data={'x_col1': [3,4,5]})

In [4]: y = pd.DataFrame(data={'y_col1': [0,1,2]}, index=[7,8,9])

In [5]: x
Out[5]:
   x_col1
0       3
1       4
2       5

In [6]: y
Out[6]:
   y_col1
7       0
8       1
9       2

In [7]: x.merge(y, left_index=True, right_on='y_col1')
Out[7]:
   x_col1  y_col1
7       3       0
8       4       1
9       5       2
```

merge 得到的这个 DataFrame 的 index 竟然来自 right？

所以 merge 我大多数都是使用 `right_index=True, left_index=True` 。

### 合并多个表

merge 只能合并两个表，有时候要合并一堆表

`pd.concat([df1, df2, ...], axis=1, how='inner')`，StackOverflow 上有对应问题

# 异常值处理

## 甄别缺失值

`df[col].unique()`，如果这一列本身类别就不多，那么可以直接看出是否具有没有意义的值

`df[col].dtype`，可以看看 dtype 是不是 object，如果这列本应该是数值类（numeric）数据，但是类型却为 object，就要注意了

查看 na：`df.isna().sum()`，`df.isna().sum().sum()`

## 处理缺失值

`df.fillna(val, inplace)` 直接把整个二维表上的缺失值都填充了

使用 `df.loc` 填充缺失值。

`df.loc[df[col].map(is_missing), col] = value`

## 甄别 inf

`np.isinf(df).sum().value_counts()`

`X_train.replace([np.inf], 50, inplace=True)`

# 查

## 查找符合条件的 row

复杂情况使用 map 来获得一个 mask，然后再将 mask 作用于原来的 df 上，得到查找的结果

```python
student['name'].map(len) > 2 # 得到一个 mask
stu_name_len_gt2 = student[student['name'].map(len) > 2] # 通过 mask 得到名字长度大于 2 的学生
```

## 重复数据

是否有重复数据：`df.index.duplicated().any()`

`df.duplicated(subset=[], keep=)`

通过 mask 找到重复数据

通过 drop 直接删掉重复数据

## 统计最大

`Series.value_counts().idxmax()`

## 迭代

参考：https://zhuanlan.zhihu.com/p/339744795

# 删

我一般是先通过“查”来找到要删除的数据，确认无误后，再通过 index 删除

`del df[col]`

`df.drop(columns=cols, axis=1, inplace=True)`

`df.drop(del.index, inplace=True)`

# 增

做特征工程的时候需要增，常用的比如 map、apply

map 可以把一个 column 映射到一个新的 column，比如 `male` 到 `1`，`female` 到 `0`。

apply 通过调节 axis 参数可以接受一个 col 或者 row 当作对象，`function(row) = value`，每个 row 就能产生一个新的 value

`school['male_to_female_ratio'] = school.apply(lambda sch: sch['male_cnt'] / sch['female_cnt'], axis=1)`，我只是举个例子，可能出现除数为 0，可以参考下面的 apply。

# 杂

## 类型转换

比如转换成数字

`df['col'] = df['col'].astype(int)`

参考：https://blog.csdn.net/zgljl2012/article/details/54880353

## 空白符替换

有时候 string 里面不能有空白符，比如使用树模型和 `graphviz` 画出一棵决策树，每个 attribute 就不能有空白符，可以替换成下划线：

对于某一列：`df[col].replace(' ', '_', regex=True, inplace=True)`

对于 dataframe 的 columns name：`df.columns = df.columns.str.replace(' ', ' ')`

对于整个 dataframe：`df.replace(' ', '_', regex=True, inplace=True)`

## one-hot 编码

`pd.get_dummies(X, columns=[col1, col2])`

## 随机取数

有内置的 random 模块和第三方 numpy.random

有几个注意点：

1. seed
2. replace：取出来的结果是否唯一
3. 内置 random 叫做 sample，numpy 叫做 choice

```python
import random
random.seed(26)
random.sample([i for i in range(10)], k=5)
```

```python
import numpy as np
rng = np.random.default_rng(26)
rng.choice([1,2,3,4], size=2, replace=False)
```

还有一种方法是 shuffle，好处是可以拿到剩下的没被选中的

```python
size = 3
arr = [i for i in range(10)]
rng.shuffle(arr)
arr[:size]
arr[size:]
```

# 接口速查

## groupby

-   原理：label 到 name 的 map
-   结果叫做 grouped
-   迭代方式
-   grouped.filter

label 就是每一行的 index 的名字，df.index 就是一堆 label

groupby 的原理是传递一个 label 到 group name 的 map，参考 [splitting-an-object-into-groups](https://pandas.pydata.org/pandas-docs/stable/user_guide/groupby.html#splitting-an-object-into-groups)，同样 name 的 row 就到一起了。

`df.groupby('a')` 是 `df.groupby(df['a'])` 的语法糖，建立了每一行的 label 到 `df['a']` 的映射。

所以要直接 groupby index，可以 `df.groupby(df.index)`。

group 的结果命名为 `grouped_*`

grouped 迭代通过 `for name, group in grouped:`

例子：

```ipython
In [4]: df = pd.DataFrame({'col1': [1,0,0,0], 'col2': [0,0,1,0]}, index=[0,0,1,1])

In [5]: df
Out[5]:
   col1  col2
0     1     0
0     0     0
1     0     1
1     0     0

In [6]: df.groupby(df.index).sum()
Out[6]:
   col1  col2
0     1     0
1     0     1

In [7]: df.groupby(df.index).max()
Out[7]:
   col1  col2
0     1     0
1     0     1

In [8]: df.groupby(df.index).min()
Out[8]:
   col1  col2
0     0     0
1     0     0

```

### grouped.filter

可以直接从 grouped 再恢复为 DataFrame

`grouped.filter(lambda group: fn(group))` fn(group) 为真的会留下来

## apply

首先要明确 apply 接受的是一个能够作用于 array 对象的函数，这个按照文档就可以理解，我遇到的问题是使用 if else 在逐元素上，解决方法在 [stackoverflow](https://stackoverflow.com/questions/71912955/pandas-dataframe-valueerror-the-truth-value-of-a-series-is-ambiguous-when-usi/71913288#71913288) 上

`df.apply(lambda x: if x > 0 return 1 else 0)`，然后报错了，因为这个时候，x 是 series，`df.apply(np.sqrt)` 能够 work，我猜测是因为 broadcast 机制

不报错的写法是 `df.apply(lambda s: s.map(lambda x: if x > 0 return 1 else 0))`

正确的写法是 `df.mask(df > 0, 1)`

接口太多了

## agg

聚合 aggregate，agg 和 aggregate 是一样的，字少而已。

我想要 reduce 的 bitwise operator，这个主要还是 NumPy 的神奇接口

`df.groupby(k).agg(lambda x: np.bitwise_or.reduce(x.values))`

我觉得可以更短：`df.groupby(k).agg(np.bitwise_or.reduce)`

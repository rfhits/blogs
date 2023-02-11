SettingWithCopyWarning 笔记

Datetime: 2023/02/11T17:18:00+08:00

Categories: Python

参考 [SettingWithCopyWarning in pandas: Views vs Copies](https://realpython.com/pandas-settingwithcopywarning/)

# TL;DR

关键要明白什么时候返回 copy，什么时候返回 view。

slice, index 一般是 view，mask 一般是 copy。但是也有例外，比如 Pandas 数据类型不一致时侯，slice 和 index 也可能返回 copy。

如果真的确定要修改数据，使用 `loc`，也要注意别写错了，`df.loc[mask]` 照样返回 copy。

# 触发 warning

创建一个 mask，这样就能过滤 df 得到 df2

在 df2 上修改，`df[mask][col] = 0`，得到 warning

当使用 `df[mask]` 时候直接返回了一个中间临时 DataFrame，是 df 的 copy。

修改临时变量没有意义，因为没有变量会 ref to 一个临时变量。

只需要交换顺序，`df[col][mask] = 0` 将直接修改原来数据。因为 `df[col]` 返回的不是临时变量，而是直接指向原来的 df，返回的是 view。

> The rules used by pandas to determine whether or not you make a copy are very complex.

所以请使用 loc，这种方式叫做“accessors”。就我个人来说，我并不想知道“complex rules”具体是啥。

如何使用 loc？这是一个反例：`df.loc[mask][col] = 0`，因为 `df.loc[mask]` 返回了一个临时 copy 变量，正确的方式是 `df.loc[mask, loc] = 0`。

# 从 Numpy 讲起

关于细节，需要从 numpy 讲起。

浅拷贝叫做 shallow copy，也叫做 view，深拷贝就是 copy。

我怎么知道当前的这个数据是 view 还是 copy？看 `data.flags.owndata`，当前数据到底有没有拥有自己的一份，有就是 `True`，没有就是 `False`。

也可以看 `data.base`，如果自己是 view，说明自己肯定被指向了某个东西；如果自己是（deep）copy 得到的，base 就是 None，我就是我，不指向别人。

```python
>>> copy_of_arr = arr.copy()
>>> copy_of_arr
array([ 1,  2,  4,  8, 16, 32])

>>> copy_of_arr.base is None
True

>>> copy_of_arr.flags.owndata
True
```

“修改原始数组会影响其视图，反之亦然”。

`df[mask]` 筛选了很多列，返回的是 copy，`df[col]` 返回的是 view。

`df.copy(deep=True)`，如果 `deep=False`，返回的是 view，否则就是 copy。

这么说来实在是太累了，一个叫做 copy 的函数，返回了 view。

如何确定一个 DataFrame 是 copy 还是 view，可以转换成 numpy，再观察 base 判断是 copy 还是 view：

```python
>>> view_of_df.to_numpy().base is df.to_numpy().base
True
>>> copy_of_df.to_numpy().base is df.to_numpy().base
False
```

三个特殊方法，Pandas 和 Numpy 都实现了，我们经常使用之：

-   `.__getitem__()` references values.
-   `.__setitem__()` assigns values.
-   `.__delitem__()` deletes values.

它们在什么时候调用呢？

-   `var = obj[key]` is equivalent to `var = obj.__getitem__(key)`.
-   `obj[key] = value` is equivalent to `obj.__setitem__(key, value)`.
-   `del obj[key]` is equivalent to `obj.__delitem__(key)`.

让我想起 `len(obj)` 实际是调用了 `obj.__len__()`

`arr[begin:end:step]` 返回 view，`arr[<arr>]` 返回 copy，`arr[mask]` 返回 copy。

> -   Slicing arrays returns views.
> -   Using index and mask arrays returns copies.

# 进入 Pandas

Numpy is clear，但是 Pandas 就不一样了：

> pandas heavily relies on NumPy arrays but offers additional functionality and flexibility. Because of that, the rules for returning views and copies are more complex and less straightforward. They depend on the layout of data, data types, and other details. In fact, pandas often doesn't guarantee whether a view or copy will be referenced.

“pandas often doesn't guarantee whether a view or copy will be referenced”。

pandas `df[[col1, col2]]` 返回 copy。

pandas 中 `df[mask]` 返回 copy。

最扯的是，有时候，在 copy 上修改，它不 issue the warning，不在 copy 上改，它反而 issue warning。

`df[:3]["z"] = 0`，直接修改原数据，但是报 warning

`df.loc[["a", "c", "e"]]["z"] = 0`，在 copy 上修改，但是不报 warning。

我们目前说的的都是在单一数据类型上的修改，是否返回 copy 还取决于数据类型。

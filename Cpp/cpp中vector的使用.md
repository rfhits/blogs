# vector的使用

## 创建

```cpp
    vector<int> a;
```

## 增

```cpp
	a.push_back(3);
	a.insert(a.begin(), 4);
	a.insert(a.begin()+1, 5);
```

## 删

erase一个迭代器，然后删除之，pop_back是删除最后一个元素，clear直接清空所有。

```cpp
    a.erase(a.begin());
    a.pop_back();
    a.clear();
```

## 改

直接下标修改和通过迭代器修改。

```cpp
	a[0] = 100;
	*a.begin() = 50;
```

## 查

通过下标或者迭代器访问元素即可，`.back()`访问最后一个元素的值

```cpp
	a.push_back(1);
	cout <<a.back() << endl;
	cout <<  a[1] << endl;
	cout << *(a.end()-1) << endl;
```

## 迭代删除

利用erase返回下一个元素的特性，删除完以后，让it向后移动一下。

```cpp
	a.clear();
	for (int i = 0; i < 10; i++) a.push_back(i);
	auto it = a.begin();
	while (it != a.end()) {
		if (*it >= 5) {
			it = a.erase(it);
		} else {
			it++;
		}
	}

	for (auto i : a) {
		cout << i << " ";
	}
```

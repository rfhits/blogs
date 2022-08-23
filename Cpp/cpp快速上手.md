## 起因

在没有任何了解的csp的情况下，报了这个考试，还选的cpp。

现在看了题目，满肚子的后悔，一来是cpp语法不熟，二来是看样子我撑死做出两道题。

所以语法很重要。

## 头文件

头文件很重要，

```cpp
#include <iostream>
#include <vector>
#include <set> 
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstring>
#include <math.h>
using namespace std;

const int INF = 0x3f3f3f3f;
```

## 输入输出

### 读取输入的整数

在保证以空格分割的情况下，`cin >> a`，就是读入一个数到a里。

`cin >> a >> b >> c`，逐个读入。

### 读取输入的字符串

直接使用cin流，只能拿到空格分割的字符串。

```cpp
string s;
cin >> s;
cout << s;
```


使用getline，可以读取到空格。

```cpp
int main() {
	string s;
	getline(cin, s);
	cout << s;
	return 0;
}
```


## 数据结构的操作

```cpp
int main() {
	pair<int, int> a={1,2};
	vector<int> vec_a  = {1,2,3};
	vec_a.push_back(4);
	unordered_map<string, int> votes;
	pair<string, int> p = {"Alice", 5};
	votes.insert(p);
	cout << a.second << endl;
	cout << vec_a[0] << endl;
	cout << votes["Alice"] << endl;
	return 0;
}
```
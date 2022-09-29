[toc]

> 在没有任何了解的csp的情况下，报了这个考试，还选的cpp。
>
> 现在看了题目，满肚子的后悔，一来是cpp语法不熟，二来是看样子我撑死做出两道题。

# 头文件

头文件很重要，

```cpp
#include <iostream> // io 流
#include <cstring> // 处理char*风格的字符串
#include <vector>
#include <stack>
#include <set> 
#include <algorithm>
#include <unordered_map>
#include <math.h>
using namespace std;

const int INF = 0x3f3f3f3f;
```

# 输入输出

输入输出使用的头文件为 `<iostream>`，更为推荐的是使用 `printf`和 `scanf`，因为 `cin`和 `cout`太慢了，这里只做介绍。

使用下面的语句可以让cin cout和printf和scanf效率变快，参考[sync_with_stdio和cin.tie(0); cout.tie(0);](https://www.cnblogs.com/xwh-blogs/p/12602671.html)

```cpp
ios::sync_with_stdio(false);
cin.tie(0);
cout.tie(0);
```

## 读取输入的整数/小数

在保证以空格分割的情况下，`cin >> a`，就是读入一个数到a里。`cin >> a >> b >> c`，逐个读入。

```cpp
#include<iostream>
using namespace std;
int main() {
	int i_var;
	double d_var;
	cin >> i_var >> d_var;
	cout << "input int number is " << i_var << endl;
	cout << "input double number is " << d_var << endl;
	return 0;
}
```

很智能，可以自动转换输入的字符串为数字类型。以下是一个样例：

```bash
1
1234.66788
input int number is 1
input double number is 1234.67

--------------------------------
Process exited after 5.21 seconds with return value 0
请按任意键继续. .
```

## 读取输入的字符串

字符串使用 `char[]`存储，而不是通过 `<string>`使用cpp的类，否则会慢。

关于 `<cstring>`和 `<string>`的区别，见[difference between cstring and string](https://cplusplus.com/forum/general/38801/)。

> Well, `<cstring>` is basically a header containing a set of functions for dealing with C-style strings (char*). `<string>`, on the other hand, is header that allows you to use C++-style strings (std::string), which can do a lot of if not all of the functions provided in `<cstring>` on their own.

直接使用cin流，只能拿到空格分割的字符串。

```cpp
#include<iostream>
using namespace std;
int main() {
	char s1[20] = {0};
	char s2[20] = {0};
	cin >> s1 >> s2;
	cout << "s1: " << s1 << endl;
	cout << "s2: " << s2 << endl;
	return 0;
}
```

以下是一个样例，可见输入的字符串被空白符分割了。

```bash
hhh 11 445
s1: hhh
s2: 11

--------------------------------
Process exited after 4.601 seconds with return value 0
请按任意键继续. . .
```

使用 `gets(s)`就能获取一行的输入，这就是c的范畴了。

### 使用scanf读取string

如果一定要使用scanf，则需要为string预留空间。并且是 `&s[0]`而不是 `&a`。

```cpp
//#include <algorithm>
#include <iostream>
using namespace std;
int main() {
	string a;
	a.resize(3);
	scanf("%s", &a[0]);
	cout << a << endl;
	return 0;
}
```

# stl的使用

## algorithm

swap可以交换两个元素

```cpp
#include <algorithm>
#include <iostream>
using namespace std;
int main() {
	int a[2] = {0,1};
	cout << "before swap\n";
	cout << a[0] << endl << a[1] << endl;
	swap(a[0], a[1]);
	cout << "after swap\n";
	cout << a[0] << endl << a[1] << endl;
	return 0;
}
```

# container

+ stack
+ vector

有一些函数是重复的：

`size()` 返回容器的大小

`empty()`：就是 `.size()==0?`

## vector

## stack

> stack s = {1,2,3} // 我写的第一行 stack 代码
> 随手一写，三个错误，cpp 有多牛就不必多说了
> 正确的写法：`stack<int> s({1,2,3});`

`top()`: 返回栈顶元素的引用

`pop()`:  弹出栈顶元素

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

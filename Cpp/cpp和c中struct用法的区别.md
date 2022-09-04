cpp和c中struct用法的区别

# c中的struct

## 不使用typedef

C语言中，定义struct的语法如下：

```
struct [<tag>] { 
    <member-list>
    <member-list>
    <member-list>
    ...
} [<variable-list>];
```

也可以不给这种struct起名字，只是定义变量，比如下面这样：

```c
#include<stdio.h>

struct {
	int age;
} a;

int main() {
	a.age = 1;
	printf("a.age is %d", a.age);
	return 0;
}
```

也可以先起好名字，然后再通过 `struct <tag> <variable-list>;`声明变量。

```c
#include<stdio.h>

struct student {
	int age;
};

int main() {
	struct student a;
	a.age = 1;
	printf("a.age is %d", a.age);
	return 0;
}
```

注意，这个时候，就一定要加上 `struct student a;`，而不能 `student a;`不能少打一个 `struct`。

如果要少打的话，只能通过 `typedef struct <tag> <type>`自己指定。

```c
#include<stdio.h>

struct student {
	int age;
};

typedef struct student Student;
int main() {
	Student a;
	a.age = 1;
	printf("a.age is %d", a.age);
	return 0;
}
```

更绝的是，`typedef struct <tag> <type>`里，`<type>`和 `<tag>`可以同名。

比如定义了一个student的tag，可以通过 `typedef struct student student;`，这样也是合法的。

```c
#include<stdio.h>

struct student{
	int age;
};

typedef struct student student;
int main() {
	student a;
	struct student b;
	a.age = 1;
	b.age = 2;
	printf("a.age is %d\n", a.age);
	printf("b.age is %d\n", b.age);
	return 0;
}
```

## 使用typedef

`typedef`如果遇上了 `struct {balabala}`，那就和定义变量没有任何关系了，就是纯粹的换名字。

```
typedef struct [<tag>] {
	<member-list>
	...
	<member-list>
} <type>;
```

用了typedef后，既可以通过 `struct <tag> <var>;`的方式声明变量，也可以通过 `<type> <var>;`的方式声明变量。

```c
#include<stdio.h>

typedef struct student{
	int age;
} Student;

int main() {
	Student a;
	struct student b;
	a.age = 1;
	b.age = 2;
	printf("a.age is %d\n", a.age);
	printf("b.age is %d\n", b.age);
	return 0;
}
```

这样的设计，真是让人吐槽。好的语言应该只有一种写法。

# cpp中的struct

cpp里，当使用如下的语法来定义一个 `struct tag`时

```
struct [<tag>] { 
    <member-list>
    <member-list>
    <member-list>
    ...
} [<variable-list>];
```

**既可以**像c语言那样通过 `struct <tag> <var>;`来声明一个变量，

**也可以不用写** `struct`，而是通过 `<tag> <var>`来声明一个变量。

```cpp
#include <iostream>
using namespace std;

struct student {
	int age;
};

int main() {
	student m;
	struct student n;
	m.age = 1;
	n.age = 2;
	cout << "m.age is " << m.age << endl;
	cout << "n.age is " << n.age << endl;
	return 0;
}
```

# 参考

[C 结构体](https://www.runoob.com/cprogramming/c-structures.html)

[struct和typedef struct彻底明白了](https://www.cnblogs.com/qyaizs/articles/2039101.html)

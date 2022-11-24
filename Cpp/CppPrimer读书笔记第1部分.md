CPP Primer 5th Edition 读书笔记第 1 部分

[toc]

# 快速入门

## 初窥输入和输出

C++ 的输入和输出依赖于标准库

`iostream` 库的基础是两种**类型**，`istream` 和 `ostream`

标准库定义了四个 IO 对象，`cin`、`cout`、`ceer` 和 `clog`，注意我们平时写 `cout << "hello";` 的时候，`cout` 是对象。

expression 由操作符和操作数组成，然后返回一个值

`cout << "hello";` 使用了 `<<` 操作符，返回的值是 `cout`，所以我们才能连起来写：`cout << "hello" << endl;`。

## 注释

注释对不能嵌套，如 `/* /* */ */`，因为 `/* /* */` 被识别成了一对，所以最后的 `*/` 就多出来了。

可以从状态机的角度来理解

-   初始状态 0
-   状态 1，表示读取到了一个双引号 `"`
-   状态 2，表示读取到了 `/*`
-   状态 1 需要再读取 `"`，才能回到状态 0，期间所有的字符都认为是字符串的一部分
-   状态 2 需要再读取 `*/`，才能回到状态 0，期间所有读取到的字符都认为是注释的一部分

## 类

`obj.method()`，这个 `.` 也是一种操作符，右操作数是类的方法。

# 变量和基本类型

## 基本内置类型

### 类型转换

当我们赋给无符号类型一个超出它表示范围的值时，结果是初始值对无符号类型表示数值总数取模后的余数。

例如，8 比特大小的 unsigned char 可以表示 0 至 255 区间内的值，如果我们赋了一个区间以外的值，则实际的结果是该值对 256 取模后所得的余数。因此，把-1 赋给 8 比特大小的 unsigned char 所得的结果是 255。

当我们赋给带符号类型一个超出它表示范围的值时，结果是未定义的（undefined）。此时，程序可能继续工作、可能崩溃，也可能生成垃圾数据。

反正不要混用就对了

### 字面

为了兼容 C 语言，C++中所有的字符串字面值都由编译器自动在末尾添加一个空字符

字面型常量：就是直接书写出来的，如 `42`，`'h'`，`"hello"`，具有默认类型，

当加上一些特定的后缀时，可以改变对应的类型，比如 `L'a'`，类型就是 `wchar_t`

## 变量

### 初始化

初始化和赋值是两种不同的操作。

C++11 整了个“花括号初始化”，既可以初始化变量，也可以赋值变量。

> 当用于内置类型的变量时，这种初始化形式有一个重要特点：如果我们使用列表初始化且初始值存在丢失信息的风险，则编译器将报错。

以后我都打算用 `{}` 来初始化了。

### 声明与定义

声明指规定变量的类型和名字，让变量为程序所知。

定义包括声明，并且申请了存储空间

`extern int i` 就是声明，但是，`extern int ix = 1024` 是定义。

一旦赋了值，就不是声明了。

个人认为，这种行为应该直接报错。

## 复合类型

引用和指针

引用就是起一个别名，如 `int &ref = i`，就是给内存里的 `i` 的地址，起了一个叫做 `ref` 的名字

`ref = 10; cout << i;`，结果将会是 `10`。

引用在定义的时候，必须指定引用（ref to）到谁。

### 指针

指针具有 4 种状态

1．指向一个对象。
2．指向紧邻对象所占空间的下一个位置。
3．空指针，意味着指针没有指向任何对象。手动置为 `nullptr`
4．无效指针，也就是上述情况之外的其他值。如不初始化指针

当指针放到 condition expression 中，表示是否为空指针。

没错，CPP 可以用特殊的值判断是不是空指针，数值上，`nullptr` 等于 0。

`if (p) // whether p is nullptr`

```cpp
if (p == 0) {
    std::cout << "nullptr equals to 0" << std::endl;
} else {
    std::cout << "no" << std::endl;
}
```

```cpp
int *p1 = 0; // 合法，编译器不会报错
int *p2 = 32; // 不合法，提示 error: invalid conversion from 'int' to 'int*'
```

还有一种指针是 `void*`，无法访问内存空间中的对象，我猜测可以从编译的角度理解，因为编译器在面对形如 `(ptr_to_obj).method` 的语句时，并不知道要调用哪一个方法。

### 定义复合类型

`int a = 1024, *p = &a, &b = a; `，这句定义里，p 是指针，r 是引用。

看起来，这些定义共用了一个 `int`。

所以千万不要写成 `int* p, a`，容易看错，这样可不是“共用”了 `int*`，共用的还是 `int`。

## const

为常量绑定引用，表示不能通过引用修改常量

太烧脑了，当常量和复合类型结合，有三种需求：

1. 不能通过 **指针/引用** 修改对象
2. 不能修改 **指针/引用**
3. 同时满足需求 1 & 需求 2

```cpp
// 不能通过指针修改对象
const int x = 5;
int y = 5;
const int *p = &x; // 显式告诉编译器，指向的是 const
// *p = 6; // error
p = &y;
```

```cpp
// 指针一旦定义，不能再改变指向
int x = 5;
int y = 5;
int *const p = &x; // * 在 const 前，表示不能修改该指针
*p = 6;
// p = &y; // error
```

由此诞生了 `top-level` 和 `low-level` 的概念：顶层就是这个对象不可变，如 `int *const`，底层表示指向的那个对象不可变，也就是不能通过解引用等方式修改“指向”的那个对象。

P56 页提及了一种记忆的方法：

> 试试这样想吧：所谓指向常量的指针或引用，不过是指针或引用“自以为是”罢了，它们觉得自已指向了常量，所以自觉地不去改变所指对象的值。

当我们把 const 写到最外层，就是认为这个指针**指向**了一个常量对象，就不能用这个指针去修改这个对象。

书里提及了从右往左读 $\leftarrow$。

现在给出同时满足需求 1 和 需求 2

```js
const int x = 5;
const int *const p = &x;
```

看一段代码：

```cpp
int main()
{
    int i = 0;
    int *const p_top = &i; //不能改变 p_top 的值，这是一个顶层 const
    const int ci = 42; //不能改变ci的值，这是一个顶层 const
    const int *p_low = &ci; //允许改变 p_low 的值，这是一个底层 const
    const int *const p_full = p_low; //靠右的 const是顶层 const，靠左的是底层 const
    const int &r = ci; //用于声明引用的 const 都是底层 const
    i = ci;
    p_low = p_full;

    // int *p = p_full; // error，需要声明 p 是 low-level
    const int *p = p_full; // correct

    p_low = p_full;
    p_low = &i;

    // int &r = ci; // error，需要声明 r 是 low-level
    const int &r = ci; // correct

    const int &r2 = i;
    return 0;
}
```

-   底层 const 赋值给别人，要保证别人也是底层 const
-   顶层 const 肯定不能被二次赋值

### const expr

能够在编译时候算出来的，就是 const expr。可以进行 **常量传播** 优化。

看书上说的，手动指定一个变量是 `constexpr`，可能有助于编译器进行常量传播？所以才导致 `constexpr int *p` 变成 top-level？

## 处理类型

强调一个误区

`typedef char *pstring`：这是把 pstring 作为一种 指向 char 指针的类型

我们在看到 `pstring ptr` 时，或许会通过人眼把 `pstring` 替换成 `char *`，但是 `pstring ptr` 和 `char *ptr` 是不等价的。

在这条语句中，前者的基本数据类型是 pstring，后者是 char，如果定义多个变量，含义就不同了。

定义变量的时候，通过 `=` 右边来推段类型，就使用 `auto`；

除此以外，还可以使用 `decltype(f()) x = y `，根据 `f()` 的返回值来决定为 `x` 确定什么类型

`auto` 的 **一般会** 忽视 top const，low const 被保留。

话说真的有人会把 auto 和 const 结合起来使用吗，我认为这一部分只是该书为了“全面”才写的，实际编码中，这种使用方式只会增加心智负担，和 auto 的原意背道而驰。

`decltype(*p) c; // p is a pointer to int, c must be initialized`

> 另一方面，如果表达式的内容是解引用操作，则 decltype 将得到引用类型。正如我们所熟悉的那样，解引用指针可以得到指针所指的对象，而且还能给这个对象赋值。因此，decltype(\*p) 的结果类型就是 int&，而非 int。

关于 decltype 返回的类型，可以参考 P121：

> 使用关键字 decltype（参见 2.5.3 节，第 62 页）的时候，左值和右值也有所不同。如果表达式的求值结果是左值，decltype 作用于该表达式（不是变量）得到一个引用类型。举个例子，假定 p 的类型是 int\*，因为解引用运算符生成左值,所以 decltype(\*p)的结果是 int&。另一方面，因为取地址运算符生成右值，所以 decltype(&p)的结果是 int\*\*，也就是说，结果是一个指向整型指针的指针。

## 头文件

遇到 `#include`，会把对应的头文件拷贝过来，代替 `#include`

`#ifndef` 和 `#define` 可以防止头文件被重复定义。

# 字符串、向量和数组

## 命名空间的 using 声明

`using std::cin` 后，可以直接使用 `cin`，而不必每次指定 `std::cin`。

## string

直接初始化：通过构造函数，传入参数初始化，如 `string('a', 10)`

拷贝初始化：我理解为编译器正确处理**赋值**这种行为

[【原创】c++拷贝初始化和直接初始化的底层区别](https://www.cnblogs.com/cposture/p/4925736.html)

[请问 c++中 直接初始化和拷贝初始化 有什么区别?](https://www.zhihu.com/question/384181483)

### string 的行为

`cin >> str;`，`cin` 会忽略空白符。如果输入 `\space hello \space` ，得到的结果是 `str = "hello"`。

为了获取空白符，可以使用 `getline()` 函数。

注意 `getline` 读到换行符停止，**同时把换行符也给读进来了**。也就是说，换行符也在输入缓冲区里，没有留到后面。不过在赋值给字符串的时候，会把换行符删掉。

`string.size()`，返回的类型是 `string::size_type`，不是 `int`，也不是 `unsigned`。

如果我们要存储 string 长度，就要用 `decltype(str.size()) len = 100;`，这样最标准。

### 处理字符

可以在遍历时候，定义为引用类型

`for(auto &c : str)` 和 `for(auto c : str)` 是有区别的。

前者是引用 str 中的字符，进行修改可以直接改动 str，后者不行。

当然，也可以使用 `str[index]` 访问字符串中特定位置的字符，但是在注意使用 `index` 的时候，一定要保证 `index` 的类型为 `string::size_type`，`index` 的范围不能超过 `str.size()`，不然是 **未定义** 行为，不会报错的。

[字符串常量总结](https://blog.csdn.net/weixin_42165647/article/details/114703791)

## vector

`vector` 是一个**类模板**，CPP 还有**函数模板**。

初始化还是使用大括号把，如 `vector<string> svec{"a", "an"};`，做到统一。

除此以外，可以给 `vector` 指定大小，然后会用 `<T>` 的默认值去初始化，如 `vector<int> ivec(10)`，初始化后，里面有 10 个 0。

花括号用来指定花括号内的值来初始化元素，圆括号用来指定 `vector` 要初始化的元素数量和元素的值。

至于 `vector<string> svec{10, "hi"}` 和 `vector<string> svec(10, "hi")` 的意思竟然一样，我认为是不好的设计。

如果有一种简洁的方法可以实现之，就应该禁用其他所用方法。

语法糖：`vec0 == vec1`，若元素数量相等且每个位置对应元素相等，则容器相等。

## 迭代器介绍

`auto b = vec.begin(), e = vec.end()`

容器都有迭代器，b 是一个迭代器，指向第一个元素，e 也是一个迭代器，尾元素下一位置

迭代器提供了一些运算：获取元素、比较元素和移动迭代器

我认为，当把遍历容器结合 OOP 的思想，迭代器就诞生了，从获取容器内部元素的角度来说，迭代器和指针没有什么不同，但是迭代器支持移动和比较。

使用迭代器将字符串改为大写：

```cpp
for (auto it = s.begin(); it != s.end() && !isspace(*it); ++it) {
    *it = toupper(*it);
}
```

如果使用引用，就这么写：

```cpp
for (auto &c : s) {
    if (isspace(c)) {// pass
    }
    else {
        c = toupper(c);
    }
}
```

### 迭代器类型

支持迭代器的一系列操作，就是迭代器类型。

以容器 `vector<int>` 为例，定义它的迭代器时，写作 `vector<int>::iterator`，这也隐含了 `vector` 容器支持迭代器的那一套操作。

如果容器是只读的，那么迭代器类型就是 `vector<int>::const_iterator`

### 访问成员

CPP 提供了语法糖，对于字符串， `(*it).empty()` 因为优先级原因，括号必不可少。

为了简化”解引用“，也就是 `*it` 这个操作，cpp 提供了箭头运算符，`it->empty()` 就和上述一样了。

## 数组

如果清楚元素的确切个数，就使用数组。

```cpp
unsigned int size = 14; // 虽然能跑，这样不好
int a[size];
constexpr unsigned int csize  = 14;
int b[csize]; // 这样好
```

### 数组的初始化

不初始化数组，里面的东西不是默认值。如：`int a[10];` 该数组中不一定放了 10 个 0。

字符数组的初始化会在结尾加上一个空白符：`char s[] = "hello"`。

### 引用类型

每当涉及到类型的时候，必谈引用和指针。那么如何来定义以下类型？（注意，不存在存放引用的数组，因为引用不是内存上的一块地）

-   存放指针的数组
-   指向数组的指针
-   指向数组的引用

在“理解复杂的数组声明”这一小节中，告诉我们从变量名开始看：

-   存放指针的数组：`int *arr[10]`
-   指向数组的指针：`int (*parr)[10]`
-   指向数组的引用：`int (&rarr)[10]`

我认为看两点：

1. 有没有括号
2. 因为声明语句都要有基本类型，所以要看 `int` 后面有没有 `*`

由此，我们得出了指向一个存放指针的数组的引用的定义：`int * (&rparr)[10]`

### 访问数组元素

使用 `size_t` 类型，该类型在 `cstddef` 中。

> 它被设计得足够大以便于表示任何对象的大小

如果要遍历数组中的所有元素，最好使用范围 for 语句，因为这样可以“减轻人为控制遍历过程的负担”。

```cpp
for (auto i : ivec) {
    cout << i << endl;
}
```

### 指针与数组

曾经我以为，数组就是指针，但是它们其实是不一样的。编译器只是在一些情况下，把数组转化为了指针。

举一个这种转换发生的例子：

```cpp
int iarr[] = {1, 2, 3};
auto p(iarr);
```

第 2 行，`iarr` 被转换成了指针，所以 `p` 其实是一个指向 `iarr[0]` 的指针

还有一些情况，编译器不会做这种转换，比如使用 `decltype` 时：

```cpp
decltype(iarr) iarr2 = {1, 2, 3};
```

#### 指针也是迭代器

我们能够创建尾后指针，并且像在迭代器里那样使用比较和递增，通过 for 循环遍历。但是这种方式不太好，C++ 11 引入了 `begin()` 和 `end()` 两个函数，创建头指针和尾后指针

```cpp
int ia[] = {1, 2, 3};
int *bptr = &ia[0];
int *eprt = &ia[3];
for (int *p = bptr; p < eprt; p++)
{
    cout << *p << endl;
}

for (int *p = begin(ia); p < end(ia); p++)
{
    cout << *p << endl;
}
```

指针可以使用负数的下标，如

```cpp
int a[] = {1,2,3};
int *p = &a[2];
cout << p[-1] << endl;
```

### C 风格字符串

用 char 数组定义字符串的时候，一定要在末尾加上 `\0`，否则将会在内存一直寻找下去，直到发现 `\0`。

```cpp
char s[] = {'c', 'p', 'p'};
cout << s << endl;
```

## 多维数组

多维数组其实是数组的数组，如 `int a[3][4]`，`a[0]` 表示了一个有 4 个元素的数组，可以 `int (&r)[4] = a[0]`，但是不可以 `int (*p)[4] = a[0] `。

在使用范围 for 遍历数组时，要小心数组被转换为指针（P114）。

```cpp
for (auto row : ia) // row is a pointer
    for (auto col : row) {
        // ...
    }
```

第一行 row 变量被转换为了指针，因为你想，要把 `a[0]` 的值 copy 一份到内存里，那么这个值就是一个地址，此时 col 就没法遍历一个地址。

所以在使用的时候，除了最内层的 for，其他都应该使用引用类型。

遍历的时候，不要被 `<type> identifier` 这种写法限制了，P116 的练习挺好

```cpp
int main()
{
    int ia[3][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    // a range for to manage the iteration
    for (const int(&p)[4] : ia)
        for (int q : p) cout << q << " ";
    cout << endl;

    // ordinary for loop using subscripts
    for (size_t i = 0; i != 3; ++i)
        for (size_t j = 0; j != 4; ++j) cout << ia[i][j] << " ";
    cout << endl;

    // using pointers.
    for (int(*p)[4] = ia; p != ia + 3; ++p)
        for (int* q = *p; q != *p + 4; ++q) cout << *q << " ";
    cout << endl;

    return 0;
}
```

# 表达式

要弄清楚运算符的优先级、结合律和求值顺序

优先级决定了不同运算符，哪一个先得到运算符的结果；当优先级相等的时候，结合律决定运算符的运算方向

乘法除法满足左结合律，所以从左向右执行

求值顺序指的是：`f1() op f2()` ，哪一个函数先被执行

> 运算对象的求值顺序与优先级和结合律无关,在一条形如 f() +g () \*h ()+j()的表达式中:
> ·优先级规定，g()的返回值和 h ()的返回值相乘。
> 结合律规定，f()的返回值先与 g()和 h ()的乘积相加，所得结果再与 j()的返回值相加。
> ·对于这些函数的调用顺序没有明确规定。
>
> P123

`m % n` 的符号和 m 相同

赋值运算符，返回的是左边的运算对象，也就是左值。满足右结合律，所以赋值顺序是从右往左。

前置递增和后置递增的区别除了在返回值以外，后置递增是以 return a copy 的方式实现的，所以没事别用。

## 位运算符

取反: 就是反引号 `\``

对负数进行位运算，符号位的处理是“未定义”行为，所以一律使用 `unsigned`

## sizeof 运算符

sizeof()

-   `sizeof(*<uninit ptr>)` 是被允许的
-   `sizeof(arr)` 中，arr 不会被转换为指针，而是直接给出整个数组的大小

`sizeof(arr)/sizeof(*arr)` 返回数组中元素的数量

## 类型转换

P141

记录强制类型转换，CPP 使用 `cast-name <type> expression` 来表示强制类型转换

-   static_cast: 静态的转换，类型明确，如 `double dval = static_cast<double> ival`
-   const_cast: 可以将常量对象转换为非常量对象，从而进行写行为，但是这是 UB
-   reinterpret_cast: 重新解释一个对象，注意是同一个对象，而且类型也变化了，如 `int *p; char *p = reinterpret_cast<char*> p`

# 语句

## 范围 for 语句

P169

范围 for 语句来自于传统的、与之等价的 for 语句。范围 for 语句预存了 end() 的值，所以如果在 for 中修改被遍历对象的长度，end 函数的值可能变得无效。

## try 语句块和异常处理

### throw

通过 `throw` 来 raise 一个异常，如 `throw runtime_error("error")`

```cpp
try {
    program-statements
} catch (exception-declaration) {
    handle-statements
} catch (exception-declaration) {
    handle-statements
} // ....
```

如：`catch(runtime_error err) { /* handle statements */}`

关于异常有哪些，参考 P176

# 函数

## 局部对象

形参和函数体内部定义的变量称为“局部变量”。局部变量的生命周期依赖于定义的方式。

自动对象：到达“块”的结尾，就被销毁，对于形参，到达函数的结尾，就被销毁

局部静态对象：第一次经过对象定义语句被初始化，直到程序结束才被销毁。

## 参数传递

如果形参是引用类型，则绑定到实参上，否则就会拷贝实参。

引用类型：我们称之为“传引用调用”

拷贝：我们称之为“传值调用”

在 CPP 中，建议使用传引用调用，而不是使用 C 语言中的传指针值调用

### 尽量使用常量引用

如果已知函数内部不改变参数，那么参数应该加上 const，原因有：

1. 防止给调用者一种函数修改了引用的错觉
2. 可以接收 const 对象作为函数的参数

一个对 CPP 新手容易犯的错：

```cpp
func(string& str);
int main() {
    func("hello"); // 将会报错，因为 "hello" 是个常量，传进去了改不了
    return 0;
}
```

### 数组作为函数的参数

P193

数组会被转换为指针传入函数

指针这种东西是由类型的，比如 `int (*arr)[10]` 就是在说指针指向一个数组，数组长度为 10

```cpp
void ptr1D(int (*arr)[10])
{
    cout << "[10]" << endl;
    return;
}

void ptr2D(int (*arr)[5][10])
{
    cout << "[5][10]" << endl;
    return;
}

int main()
{
    int arr[5][10];
    ptr1D(arr);
    ptr2D(&arr);
    cout << "arr: " << arr << endl;
    cout << "&arr: " << &arr << endl;
    cout << "&arr[0]: " << &arr[0] << endl;
    return 0;
}
```

虽然 `arr`、`&arr` 和 `&arr[0]` 数值上一样，但是是不同的类型。

我认为，函数中直接填写参数 `arr`，会被自动转换为 `&arr[0]`。

一个函数，如果想要“精确地”表达“我接受一个指针，这个指针指向了一个大小为 10 的数组”，必须把 10 体现出来：

```cpp
void func(int (*arr)[10]);
void func(int arr[][10]); // 写法二
```

写法二的好处是，在声明了 `int arr[3][10]` 后可以直接 `func(arr)`。

## 函数返回类型

首先，最重要的是知道函数是如何返回一个值的，见 P201。

返回值用于**初始化调用点的一个临时量**。

不要返回函数的局部变量的引用或指针。

如果只是单纯返回一个非指针或非返回值还好，可以通过拷贝，在局部变量被销毁后，可以通过返回值初始化调用点，相当于拷贝了临时值。

```cpp
// workable
string test() { return "hello"; }
int main() {
    string s = test();
    cout << s << endl;
    return 0;
}
```

如果返回是引用类型：

```cpp
const string& test()
{
    return "hello";
}

int main()
{
    const string t = test();
    cout << t << endl;
    return 0;
}
```

返回值是一个引用，我理解为 `string& ret = "hello";`，当然这个 `ret` 不在栈上，但是这个引用指向了局部变量，函数结束后，局部变量是啥也不知道。会报错。

函数可以返回一个引用类型，这样返回值能够作为左值被写入。比如编写一个获取两个字符串中较长的那个字符串的函数 `string& func(string& str1, string& str2);`，然后 `func(s1, s2) << "hello";`。

这个例子举得不太好，因为如果只获取较长的那个字符串，需要 const 作为参数和返回值，但是返回值如果是常量引用，也是不能被修改的。

### 返回数组的指针

P205

有数种办法可以返回一个数组的指针

1. 使用类型别名。如 `using arrT = int[10]; arrT* func();`
2. 像声明一个数组指针那样，声明一个函数。如 `int (*func(<args>))[10]`，括号与维度必不可少，括号是告诉编译器返回的是指针，没有括号，就要变成数组，维度是告诉编译器指针指向的数组的大小
3. 使用“尾置返回类型”，用“auto”和“->”。如 `auto func(<args>) -> int(*)[10];`
4. 使用 `decltype`。如 `int odd[5]; decltype(odd) *func(<args>);`

## 函数重载

函数重载的是函数的参数，会根据不同的传入的参数类型决定要调用哪一个参数。

意思是可以让同名的函数有不同的返回值。

谈到类型，一定涉及 const

编译器无法区分顶层 const，即传入后无法修改这个这对象的函数。我的理解是，因为调用的时候，都要拷贝一份参数。

但是编译器可以区分底层 const，由此有一种写法，利用 const_cast，如果传入不是常量，就转换为常量。

```cpp
string& shorterString(string& s1, string& s2)
{
    auto& r = shorterString(const_cast<const string&>(sl), const_cast<const string&>(s2));
    return const__cast<string&>(r);
}
```

重载与作用域：当在一个作用域内声明一个函数时候，就会屏蔽掉作用域外面的声明，如

```cpp
int func(double d);
{
    int func(int i);
    cout << func(3.2);
}
```

调用的是第二个 `func`，而且是把 3.2 转换为 int 类型调用的。

## 特殊用途语言特性

### 默认实参

P211

定义一个函数的时候，可以给参数提供默认值。但是一个参数如果有默认值，他后面所有的参数也得提供默认值。就好像 Python 允许接受参数列表一样，列表只能放在最后面。

调用函数的时候，只能省略尾部的实参。也就是说，想要让前面的参数使用默认值，后面的自定义，那么做不到。

所以设计函数的时候，要让较频繁变动的参数靠前，不怎么变动的靠后。

默认实参声明，在声明函数的时候，提供默认参数

### inline 和 constexpr 函数

P213

内联函数要在头文件中定义。

我是这么理解的：因为得到可执行文件需要编译和链接两个阶段，链接一般是把不同 cpp 文件编译得到的 .o 文件链接，而且 .o 文件里记录的是函数调用的相关信息。如果一个函数被声明为了 inline，就不希望被单独编译成一个函数，所以要在编译的时候，把这个函数的内容告诉编译器。

关键还是在于，每次编译器只能编译一个 cpp 文件，导致如果 inline 函数单独放到对应的 cpp 文件中，编译器是不知道其他 cpp 文件的具体内容的。

### 调试帮助

P216

首先明确，`assert` 是一个宏，用 `NDEBUG` 这个宏变量来控制 `assert` 的具体内容。

如果不定义 NDEBUG 这个宏，`assert(expr)` 就会在运行的时候检查 expr，如果为假就直接终止程序。

除了 assert 以外，还有这些变量：

-   `__FUNC__`
-   `__LINE__`
-   `__TIME__`
-   `__DATE__`

## 函数匹配

最匹配的函数满足两个条件

1. 该函数的每个实参都不劣于其他可行函数
2. 至少一个实参的匹配优于其他函数

也就是说，每个参数要么做到单人第一，要么就并列第一。

如果找不到最匹配的函数，就报错。

如何确定优劣？参考 P219

## 函数指针

可以让指针指向一个函数

声明方式：用指针声明替换掉函数名称即可，如

```
int func(int a, int b);
int (*prt_func)(int a, int b);
```

给函数赋值，下面两种方式均可，取地址符 `&` 不是必须的：

```
ptr_func = &func;
ptr_func = func;
```

通过指针调用，两种方式均可，解引用也不是必须的

```cpp
int a = ptr_func(i, j);
int a = (*ptr_func)(i, j);
```

使用函数，就会涉及到重载，当指针要指向重载函数时候，类型必须精确匹配

注意，函数和指向函数的指针是两种不同的类型，当把“函数”作为函数的参数传递的时候，函数类型会被自动转换为函数指针。

为了简化书写，让人联想到前面的方式：

-   使用 typedef
-   使用 decltype
-   使用类型别名
-   尾置类型

```cpp
// typedef 写法，参考 P222
// Func 和 Func2 是函数类型
typedef bool Func(const string&, const string&);
typedef decltype (lengthCompare) Func2; //等价的类型


// FuncP和FuncP2是指向函数的指针
typedef bool(*FuncP)(const string&, const string&);
typedef decltype (lengthCompare) *FuncP2; //等价的类型
```

类型别名：

```cpp
using func = int (int, int);
using ptr_func = int (*) (int, int);
```

注意括号，这决定了**返回值**是指针还是**定义的对象**是指针

最后，看懂这段代码就行：

```cpp
int func(int a, int b);

int sum(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int main()
{
    int i;
    vector<decltype(func)*> v_ptr_func;
    v_ptr_func.push_back(&sub);
    v_ptr_func.push_back(&sum);
    cout << (*(v_ptr_func[0]))(100, 2) << endl;
    return 0;
}
```

# 类

定义在类中的函数是自动 inline 的。

## 定义抽象数据类型

P230

为类定义成员函数，是可以直接写到 `struct` 里面的。

```cpp
struct Sales_data {
    string bookNo;
    string isbn() const {return bookNo;}
}
```

这个函数有值得记录的地方。

首先，`bookNo` 是一个成员变量，当调用 `isbn()` 的时候，如何知道使用的是哪一个对象的呢？

这就涉及到“隐式参数”的概念。使用 `s.isbn()` 时候，编译器会把 `s` 的地址传给 `isbn()` 的参数，这个参数叫做 `this`，是一个**指针**。

第二点是 `isbn` 定义的时候，有一个 `const`。这个 const 的作用类似于前文提到的，把函数参数声明为 const 类型，可以使得函数接收更广泛的参数类型。

因为默认情况下，这个 this 不是底层 const，指向的对象是非常量。为了让 `const Sales_data` 也能调用，所以要告诉编译器，这个默认的 this 指向了常量。

```cpp
struct Data {
    string name = "hello";
    string getName() const { return name; } // workable

    // error: passing 'const Data' as 'this' argument discards qualifiers [-fpermissive]
    // string getName() { return name; }
};

int main()
{
    const Data d;
    cout << d.getName() << endl;
    return 0;
}
```

当然也可以把函数写到外面，但是这样需要在类的内部声明这个函数，有 `const` 也要写在声明里面：

```cpp
struct Data {
    string name = "hello";
    string getName() const;
};

string Data::getName() const
{
    return name;
}
```

### 构造函数

P236

构造函数没有返回值。

没有定义构造函数的类，由编译器控制默认初始化，执行初始化的函数叫做默认构造函数。

默认构造函数无需任何实参。

`=default` 是告诉编译器生成默认的构造函数，如 `Sales_data() = default;`

默认的构造函数执行默认初始化。

**构造函数初始值列表**是一段在参数和代码段之间，以冒号开头的代码

```cpp
Sales_data(const string &s, unsigned n, double p):
    bookNo(s), units_sold(n), revenue(p*n) {}
```

没有在这个列表里出现的参数将被默认初始化

### 拷贝、赋值和析构

不主动定义对象赋值的操作，编译器就会替我们合成它们。

```cpp
// total = trans
total.attrib = trans.attrib
```

在所有对象都在栈上时候，这有点像递归赋值，可以保证对象的每个属性被拷贝。

如果需要动态内存，可能不能依赖于编译器，但是如果使用 `vector` 和 `string`，就可以依赖于编译器。

## 访问控制与封装

使用 `private` 和 `public` 关键字控制权限，`struct` 和 `class` 的唯一区别就是默认的访问权限。

使用 `friend` 关键字，可以让不是类的成员函数访问类的私有成员变量，如：

```cpp
class Person {
    friend istream& read(istream& is, Person& p);
private:
    string name;
}
```

这样 `read` 函数内就能访问 `person.name` 了。

友元只是授予访问权限，而不是一个真正的“函数申明”。

## 类的其他特性

类型成员，在类的内部，也能通过 `typedef` 或者 `using` 来声明新的类型

可变数据成员：对于 `const` 对象，如果内部的成员变量被声明为 `mutable`，那么还是可以改变的，如 `mutable int i;`，即使某个对象被声明为了 `const`，仍旧可以通过 `const` 成员函数修改它的值。

P248

基于 const 的重载

看下面代码可能有一个问题：

```cpp
Screen myScreen;
myScreen.display(cout).set('*');
```

display 为了兼容常量的调用，所以返回的是常量，就不能调用 `set`。

除非我们让 display 可以返回普通变量的引用，很幸运，cpp 支持底层 const 的重载，也就是说，cpp 可以通过指针（变量）是不是底层 const 做函数重载。通过调用者是不是 const 来进行函数的重载，书山的原话说这两者的“原因差不多”。

```cpp
class Scree {
public:
    Screen &display(std::ostream &os) {
        do_display(os);
        return *this;
    }
    Screen &display(std::ostream &os) const {
        do_display(os);
        return *this;
    }
}
```

## 类的作用域

P253

在类的外部定义类属于类的成员函数时候，要使用 `<class_name>::<func_name>`，这就是在告诉编译器，作用域在类的内部了，所以可以在函数的内部使用类的成员。

不过函数的返回值是在函数名字的外面，“不知道”类的作用域，所以如果返回的类型是类的类型成员，比如 `using pos = std::string::size_type`，返回值如果要是 `pos`，还得告诉编译器这个类型需要到类的作用域里面去找：`<class_name>::<type_member> <class_name>::<func_name>`。

编译器处理完类中的全部声明后，才会取处理成员函数的定义。这样，哪怕成员变量定义在函数后面，也可以在函数中使用之。

相同类型名的定义，不能在类外部和内部同时出现。

一般来说，在子作用域定义重新定义外部的名字是合法的，但是对于“类+类型名”而言，这是一种特殊情况。

```cpp
typedef double Money;
class Account {
public:
    typedef double Money; // wrong
}
```

类的内部还有函数的作用域，当函数参数和类的成员变量同名时候，因为函数作用域是类的子作用域，会使用函数参数。所以不要让参数和成员变量同名。

当然，可以使用 `this->member` 来访问类内部的成员。

如果在类的内部使用类外部的变量，可以通过 `::var` 来访问。`::` 前面什么都不写，就是说明使用全局的变量。

总结一下，作用域有：全局作用域，类作用域和函数作用域。会从内而外查找名字。

使用 `<class_name>::` 就进入了类作用域，在类里面定义函数，参数的名字会优先在类作用域，也就是函数参数里面去找。类中找不到的函数名/成员就到全局作用域里查找。

## 构造函数再探

如果不在初始值列表里初始化成员变量，那么实际执行的流程就是，先用默认值初始化，再执行函数中的赋值操作。比如下面这段代码：

```cpp
Data::Data(const string &s) { bookNo = s; }
```

牢记初始化和赋值是两种不同的操作。

当成员变量是 `const` 或者引用的时候，就不能在函数体内部去给它赋值了。

构造函数初始值列表不规定初始化的顺序，所以不要想着用初始化好的成员变量 A 去初始化还未初始化的成员变量 B。

使用默认实参的构造函数，实际上也提供了默认构造函数。如：

```cpp
Person(const string& name="LiHua", const string& addr="Beijing")
    : name(name), address(addr) {};

// ...
const Person p;
// p.name = "LiHua", p.address = "Beijing"
```

### 委托构造函数

P261

在构造函数的**初始化值列表**里，调用其他的构造函数。注意千万不要写到 `{}` 里面了。

相当于把参数初始化交给了另一个函数去完成，一个语法糖。

### 隐式的类型转换

如果构造函数只接受一个实参，实际上定义了转换为此类类型的隐式转换机制。

```cpp
string bookNo = "123";
item.combine(bookNo);
```

因为 `bookNo` 这个 string 对象被转换为了一个**临时**的 `Sales_data` 对象，所以 `combine` 这个函数才能 work。

注意：

1. 这种类型转换只能是“一步”的，不能说把一个 int 类型转换成 string 再转换成 `Sales_data` 类型。
2. 在定义 `combine` 函数的时候，接受的参数只能是 `const`，因为临时变量不能绑定到可修改的值上。这是 `gcc` 的 feature，它认为修改临时变量没有意义。可以参考 [Non const lvalue references](https://stackoverflow.com/questions/18565167/non-const-lvalue-references)

可以在类的内部通过 explicit 关键词抑制这种转换。

### 字面值常量类

P267

真的有人会去使用这种东西吗...

## 类的静态成员

静态成员变量不和某个对象绑定，对象中也不保存与静态成员相关的数据

静态成员函数没有 this 指针，所以不能声明为 `const`

使用静态成员变量，既可以通过 `<obj_name>.member`，也可以 `<class_name>::member`

类静态成员在类的内部这叫做“声明”

```cpp
class Account {
public:
    void calculate();
    static int days;

private:
    double amount;
    static double rate;
    //  static double rate = 10; // error
    static double initRate();
};
```

注意，直接写 `static int days = 30` 是会报错的。在类内初始化静态成员变量也是有讲究的。

如果要在类的外部定义且初始化静态成员变量，那么首先，不要把这个初始化的过程写到 `main` 函数里面，书中的原话是这种类的静态变量“类似全局变量”。

```cpp
class Account {
public:
    static int days;

private:
    double amount;
    static double rate;
};

// 必须要 define
int Account::days = 10; // define and init here
// int Account::days; // define and init with default value

int main()
{
    cout << Account::days << endl;  // 10
    return 0;
}
```

必须要在类的外部定义这个静态变量，类的内部只是声明。

在类外部定义的时候，不要再写 `static` 关键词了。

`days = 10` 如果不写 `=` 号，说明用默认值初始化，就是 0。

### 类内初始化

P270

我的理解，就是用 `constexpr static <var> = <constexpr>;` 来在类的内部初始化一个静态成员变量。

这句话太经典了，在我看来，CPP 为何被那么多人诟病，绝对有它的一份功劳：

> 例如，如果 `period` 的唯一用途就是定义 `daily_tbl` 的维度，则不需要在 `Account` 外面专门定义 period。此时，如果我们忽略了这条定义，那么对程序非常微小的改动也可能造成编译错误，因为程序找不到该成员的定义语句。举个例子，当需要把 `Account::period `传递给一个接受 `const int&`的函数时，必须定义 `period`。

我的理解是，如果仅仅只是做常量传播，那么不写定义语句就不报错。

所以，书中的“Best Practice”说通常要写定义语句。

### 静态成员可以用于某些场景

P271

-   静态成员可以是不完全类型
-   静态成员可以是默认参数

# 第一部分小结

给我印象最深的是“类型”

从最普通的 int 和 double 等，到数组、vector 和 string

然后是复合类型：指针和引用

接着是 const，top level 和 low level

中间讲了一些语句和函数的内容。

最后进入类，也就是自定义的类型

在类里面，很荣幸又见到了 const 和构造函数。

ELF 文件与链接

Created: 2024-07-02T11:03+08:00
Published: 2024-07-02T16:44+08:00
Categories: OperatingSystem

[toc]

链接就是为了解决符号的问题，符号分为变量和函数。
关于什么是 section 什么是 segment 就不赘述了，可以参考文章[^1]，里面的细节太繁琐，
我觉得更重要的是知道每个 section 中需要记录的信息，编码的格式是次要的。

# 工具：`readelf` 和 `objdump`

`objdump -h <file>` 看 sections
`readelf -S <file>` 看 sections
`readelf -s <file>` 看符号表
`objdump -t <file>` 看符号表
`objdump -d <file>` 看汇编代码
`objdump -r <file>` 看重定位符号

> `readelf` 和 `objdump` 是两个不同的工具，它们在分析和调试 ELF 文件时提供了不同的功能和信息。
>
> `readelf` 是一个用于查看 ELF 文件结构和内容的工具。它提供了对 ELF 文件头、节头表、符号表、重定位表等结构的解析和显示。`readelf` 可以用于查看 ELF 文件的基本信息、节的属性、符号的绑定和类型、重定位信息等。它对于了解 ELF 文件的结构和元数据非常有用。
>
> `objdump` 则是一个更强大的工具，它提供了比 `readelf` 更多的功能。除了显示 ELF 文件的结构和内容外，`objdump` 还可以反汇编可执行文件或目标文件的机器代码，并提供源代码与汇编代码之间的对应关系。它还可以显示符号表、调试信息、堆栈跟踪等。`objdump` 还支持多种输出格式，如十六进制、反汇编、源代码等。
>
> 因此，尽管 `readelf` 可以提供对 ELF 文件的基本解析和显示，但 `objdump` 提供了更多的功能，包括反汇编、源代码对应、调试信息等。根据具体的需求，选择适合的工具可以更好地满足对 ELF 文件的分析和调试的需求。
> —— GPT-3.5

# 程序 = 指令 + 数据

假设有 `a.c`:

```cpp
// import from global
extern const int eci;
extern int ei;

// static
static int si;
static const int sci;

// export
const int ci;
int i = 1;

int print(int);

int main() {
    print(eci);
    print(i);
    return 0;
}
```

注：

1. C 语言中的 static 关键字是限制该符号仅在本文件内使用，是该文件内局部的（local）
2. C 语言中的 extern 关键字表示使用外部定义的变量，可以认为是 import from global
3. C 语言中在任何函数外定义变量如 `int a` 将会暴露给外部，可以认为是 export to global

一份文件编译好后应当包含如下信息：

1. 文件内定义的函数，如 `main`，放在 `.text` 中
2. 文件使用了外部定义的函数，如 `print`
3. 文件使用了外部定义的变量，用 extern 关键字导入，如 `eci`、`ei`
4. 文件内定义的变量，并且使用 static 关键字限定不导出，如 `si`，`sci`
5. 文件内定义的变量，并且导出给其他文件使用，也就是 global 变量，如 `ci` 和 `i`

毕竟 `程序 = 代码 + 数据`，CPU 就是指针在函数间跳来跳去修改数据。

## 符号表

```bash
gcc -c a.c -o a.o
objdump -t a.o
readelf -s a.o
```

objdump：

```bash
a.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 a.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l     O .bss   0000000000000004 si
0000000000000000 l     O .rodata        0000000000000004 sci
0000000000000004 g     O .rodata        0000000000000004 ci
0000000000000000 g     O .data  0000000000000004 i
0000000000000000 g     F .text  0000000000000029 main
0000000000000000         *UND*  0000000000000000 eci
0000000000000000         *UND*  0000000000000000 print
```

关于变量：
在文件内部定义的变量，每个变量都有 l 或者 g 标识，标识 Local 或者 Global。

1. const 一定在 `.rodata`
2. `.bss` 意思是 `block started by symbol`，用来存储 uninitialized 变量以节约空间，
3. `.data` 就是存放初始化好的变量
4. 被优化掉未使用的 extern 就不在符号表中，如 ei

UND 意思是，不在任何一个 section 节中

# 静态链接与重定位

## 不知道的地址先用 0 填充

在 assemble 得到的 二进制代码中，总会有一条 call print 的函数，但是这个 print 的地址还是未知的，所以先用 0 填充，
同时需要记录之后链接要将这个位置替换到真实的函数位置。

查看 a.c 汇编后的结果：

```bash
objdump -d a.o

a.o:     file format elf64-x86-64

Disassembly of section .text:

0000000000000000 <main>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   8b 05 00 00 00 00       mov    0x0(%rip),%eax        # e <main+0xe>
   e:   89 c7                   mov    %eax,%edi
  10:   e8 00 00 00 00          call   15 <main+0x15>
  15:   8b 05 00 00 00 00       mov    0x0(%rip),%eax        # 1b <main+0x1b>
  1b:   89 c7                   mov    %eax,%edi
  1d:   e8 00 00 00 00          call   22 <main+0x22>
  22:   b8 00 00 00 00          mov    $0x0,%eax
  27:   5d                      pop    %rbp
  28:   c3                      ret
```

在 `a.c` 中我们调用了两次函数，`print(i)` 和 `print(eci)`

1. 10 和 1d 的 call print 指令都没有填写地址
2. 调用的参数都是 0x0(%rip)

## 重定位表记录那些暂时用 0 填充的位置

这些地方肯定是要修改的，记录在`a.o` 的重定位表中：

```bash
objdump -r a.o

a.o:     file format elf64-x86-64

RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE              VALUE
000000000000000a R_X86_64_PC32     eci-0x0000000000000004
0000000000000011 R_X86_64_PLT32    print-0x0000000000000004
0000000000000017 R_X86_64_PC32     i-0x0000000000000004
000000000000001e R_X86_64_PLT32    print-0x0000000000000004



RELOCATION RECORDS FOR [.eh_frame]:
OFFSET           TYPE              VALUE
0000000000000020 R_X86_64_PC32     .text
```

## 链接后修改位置

b.c

```c
// b.c
#include <stdio.h>

int eci = 2;
int ei = 3;
void print(int val)
{
    printf("%d\n", val);
    return;
}
```

a.o 和 b.o 合并的时候，就把 text 合并，并且根据重定位表修改 a.o 中使用外部符号对应的 byte 位置

```bash
gcc -c b.c -o b.o
gcc a.o b.o -o ab.out
objdump -d ab.out # you can see the actual function and variable address
```

## `*.o` 没有 segments

sections 详细记录了每个节的数据，用于给 linker（链接器）看，我们使用 `a.o` 和 `b.o` 链接为 `ab.out`，所以这两个 `.o` 文件一定有 sections。
但是当使用命令 `readelf -l a.o` 时候，会提示「There are no program headers in this file.」。
因为 segments 是给 loader（加载器）看的，决定要把各个 sections 加载到内存的哪些地方，`a.o` 作为中间文件，加载器不会加载，所以没有 segments。

如果使用命令：`readelf -l ab.out`，解析可执行文件的 segments，会发现 segments 就是 sections 的集合，不同的 sections 可以被 map 到同一个 segment 中：

```bash
Elf file type is DYN (Position-Independent Executable file)
Entry point 0x1060
There are 13 program headers, starting at offset 64

Program Headers:
  Type           Offset             VirtAddr           PhysAddr
                 FileSiz            MemSiz              Flags  Align
  PHDR           0x0000000000000040 0x0000000000000040 0x0000000000000040
                 0x00000000000002d8 0x00000000000002d8  R      0x8
  INTERP         0x0000000000000318 0x0000000000000318 0x0000000000000318
                 0x000000000000001c 0x000000000000001c  R      0x1
      [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
  LOAD           0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000628 0x0000000000000628  R      0x1000
  LOAD           0x0000000000001000 0x0000000000001000 0x0000000000001000
                 0x00000000000001ad 0x00000000000001ad  R E    0x1000
  LOAD           0x0000000000002000 0x0000000000002000 0x0000000000002000
                 0x000000000000011c 0x000000000000011c  R      0x1000
  LOAD           0x0000000000002db8 0x0000000000003db8 0x0000000000003db8
                 0x0000000000000264 0x0000000000000270  RW     0x1000
  DYNAMIC        0x0000000000002dc8 0x0000000000003dc8 0x0000000000003dc8
                 0x00000000000001f0 0x00000000000001f0  RW     0x8
  NOTE           0x0000000000000338 0x0000000000000338 0x0000000000000338
                 0x0000000000000030 0x0000000000000030  R      0x8
  NOTE           0x0000000000000368 0x0000000000000368 0x0000000000000368
                 0x0000000000000044 0x0000000000000044  R      0x4
  GNU_PROPERTY   0x0000000000000338 0x0000000000000338 0x0000000000000338
                 0x0000000000000030 0x0000000000000030  R      0x8
  GNU_EH_FRAME   0x0000000000002010 0x0000000000002010 0x0000000000002010
                 0x000000000000003c 0x000000000000003c  R      0x4
  GNU_STACK      0x0000000000000000 0x0000000000000000 0x0000000000000000
                 0x0000000000000000 0x0000000000000000  RW     0x10
  GNU_RELRO      0x0000000000002db8 0x0000000000003db8 0x0000000000003db8
                 0x0000000000000248 0x0000000000000248  R      0x1

 Section to Segment mapping:
  Segment Sections...
   00
   01     .interp
   02     .interp .note.gnu.property .note.gnu.build-id .note.ABI-tag .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
   03     .init .plt .plt.got .plt.sec .text .fini
   04     .rodata .eh_frame_hdr .eh_frame
   05     .init_array .fini_array .dynamic .got .data .bss
   06     .dynamic
   07     .note.gnu.property
   08     .note.gnu.build-id .note.ABI-tag
   09     .note.gnu.property
   10     .eh_frame_hdr
   11
   12     .init_array .fini_array .dynamic .got
```

# 动态链接

构造一个动态依赖库依赖于另一个动态依赖库的例子：a 无法直接访问 c 中的数据，只能通过 b 去访问。

```bash
gcc -fPIC -shared -o b.so b.c # compile b.c to shared object b.so
gcc -fPIC -shared -o c.so c.c
gcc -o abc a.c b.so c.so # compile and link to abc
```

![](./dynamic-abc.svg)

```cpp
// a.c
extern void print_in_b(int);
extern int get_var_in_c();

int main() {
    int a = get_var_in_c();
    print_in_b(a);
    return 0;
}
```

```cpp
// b.c
extern int var_in_c;
extern void print_in_c(int);

void print_in_b(int val) {
    print_in_c(val);
}

int get_var_in_c() {
    return var_in_c;
}
```

```cpp
// c.c
#include<stdio.h>
int var_in_c = 1;

void print_in_c (int val) {
    printf("%d\n", val);
}
```

根据依赖的顺序，需要把 c.so 中的函数加载到内存中，才可以调用 b.so 中的函数。
因为 c.so 可能在多个进程中，每个进程中 `print_in_c` 的地址不一定是一样的，
所以在 `b.so` 中 call print_in_c 的地址不能是写死的，要先把 c.so 加载到内存中，才能确定。

## 动态依赖库依赖外部变量

在 b.c 中访问了 c.c 中的变量：

```CPP
extern int var_in_c;

int get_var_in_c() {
    return var_in_c;
}
```

对应的汇编如下：

```bash
objdump -d b.so

0000000000001135 <get_var_in_c>:
    1135:       f3 0f 1e fa             endbr64
    1139:       55                      push   %rbp
    113a:       48 89 e5                mov    %rsp,%rbp
    113d:       48 8b 05 ac 2e 00 00    mov    0x2eac(%rip),%rax        # 3ff0 <var_in_c>
    1144:       8b 00                   mov    (%rax),%eax
    1146:       5d                      pop    %rbp
    1147:       c3                      ret
```

GPT 说：

这段代码的逻辑很简单，它从 `var_in_c` 变量中获取值并返回。
首先，它将当前函数的基址 `%rbp` 压入栈中，然后将栈指针 `%rsp` 的值复制到 `%rbp`，建立当前函数的栈帧。
接下来，使用 `mov` 指令将 `var_in_c` 的地址加载到 `%rax` 寄存器中。
然后，使用 `mov` 指令将 `%rax` 寄存器中的值（即 `var_in_c` 的值）复制到 `%eax` 寄存器中。
最后，通过 `pop` 指令恢复上一个函数的栈帧基址，并使用 `ret` 指令返回到调用者。

也就是说，b.so 中有几个字节记录了 var_in_c 的地址，每次访问 var_in_c 时候，先找到这个地址，再取出来。
`mov a,b` 和 `mov (a),b` 是两种不同的指令，后者是需要取地址的。
var_in_c 的地址就记录在 `0x2eac(%rip)` 中（当 %rip 指向 113d 的时候）。

`.got` 负责记录使用了外部变量的地址：

```CPP
objdump -R b.so

b.so:     file format elf64-x86-64

DYNAMIC RELOCATION RECORDS
OFFSET           TYPE              VALUE
0000000000003e48 R_X86_64_RELATIVE  *ABS*+0x0000000000001110
0000000000003e50 R_X86_64_RELATIVE  *ABS*+0x00000000000010d0
0000000000004020 R_X86_64_RELATIVE  *ABS*+0x0000000000004020
0000000000003fd8 R_X86_64_GLOB_DAT  __cxa_finalize
0000000000003fe0 R_X86_64_GLOB_DAT  _ITM_registerTMCloneTable
0000000000003fe8 R_X86_64_GLOB_DAT  _ITM_deregisterTMCloneTable
0000000000003ff0 R_X86_64_GLOB_DAT  var_in_c
0000000000003ff8 R_X86_64_GLOB_DAT  __gmon_start__
0000000000004018 R_X86_64_JUMP_SLOT  print_in_c
```

b.so 中，记录了 var_in_c 的 offset 为 0x3ff0 = 0x11dd + 0x2eac, 11dd 是 rip 下一条指令的地址，正好对上。

```bash
113d:       48 8b 05 ac 2e 00 00    mov    0x2eac(%rip),%rax        # 3ff0 <var_in_c>
```

## 动态链接库依赖外部函数

print_in_b 中对 print_in_c 的调用，也是通过指针记录 print_in_c 的地址，先 call print_in_c@plt，再 call print_in_c.

```bash

Disassembly of section .plt.sec:

0000000000001050 <print_in_c@plt>:
    1050:       f3 0f 1e fa             endbr64
    1054:       f2 ff 25 bd 2f 00 00    bnd jmp *0x2fbd(%rip)        # 4018 <print_in_c>
    105b:       0f 1f 44 00 00          nopl   0x0(%rax,%rax,1)

0000000000001119 <print_in_b>:
    1119:       f3 0f 1e fa             endbr64
    111d:       55                      push   %rbp
    111e:       48 89 e5                mov    %rsp,%rbp
    1121:       48 83 ec 10             sub    $0x10,%rsp
    1125:       89 7d fc                mov    %edi,-0x4(%rbp)
    1128:       8b 45 fc                mov    -0x4(%rbp),%eax
    112b:       89 c7                   mov    %eax,%edi
    112d:       e8 1e ff ff ff          call   1050 <print_in_c@plt>
    1132:       90                      nop
    1133:       c9                      leave
    1134:       c3                      ret
```

## 动态链接库内部使用自己的地址

如果在 b.c 中加入：

```c
int var_in_b = 1;
int* ptr_in_b = &var_in_b;
```

ptr_in_b 的值也是要在 b.so 转载到内存后才可以确定，这个记录在 rela.dyn 中[^3]

## 延迟绑定

调用 print_in_c@plt 时候，使用的是 offset 为 0x4018 内的值，
但是因为使用了延迟绑定技术，这里现在还没放 print_in_c 的地址，而是指向 .plt 中一段解析出 print_in_c 的指令。

b.so 中，0x4018 存放的值是 0x1030:

```bash
objdump -s -j .got.plt b.so

b.so:     file format elf64-x86-64

Contents of section .got.plt:
 4000 583e0000 00000000 00000000 00000000  X>..............
 4010 00000000 00000000 30100000 00000000  ........0.......
```

第一次调用 print_in_c 的时候，会先跳转到 .plt 中执行解析出 print_in_c 的指令，也就是 0x1030：

```CPP
Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:       ff 35 e2 2f 00 00       push   0x2fe2(%rip)        # 4008 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:       f2 ff 25 e3 2f 00 00    bnd jmp *0x2fe3(%rip)        # 4010 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:       0f 1f 00                nopl   (%rax)
    1030:       f3 0f 1e fa             endbr64
    1034:       68 00 00 00 00          push   $0x0
    1039:       f2 e9 e1 ff ff ff       bnd jmp 1020 <_init+0x20>
    103f:       90                      nop
```

很遗憾，只能分析到这里了，因为 0x4010 处应该就是存放解析 print_in_c 地址的逻辑地址，也就是其他博客说的 dl_runtime_resolve[^3]
但是 b.so 中 0x4010 还是用 0 填充，如果读者知道这个值是在什么时候被修改的，教教我好嘛。

## 总结

1. `.got`：动态链接库使用了外部变量，要记录其地址
2. `.got.plt`: 动态链接库使用了外部函数，要记录其地址
3. `.plt`: .got.plt 中第一次先跳转到 .plt 中，调用 dl_runtime_resolve 解析地址
4. `.rela.plt`: 解析地址需要使用 dl_runtime_resolve 的参数
5. `.rela.dyn`: 动态链接库自己用了自己变量的地址

[^1]: [Linux 系统中编译、链接的基石\-ELF 文件：扒开它的层层外衣，从字节码的粒度来探索 \- 知乎](https://zhuanlan.zhihu.com/p/375510683)
[^2]: [一文看懂静态链接 \- 知乎](https://zhuanlan.zhihu.com/p/317478523)
[^3]: [一文看懂动态链接 \- 知乎](https://zhuanlan.zhihu.com/p/319784776)

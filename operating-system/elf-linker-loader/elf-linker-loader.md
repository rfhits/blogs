# ELF 文件的链接与加载

-   created: 2024-07-02T11:03+08:00
-   published: 2024-07-02T16:44+08:00
-   modified: 2024-11-19T23:14+08:00
-   categories: operating-system

前置阅读：

1. [calling convention](../../compiler/calling-convention/calling-convention.md)
2. x86 汇编教学

[toc]

## compiler linker loader

```c
// hello.c
#include <stdio.h>
void hello() {
    printf("hello world\n");
}


// main.c
void hello(void);
int main() {
    hello();
}
```

![compiler-linker-loader](./compiler-linker-loader.svg)

gcc 指令介绍：

-   `-S`: s 表示 assemble only，得到汇编代码
    注意得到的指令格式为 AT&T syntax[^1]，比如 `mov src, dst`，Intel syntax 为 `mov dst src`
-   `-c`: 编译（compile）并汇编（assemble），得到 `.o` 文件，`-c` 表示 compile to an object file
-   `-s`: strip symbol table，不要符号表

以 `main.c` 和 `hello.c` 为例

1. `gcc -c main.c` 得到 `main.o`，从汇编语言课程我们就知道 `main.o` 里面有 `.text`、`.data` 等 sections。
2. `gcc -c hello.c` 得到 `hello.o`
3. `gcc main.o hello.o -o a.out`，这一步是链接（link），  
   把 `main.o` 和 `hello.o` 里需要合并的 sections 拼接在一起，比如 `.text` 就要拼在一起
4. `./a.out` 输出 `hello world`。
   这一步发生了 `execve` 系统调用，loader 把 `a.out` 中需要加载到内存中的 segment 加载到内存里，  
   然后按照调用约定设置好 `%rsp` 等寄存器后，直接跳转到 `.text` 的第一条指令位置开始执行。

-   section 是针对 linker 的概念，把不同的 .o 文件拼起来需要把 `.text` 合并
-   segment 是针对 loader 的概念，一个 segment 包含 0 个或者多个 section

### 系统调用 `execve`

`int execve(const char *path, char *const argv[], char *const envp[]);`

这个系统调用加载一个可执行文件并执行，v 表示 argv，e 表示 envp[^2]。

从抽象的角度说，可执行文件记录了一个状态机的初始状态。

例子：shebang 文件参数传递：

`print-args.c` 文件内容。

```c
// print-args.c

#include <stdio.h>
int main(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i)
    {
        printf("arg[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
```

`./demo` 文件内容：

```bash
##!././print-args.out hello world

file content
```

在 shell 中执行 `filename` 时候，如果第一行是 shebang `#!<exe-path> arg`，
会调用 `execve(exe-path, arg, filename)`。

### 用户态 static-loader

来自蒋炎岩  
视频：https://www.bilibili.com/video/BV1wL4y1L72C?t=570
课件：[可执行文件的加载](https://jyywiki.cn/OS/2022/slides/17.slides.html#/1/1)

可以看到就是依据 program header，使用 mmap 系统调用加载所有需要加载的 segment 到内存中
设置好 stack pointer ，一个 jump 过去就好了。

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>

#define STK_SZ           (1 << 20)
#define ROUND(x, align)  (void *)(((uintptr_t)x) & ~(align - 1))
#define MOD(x, align)    (((uintptr_t)x) & (align - 1))
#define push(sp, T, ...) ({ *((T*)sp) = (T)__VA_ARGS__; sp = (void *)((uintptr_t)(sp) + sizeof(T)); })

void execve_(const char *file, char *argv[], char *envp[]) {
  // WARNING: This execve_ does not free process resources.
  int fd = open(file, O_RDONLY);
  assert(fd > 0);
  Elf64_Ehdr *h = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE, fd, 0);
  assert(h != (void *)-1);
  assert(h->e_type == ET_EXEC && h->e_machine == EM_X86_64);

  Elf64_Phdr *pht = (Elf64_Phdr *)((char *)h + h->e_phoff);
  for (int i = 0; i < h->e_phnum; i++) {
    Elf64_Phdr *p = &pht[i];
    if (p->p_type == PT_LOAD) {
      int prot = 0;
      if (p->p_flags & PF_R) prot |= PROT_READ;
      if (p->p_flags & PF_W) prot |= PROT_WRITE;
      if (p->p_flags & PF_X) prot |= PROT_EXEC;
      void *ret = mmap(
        ROUND(p->p_vaddr, p->p_align),              // addr, rounded to ALIGN
        p->p_memsz + MOD(p->p_vaddr, p->p_align),   // length
        prot,                                       // protection
        MAP_PRIVATE | MAP_FIXED,                    // flags, private & strict
        fd,                                         // file descriptor
        (uintptr_t)ROUND(p->p_offset, p->p_align)); // offset
      assert(ret != (void *)-1);
      memset((void *)(p->p_vaddr + p->p_filesz), 0, p->p_memsz - p->p_filesz);
    }
  }
  close(fd);

  static char stack[STK_SZ], rnd[16];
  void *sp = ROUND(stack + sizeof(stack) - 4096, 16);
  void *sp_exec = sp;
  int argc = 0;

  // argc
  while (argv[argc]) argc++;
  push(sp, intptr_t, argc);
  // argv[], NULL-terminate
  for (int i = 0; i <= argc; i++)
    push(sp, intptr_t, argv[i]);
  // envp[], NULL-terminate
  for (; *envp; envp++) {
    if (!strchr(*envp, '_')) // remove some verbose ones
      push(sp, intptr_t, *envp);
  }
  // auxv[], AT_NULL-terminate
  push(sp, intptr_t, 0);
  push(sp, Elf64_auxv_t, { .a_type = AT_RANDOM, .a_un.a_val = (uintptr_t)rnd } );
  push(sp, Elf64_auxv_t, { .a_type = AT_NULL } );

  asm volatile(
    "mov $0, %%rdx;" // required by ABI
    "mov %0, %%rsp;"
    "jmp *%1" : : "a"(sp_exec), "b"(h->e_entry));
}

int main(int argc, char *argv[], char *envp[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s file [args...]\n", argv[0]);
    exit(1);
  }
  execve_(argv[1], argv + 1, envp);
}
```

## 「符号」

下面不考虑多文件编译、静态链接、动态链接什么的，只编译一个文件。

```c
// single.c
int add(int, int);
int x;

int main()
{
    x = add(1, 2);
    return 0;
}

int add(int x, int y)
{
    return x + y;
}
```

-   `gcc -c single.c` 得到 `single.o`
-   `gcc single.c -o single.out` 得到 `single.out`
-   `objdump -d single.o` 看反编译，-d 参数意思是 de-assemble
-   `objdump -d single.out` 看反编译

在 `main.o` 中，变量 x 和函数 add 都是符号，`main()` 并不知道他们的地址在哪里，生成的二进制文件反汇编如下：

![](./single.svg)

特别注意 `x = add(1, 2);` 这条语句，我们用 objdump 看反汇编结果，注意得到的是 AT&T syntax[^1]。

single.o 中的汇编如下：

```
   8:   be 02 00 00 00          mov    $0x2,%esi
   d:   bf 01 00 00 00          mov    $0x1,%edi
  12:   e8 00 00 00 00          call   17 <main+0x17>
  17:   89 05 00 00 00 00       mov    %eax,0x0(%rip)        # 1d <main+0x1d>
  1d:   b8 00 00 00 00          mov    $0x0,%eax
```

### `e8 0x00_00_00_00: call 17`

call 前的 mov 指令在设置 add 的参数：

```
   8:   be 02 00 00 00          mov    $0x2,%esi
   d:   bf 01 00 00 00          mov    $0x1,%edi
```

紧接着是 `call   17 <main+0x17>`，为什么是 call 17 呢？

首先要知道 call 指令在 x86 上的实现：当 pc 指向 `call data` 指令的时候，`%rip` 寄存器存储着下一条地址，call 的执行结果是跳转到 `%rip + data`

而 call 17 是反汇编得到的结果，single.o 的本意是，这个地方 `call data`，data 不知道先填写 0。

反汇编以为执行 `call data` 的时候，`%rip` 存储 call 的下一条指令地址 0x17，就跳转到 `%rip + data = 17 + 0 = 17` 的位置，所以将这条指令解读为 `call 17`。

本意是 `call 这个地址我以后再填写，先填0`，只是被反汇编解释成为了 `jump %rip`，恰好填 0 时候对应 call 的下一条语句而已。

### `mov    %eax,0x0(%rip)`

同理，

```
17:   89 05 00 00 00 00       mov    %eax,0x0(%rip)        # 1d <main+0x1d>
```

本意是想将 %eax 寄存器中的值存储到全局变量 x 中，这个变量的地址是相对 %rip 而言的。  
不是说把 %eax 存储到 mov 的下一条指令的位置，那样就直接写到 .text 区域了。

### 可执行文件的反汇编

single.out 中反汇编如下：

```
    1131:       be 02 00 00 00          mov    $0x2,%esi
    1136:       bf 01 00 00 00          mov    $0x1,%edi
    113b:       e8 0d 00 00 00          call   114d <add>
    1140:       89 05 ce 2e 00 00       mov    %eax,0x2ece(%rip)        # 4014 <x>
```

可以看到 call 指令从 `e8 00 00 00 00` 变成了 `e8 0d 00 00 00`，注意大端小端。  
当 pc 指向 call 时候，%rip 中存储下一条指令地址 1140，  
所以就是 call 的真实地址就是`%rip + d8 = 0x1140 + 0x0d = 0x114d`，也就是 `single.out` 中 add 的地址。

mov 写入 x，不再赘述

### 总结

所有的符号，哪怕是在一个文件中，`.o` 也没有立刻填写上它的地址，而是在成为可执行文件后，再填充地址，  
如果在 .o 中填充位置，还是不能确定最后链接的地址，谁知道这个 .o 未来会和其他什么文件链接呢？  
在可执行文件中填写最后的地址，execve load 这些 segment，一步到位。

## 工具：`readelf` 和 `objdump`

-   `objdump -h <file>` 看 sections
-   `readelf -S <file>` 看 sections
-   `objdump -t <file>` 看符号表
-   `objdump -d <file>` 看汇编代码
-   `objdump -r <file>` 看重定位符号
-   `objdump -R <file>` 看动态重定位符号

> `readelf` 和 `objdump` 是两个不同的工具，它们在分析和调试 ELF 文件时提供了不同的功能和信息。
>
> `readelf` 是一个用于查看 ELF 文件结构和内容的工具。它提供了对 ELF 文件头、节头表、符号表、重定位表等结构的解析和显示。`readelf` 可以用于查看 ELF 文件的基本信息、节的属性、符号的绑定和类型、重定位信息等。它对于了解 ELF 文件的结构和元数据非常有用。
>
> `objdump` 则是一个更强大的工具，它提供了比 `readelf` 更多的功能。除了显示 ELF 文件的结构和内容外，`objdump` 还可以反汇编可执行文件或目标文件的机器代码，并提供源代码与汇编代码之间的对应关系。它还可以显示符号表、调试信息、堆栈跟踪等。`objdump` 还支持多种输出格式，如十六进制、反汇编、源代码等。
>
> 因此，尽管 `readelf` 可以提供对 ELF 文件的基本解析和显示，但 `objdump` 提供了更多的功能，包括反汇编、源代码对应、调试信息等。根据具体的需求，选择适合的工具可以更好地满足对 ELF 文件的分析和调试的需求。
> —— GPT-3.5

## 静态链接

将 single.c 拆分为 main.c 和 add.c 举例：

```c
// add.c
int add(int x, int y) {
    return x + y;
}


// main.c
int add(int, int);
int x = 0;

int main() {
    x = add(1, 2);
    return 0;
}
```

`main.o` 反汇编结果如下：

```
objdump -d main.o

main.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   be 02 00 00 00          mov    $0x2,%esi
   d:   bf 01 00 00 00          mov    $0x1,%edi
  12:   e8 00 00 00 00          call   17 <main+0x17>
  17:   89 05 00 00 00 00       mov    %eax,0x0(%rip)        # 1d <main+0x1d>
  1d:   b8 00 00 00 00          mov    $0x0,%eax
  22:   5d                      pop    %rbp
  23:   c3                      ret
```

main.o 这个 binary 里面，  
0x13 应该填 add 地址，0x19 应该填写 x 的地址，但是目前填写的是 0。

所以 main.o 还需要记录 .text 中哪些位置需要被替换为什么符号，这个记录在 reloc 中。  
可以通过 `objdump -r main.o` 查看。

```
objdump -r main.o

main.o:     file format elf64-x86-64

RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE              VALUE
0000000000000013 R_X86_64_PLT32    add-0x0000000000000004
0000000000000019 R_X86_64_PC32     x-0x0000000000000004


RELOCATION RECORDS FOR [.eh_frame]:
OFFSET           TYPE              VALUE
0000000000000020 R_X86_64_PC32     .text
```

reloc 说：

1. 13 的位置要填 add
2. 19 的位置要填 x

那 add 和 x 究竟是什么呢？`objdump -t main.o` 查看符号表

```
objdump -t main.o

main.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 main.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 g     O .bss   0000000000000004 x
0000000000000000 g     F .text  0000000000000024 main
0000000000000000         *UND*  0000000000000000 add
```

-   x 的作用域为 g（global），在 bss 段中，是一个 O（object）
-   add 的作用域位置，既不是 g 也不是 l（local），是 UND

除此以外，还有 main，作用域是 g，是一个 F（function），在 .text 段中。

main.o 和 add.o 合并的时候，就把 text 合并，并且根据重定位表修改 main.o 中使用外部符号对应的 byte 位置

```bash
gcc -c main.c
gcc -c add.c
gcc add.o main.o
objdump -d a.out # you can see the actual function and variable address
```

### `*.o` 没有 segments

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

## 动态链接

### load, import, export

蒋炎岩说，如果要我们自己定义链接库的格式，只需要解决下面的需求：

1. LOAD("lib") # 加载动态库
2. IMPORT(putchar) # 加载外部符号
3. EXPORT(hello) # 为动态库导出符号

```pseudocode
DL_HEAD

LOAD("libc.dl") # 加载动态库
IMPORT(putchar) # 加载外部符号
EXPORT(hello)   # 为动态库导出符号

DL_CODE

hello:
  ...
  call DSYM(putchar) # 动态链接符号
  ...

DL_END
```

### GOT(Global Offset Table) & PLT(Procedure Linkage Table)

还是 main.c 和 add.c 的例子，这次我们加上 `printf` 并使用动态链接：

```c
// main.c
#include<stdio.h>

int add(int, int);
int x;
int main() {
    x = add(1, 2);
    printf("%d\n", x);
    return 0;
}

// add.c
int add(int x, int y) {
    return x + y;
}
```

-   `gcc -c main.c` 得到 `main.o`
-   `gcc -fPIC -shared add.c -o libadd.so` 得到 `libadd.so`
-   `gcc -o main main.o -L. -ladd -Wl,-rpath,.` 得到 `main`

main 中需要使用到两个外部函数，add 和 printf  
main.o 中自然有两条指令 `call <add>` 和 `call <printf>`  
我们已经验证过了 main.o 中这些地址是 0，如果是静态链接，linker 把 add 和 printf 的 .text 和 main 的 .text 合在一起后，会把 call 的后半截内容改掉。

对于动态链接的情况，printf 和 add 需要加载到内存中后，地址才可以确定，所以 `call <add>` 中 add 的地址无法在 main 中确定下来。

所以自然想到一种设计，当 loader 加载 main 的时候，分析符号表，发现需要使用外部符号 add，
加载 libadd.so 到内存中，加载完以后，得知 add 的地址，去修改 main 这个二进制文件中所有 `call <add>` 的地方。

![simple-dynamic-loader](./simple-dynamic-loader.svg)

这样的问题是，如果 main 里面很多地方都 call add(%rip)，那 loader 就对每一处 call 进行修改。

干脆就把 add 的地址作为一个变量存起来，所有 `call add(%rip)` 都跳转到一个 `jump real_add` 上去。  
这就是 GOT 的作用：把外部调用函数地址都存到 GOT 中，main 对其的调用都 wrap 一层，从 GOT 中取出地址再调用。  
新增 wrap 的这一层叫做 PLT，于是 main 中内容如下：

```
main:
call <add@plt>
call <printf@plt>

<add@plt>
jump got[add]

<printf@plt>
jump got[printf]

.got
add: address will be replaced after loader load libadd.so
printf: address will be replaced after loader load ...
```

### 延迟绑定

分析上面的 loader 实现，其实不需要在 loader 加载 main 的时候分析发现要用 add，然后立刻 load libadd.so
可以在 main call add 的时候，在跳转到一段加载并解析 libadd.so 的代码，填写 `got[add]`，延迟动态库的加载。

下面是使用 `objdump -D main` 和 `objdump -s --start-address=0x3fb8 main` 这两种指令分析的 `main` 内容，
第一列为地址，第二列为该地址存储的值。调用链比较长：

```
main
    call<add@plt>
        jump got[add]
            jump plt
                jump got[?]
```

```h
addr | value / section
1020 .plt
... push 3fb8 # got + 0x08
... jump 3fc0 # got + 0x10

1030
... ...
... jump 1020

1040
... ...
... jump 1020


1060 .plt.sec
1060 <add@plt>
... jump 3fc8 # got[add]
1070  <printf#plt>
... jump 3fd0 # got[printf]


1169 main
... ...
... call 1060 # add@plt
... call 1070 # printf@plt

3fb0 .got
3fb8 0000
3fc0 0000
3fc8 1030 # add real address
3fd0 1040 # printf read address
```

按道理来说，`.plt` 开始这段就应该是 dl_runtime_resolve 的地址，但是目前填写的是 0。  
或许是在其加载到内存后，才会修改

### 动态链接库内部使用自己的地址

如果 `libadd.so` 中使用了自己的内部的全局变量，该变量的地址也要 libadd.so 加载到内存后才可以确定。这个记录在 rela.dyn 中。

```c
// add.c -> libadd.so
int add_inner_var = 1;

int add(int x, int y)
{
    return x + y;
}

int *get_inner_var_addr()
{
    return &add_inner_var;
}
```

`objdump -R libadd.so` 读取 rela.dyn 得知 add_inner_var 需要修正，在 3fc8.

```
objdump -R libadd.so

libadd.so:     file format elf64-x86-64

DYNAMIC RELOCATION RECORDS
OFFSET           TYPE              VALUE
0000000000003e60 R_X86_64_RELATIVE  *ABS*+0x00000000000010f0
0000000000003e68 R_X86_64_RELATIVE  *ABS*+0x00000000000010b0
0000000000004000 R_X86_64_RELATIVE  *ABS*+0x0000000000004000
0000000000003fc0 R_X86_64_GLOB_DAT  __cxa_finalize
0000000000003fc8 R_X86_64_GLOB_DAT  add_inner_var               # <--- look here
0000000000003fd0 R_X86_64_GLOB_DAT  _ITM_registerTMCloneTable
0000000000003fd8 R_X86_64_GLOB_DAT  _ITM_deregisterTMCloneTable
0000000000003fe0 R_X86_64_GLOB_DAT  __gmon_start__
```

get_inner_var_addr 访问的也正是 add_inner_var 的地址 3fc8：

```
objdump -d libadd.so

0000000000001111 <get_inner_var_addr>:
    1111:       f3 0f 1e fa             endbr64
    1115:       55                      push   %rbp
    1116:       48 89 e5                mov    %rsp,%rbp
    1119:       48 8b 05 a8 2e 00 00    mov    0x2ea8(%rip),%rax        # 3fc8 <add_inner_var-0x40>
    1120:       5d                      pop    %rbp
    1121:       c3                      ret
```

## 总结

1. elf 文件有 sections 和 segments
   section 是给 linker 看的，把许多 linkable 中各种 section 如 .text 合并在一起  
   segment 是给 loader 看到，把 executable load 到内存里
2. linker 把 section 合并的一个绝佳例子是 `main.o` 和 `add.o` 的合并
   main.o 中对于外部函数的调用，还不知道函数地址，所以先填写 0，linker 合并 .text 以后，直接填上  
   所以 main.o 中需要记载二进制中何处需要被 linker 修改成为哪个符号
3. loader 动态加载的绝佳例子是 `main` 依赖 `libadd.so`，
   main 中需要 `call add`，但是 add 地址需要 libadd.so 加载后才可以得知  
   GOT 是 main 中用于存放 libadd.so 加载后 add 的真实地址  
   PLT 用来将所有的 `call add` 被 wrap 成 `call <add@plt>` 统一跳转到 `jump got[add]`  
   延迟绑定技术是将 `GOT[add]` 中初始值写为 dl_runtime_resolve 地址，调用 add 时候再解析。

[^1]: [assembly \- MOV src, dest \(or\) MOV dest, src? \- Stack Overflow](https://stackoverflow.com/questions/2397528/mov-src-dest-or-mov-dest-src)
[^2]: [posix \- What does the "ve" in "execve" mean? \- Stack Overflow](https://stackoverflow.com/questions/50421197/what-does-the-ve-in-execve-mean)

# gdb 使用

-   created: 2024-04-29T20:17+08:00
-   modified: 2025-02-28T19:51+08:00
-   published: 2024-04-30T16:02+08:00
-   categories: applications

要看到符号表，编译需要加上参数 -g

常用命令有：

-   step/s
-   next/n
-   finish/fin: 结束当前函数调用
-   continue/c

更多命令参考 [Linux 系统编程（一）：C/C\+\+程序编译与 gdb 调试 \| Chou Bin's Blog](https://choubin.site/2020/02/15/apue-hm01/)

## 调试进程

### gdb attach 到现有的进程

`gdb -p {pid}`

ps，查看进程的命令：

```shell
ps -elf | grep {process_name}
```

或者

```shell
ps aux | grep {process_name}
```

### 通过 gdb 启动进程

run 前为 `main(argc, argv)` 设置参数：

```shell
set args <arg1> <arg2> ...
```

## 断点

支持三种打断点的方法：

`b filename:lineno | filename:function | function`

只需要提供文件名就好了，不需要输入文件所在的路径，非常方便。
如果有同名文件，通过 directory 命令切换目录打断点[^1]。

```shell
directory path_to_file_folder
b filename:lineno
```

## 堆栈分析

如果有 core 文件，`gdb {exe} {core}` 或者 `gdb -c {core} {exe}`

`bt` 指令可以查看调用栈，会打印所有的栈桢（frame）

可以通过 `frame {no}` 切换栈桢，查看对应栈桢中的局部变量。

## layout

看源码：`layout asm/src/split`，切换回 plaintext 模式，`c-a x`。

## 多线程 debug[^2]：

首先要知道 [Light Weight Process（LWP）](../operating-system/process-thread-lwp.md) 的概念。

对于一个多线程程序，gdb 可能只会跟踪一个线程。
如果背后有多个线程在运行，比如 LWP01，LWP02。

LWP01 触发了断点，gdb 跟踪到了，然后通过 continue 指令继续，当 LWP02 触发断点，gdb 可能无法知晓，因为它在跟踪 LWP01！

可以尝试 `set scheduler-locking off`

```shell
info threads # 查看当前进程的线程
thread <ID>  # 切换调试的线程为指定 ID 的线程
break test.c:100 thread all   #在所有线程中相应的行上设置断点

set scheduler-locking off # off | on
  off   不锁定任何线程，所有线程都执行
  on    只有当前被调试程序会执行
```

[^1]: [GDB 不同文件断点调试 \- 知乎](https://zhuanlan.zhihu.com/p/98811602)
[^2]: [GDB 多线程之旅 \- 知乎](https://zhuanlan.zhihu.com/p/173652671)

gdb 使用
gdb usage

Created: 2024-04-29T20:17+08:00
Published: 2024-04-30T16:02+08:00
Categories: CPP

编译需要加上参数 -g

简单的如 step、continue 就不介绍了，参考 [Linux 系统编程（一）：C/C\+\+程序编译与 gdb 调试 \| Chou Bin's Blog](https://choubin.site/2020/02/15/apue-hm01/)

run 前为 `main(argc, argv)` 设置参数：

```gdb
set args <arg1> <arg2> ...
```

切换目录打断点[^1]：

```
directory <path_to_file_folder>
break <filename>:<lineno>
```

多线程 debug[^2]：

```
info threads 查看当前进程的线程
thread <ID>  切换调试的线程为指定 ID 的线程
break test.c:100 thread all   在所有线程中相应的行上设置断点

set scheduler-locking off|on
  off   默认值，不锁定任何线程，所有线程都执行
  on    只有当前被调试程序会执行
```

[^1]: [GDB 不同文件断点调试 \- 知乎](https://zhuanlan.zhihu.com/p/98811602)
[^2]: [GDB 多线程之旅 \- 知乎](https://zhuanlan.zhihu.com/p/173652671)

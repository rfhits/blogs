# 进程、线程和轻量级进程

-   created: 2024-12-26T21:04+08:00
-   categories: operating-system

进程是对资源的抽象。

很久以前，Linux 的线程实现有一个弊端[^1]：
当一个线程阻塞时，会阻塞同个进程内的所有其他线程。

> Threads are often mixed with the term Light Weight Processes or LWPs.
> The reason dates back to those times when Linux supported threads at user level only.
> This means that even a multi-threaded application was viewed by kernel as a single process only.
> This posed big challenges for the library that managed these user level threads because it had to take care of cases that a thread execution did not hinder if any other thread issued a blocking call.

那进程内部线程的轮换只能完全由用户态控制，
一旦进程的一个线程进入了 kernel 然后被阻塞，这个线程就没法回到 user space 通过调度执行其他线程。

我的理解这样原因是，假设内核对进程的描述叫做 `task_struct`，它只能看到每个 process 的 `task_struct`，  
多个线程也只是共享一个 `task_struct`，所以多个线程一次只能有一个代表该进程进入内核。

后来线程的实现改为了 lwp，light weight process，

> Later on the implementation changed and processes were attached to each thread so that kernel can take care of them.
> But, as discussed earlier, Linux kernel does not see them as threads, each thread is viewed as a process inside kernel. These processes are known as light weight processes.

一个进程内的很多线程，可以说是共享很多资源的 light weight process，一个被阻塞以后，不会相互影响了。

[^1]: [What are Linux Processes, Threads, Light Weight Processes, and Process State](https://www.thegeekstuff.com/2013/11/linux-process-and-threads/)

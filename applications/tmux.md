# tmux(Terminal Multiplexer)

-   created: 2024-12-25T14:55+08:00
-   modified: 2025-02-27T11:07+08:00
-   published: 2025-02-28T20:09+08:00
-   categories: applications

首先介绍 tmux 的好处，比如你 ssh 到了远程机器上，突然因为网络波动断开了，那原来的工作没保存就寄了。
如果 ssh 上去，再 tmux，就创建了一个 session（会话），如果断开了，可以重连该会话。

[A Quick and Easy Guide to tmux](https://hamvocke.com/blog/a-quick-and-easy-guide-to-tmux/)

常用命令：

-   分屏：`c-b %` 和 `c-b "`
-   关闭 panel：`c-b x`
-   关闭 window：关闭所有 panel 后，自动关闭该 window
-   panel 之间导航：`c-b <arrow>`
-   退出，本质上是 detach 而不是销毁 session：`c-b d`
-   一个 window 类似一个桌面，切换桌面：`c-b <number>`
-   查看 历史记录：`c-b [`，然后按方向键

概念：

1. session：会话，使用 tmux 命令会自动创建一个新的会话
2. window：执行 tmux 后，得到一个 window 和一个 panel
   window 的概念相当于 MacOS 中的 window，比如最大化一个 app，就有了一个 window，触控板上三指滑动切换 window
   window 的概念也相当于 WindowsOS 中的 `Win + Tab` 切换 window
3. panel: 可以使用 `c-b %` 切分一个 window

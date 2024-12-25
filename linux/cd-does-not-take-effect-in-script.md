# 脚本中切换目录命令不生效

-   created: 2024-05-14T10:59+08:00
-   published: 2024-05-15T11:19+08:00
-   categories: linux
-   tags: bugs

打算在 wsl 中运行 Windows 文件中的 sh 文件，script.sh 内容为：

```sh
#!/usr/bin/sh
cd ..
```

但是终端中执行 `bash ./script.sh`，什么也没有发生。

排查了 shebang、line break、wsl 能否使用 Windows 文件系统等问题，最后终于找到原因。

## 原因

因为在 shell 的 child process 中 `cd` 了，当前 shell process 没有 `cd`，可以参考 xv6 的 shell 的代码。

[bash \- Why can't I change directories using "cd" in a script? \- Stack Overflow](https://stackoverflow.com/questions/255414/why-cant-i-change-directories-using-cd-in-a-script/255416#255416)

这就是 `source` 命令的作用：在当前 shell 执行命令，不要 spawn 子进程。

## 解决

```
. ./script.sh
```

气笑了

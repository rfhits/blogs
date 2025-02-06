# conda 常用命令

-   created: 2024-08-12T10:37+08:00
-   published: 2025-02-06T17:14+08:00
-   category: python

[toc]

最全面的教程是 conda.io 上的 [user guide » tasks](https://conda.io/projects/conda/en/latest/user-guide/tasks/)

## 安装 Package

我使用的是 Miniconda，操作系统是 win10。

首先，保证打开的是 Anaconda Prompt，而不是自己随意的一个 cmd 窗口

其次，如果 Miniconda 安装到了系统目录，需要以管理员权限运行这个 Prompt

最后，如果打算安装 Package，有两种方法：

1. 如果自己配置了代理/梯子，就不要再设置 channel 了，但是要在 conda 配置文件中指定代理服务器
2. 无代理，使用清华源或者豆瓣源等，下载时最好关闭系统代理

### 配置代理

为当前用户生成 `.condarc` 配置文件：

```
conda config --set show_channel_urls yes
```

在文件中添加代理规则，代理服务器的 ip 和 端口注意改成自己的：

```
ssl_verify: true
show_channel_urls: true
proxy_servers:
  http: http://127.0.0.1:7890
  https: http://127.0.0.1:7890
```

接下来就可以愉快地下载了

### 指定第三方源

第三方源，用的人多了就慢了，容易卡，哪怕是清华源也一样。

所以个人更加推荐使用代理服务器的方式，指定第三方源这种方法就不写了。

## 常用指令

Anaconda 自己也准备了 cheatsheet，见 [conda.io: cheat sheet](https://conda.io/projects/conda/en/latest/user-guide/cheatsheet.html)，中文版见 [conda 速查表](https://blog.xieqk.com/posts/2170262917.html)。

| description                                      | command    |
| ------------------------------------------------ | ---------- |
| 查看当前环境下的包，常用于检查某个包是否已经安装 | conda list |

---- 持续更新 ----

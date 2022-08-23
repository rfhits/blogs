想在Windows Linux Subsystem里写c语言，结果没法gcc，装包也报错，特地记录下。

## 版本和源

Ubuntu有很多个版本，这些版本上的包要更新的时候，下载下来的二进制文件是不一样的。

所以Ubuntu在apt-get的时候是要知道自己去哪里下载文件的，这些地址被写在

`/etc/apt/sources.list`下。

我们装完系统后，所谓的“换源”，就是让apt换一个地址去下文件。

有篇很好的文章：[[Linux]Ubuntu 20.04换阿里源](https://blog.csdn.net/wangyijieonline/article/details/105360138)

## 配置c语言环境

运行`sudo apt-get build-essential`

报错如下：

```bash
Some packages could not be installed. This may mean that you have
requested an impossible situation or if you are using the unstable
distribution that some required packages have not yet been created
or been moved out of Incoming.
The following information may help to resolve the situation:

The following packages have unmet dependencies:
 build-essential : Depends: libc6-dev but it is not going to be installed or
                            libc-dev
                   Depends: g++ (>= 4:7.2) but it is not going to be installed
E: Unable to correct problems, you have held broken packages.
```

很大可能是，Ubuntu版本和apt要去包的版本不匹配。

比如20.04的Ubuntu，要去下18.xx版本的包，肯定是下载不下来的。

换了源就好了。

也是很好的文章：[WSL下搭建C开发环境，解决stdio.h缺失，build-essential安装失败，包依赖问题](https://blog.csdn.net/Caption_Coco/article/details/109258959)

总之，不要随便复制粘贴别人的配置。
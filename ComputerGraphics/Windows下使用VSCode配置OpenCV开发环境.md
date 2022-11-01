Windows 下使用 VS Code 配置 OpenCV 开发环境

通过使用 GitHub 上他人编译好的动态库，进行 OpenCV 环境的配置。

本博客对应 Bilibili 实操视频：https://www.bilibili.com/video/BV1BP4y1S7NX/

[toc]

配置环境的前置知识非常多，在此一一罗列

1. 环境变量的作用
2. MinGW 不同版本的差异
3. C/CPP 文件的编译与链接
4. 动态链接与静态链接
5. OpenCV 编译后的文件夹的结构
6. g++ 编译命令中 `-I, -L, -l` 三个参数的含义
7. VS Code 开发 CPP 项目，生成的三个 `.json` 文件的作用

# MinGW 安装

## 选择 POSIX

这里涉及到**环境变量**相关的知识。

来到 https://sourceforge.net/projects/mingw-w64/files/#mingw-w64-gcc-8-1-0

看到

```sh
MinGW-W64 GCC-8.1.0
- x86_64-posix-sjlj
- x86_64-posix-seh (请选择这个版本下载)
- x86_64-win32-sjlj
- x86_64-win32-seh
- i686-posix-sjlj
- i686-posix-dwarf
- i686-win32-sjlj
- i686-win32-dwarf
```

如果之前从未使用过 MinGW，那么请下载 posix 类别，并配置环境变量；

如果之前使用过 MinGW，那么你大概率下载的是 win32 类别，所以请重新安装并配置环境变量。

就算不编译 OpenCV 源码，要用它提供的动态链接库，也得老老实实使用 posix 那个。

## 保留两个 MinGW

我自己之前下载的是 win32 ，所以直接踩坑。

最后我通过修改文件夹名字的方法，把两个 MinGW 都留下来，等以后要换的时候，再改回来。

```
# 我自己使用的部分系统环境变量
GCC_WIN32_HOME: C:\Library\mingw64-posix
GCC_POSIX_HOME: C:\Library\mingw64-win32
Path: 
  ***
  %GCC_POSIX_HOME%\bin
  ***
```

关于 win32 和 posix 的区别，请参考 [【c/cpp 开发工具】MingGW 各版本区别及安装说明](https://blog.csdn.net/qq_29856169/article/details/119380663)。

## 检验是否成功

`g++ --version`

```bash
C:\Users\User>g++ --version
g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

看第二行，有 posix 字样。

# C/CPP 文件的编译与链接

- 如果你是只会写单个 `.c/.cpp` 文件的小白
- 如果你没有上过**计算机组成**、**编译原理**和**操作系统**这三门课
- 如果你没有听说过 `readelf`，`objdump`，`.text/.data/.bss`

我建议你先打好基础，再来配置 OpenCV，否则欲速则不达，就算手把手教会如何配置，

**知其然而不知其所以然**，下回配置环境还是得折腾一遍。

我先抛出一个问题：

> 一句简单的 `printf("hello world");`，在我的电脑上编译出了 `.exe` 文件，能直接在另一台电脑上运行？我为什么不用手动实现 `printf` 这个函数，别人也不用实现，这个函数到底定义在了哪里，实现在了哪里？

放两个视频：

- [编译链接原理](https://www.bilibili.com/video/BV1FL411g7m5)
- [深入理解计算机系统19：链接总结：编译、链接、加载，ELF格式，符号表，符号解析，重定位，动态链接】](https://www.bilibili.com/video/BV1WU4y1b78m)

# OpenCV 文件夹结构

## 版本选择

使用别人编译好的 OpenCV dll 文件，保证编译后的文件能够运行。

这份文件在 [GitHub 仓库](https://github.com/huihut/OpenCV-MinGW-Build) 上可以下载，GitHub 下载的加速有很多办法，这里用最简单的一种：

[GitHub仓库快速导入Gitee及同步更新](https://gitee.com/help/articles/4284)。

来到 Gitee 上别人同步好的 [镜像仓库](https://gitee.com/mirrors_huihut/OpenCV-MinGW-Build/tree/OpenCV-4.5.2-x64)，直接下载 zip 文件就行，20 MB 左右。

我使用的版本是 `OpenCV 4.5.2-x64`，`OpenCV 4.5.5-x64` 我试了不行。

`4.5.5` 的问题是能够生成 `.exe`  文件，但是无法运行。

所以为了保证成功，用 `4.5.2`。

## 文件结构

```bash
C:\LIBRARY\CPP\PACKAGES\OPENCV-MINGW-BUILD-OPENCV-4.1.0-X64
├─etc # 不用管
├─include # 头文件
│  └─opencv2
│      ├─calib3d
│      ├─core
│      ├─dnn
│      ├─features2d
│      ├─flann
│      ├─... # 不一一罗列
└─x64
    └─mingw
        ├─bin
        │  └─ *.dll # 一堆 dll 文件
        └─lib
```

首先，`include` 文件夹，字面意思，用来 include 的。

我们知道 CPP 是定义和实现分离的，以函数为例，通常在 `.h` 文件中声明，在 `.cpp` 文件中实现。

如果 `main.cpp` 里使用了其他文件中定义的函数，而我们只是想要生成 `.o` 文件，只需要 `include` 对应的 `.h` 文件即可。

`include/opencv2` 下就是各种 `.h` 文件。

然后是 `x64/mingw/bin`，这里有一堆 `*.dll`，这就是动态链接库文件。

# 将动态链接库添加到系统环境变量

为了让第三方的动态链接库生效，我们需要将动态链接库添加到系统环境变量。

对于我来说，是将 `C:\Library\CPP\Packages\OpenCV-MinGW-Build-OpenCV-4.1.0-x64\x64\mingw\bin` 添加到环境变量中。

这一步的作用类似于告诉系统 `prinf()` 这个函数的二进制文件在哪儿。

# g++ 命令的参数

介绍三个参数 `-I`、`-L`  和 `-l`。

`-I`  告诉编译器，头文件里的 `include<package>` 去哪儿找。

`-L` 告诉编译器，添加一个要动态链接的目录

`-l` 指定具体的动态链接库的名称

具体可以参考 [gcc -L -l -I -i参数](https://blog.csdn.net/woaiclh13/article/details/88719324)。

# VS Code 项目配置

我们使用 VS Code 生成的三个 `.json` 文件来配置 OpenCV 项目，**而不是使用** cmake。

这三个文件分别是：

`c_cpp_properties.json`，`launch.json` 和 `tasks.json`。

## c_cpp_properties.json

这个文件删了，不影响**编译**与**链接**，但是 VS Code 的 C/C++ 插件依赖于这个文件做智能提示和代码分析。

```json
{
    "configurations": [
        {
            "name": "Win32", // 指示平台，如 Mac/Linux/Windows，实测乱填也行
            "includePath": [
                "${workspaceFolder}/**",
                "C:/Library/CPP/Packages/OpenCV-MinGW-Build-OpenCV-4.5.2-x64/include"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "compilerPath": "C:/Library/CPP/mingw64-posix/bin/g++.exe"
        }
    ],
    "version": 4
}
```

`includePath`：告诉插件，要用的依赖在哪儿。

`compilerPath`: 告诉插件，编译器的路径在哪儿。

## tasks.json

```json
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "build",
            "command": "g++",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "-I",
                "C:/Library/CPP/Packages/OpenCV-MinGW-Build-OpenCV-4.5.2-x64/include",
                "-L",
                "C:/Library/CPP/Packages/OpenCV-MinGW-Build-OpenCV-4.5.2-x64/x64/mingw/bin",
                "-l",
                "libopencv_calib3d452",
                "-l",
                "libopencv_core452",
                "-l",
                "libopencv_dnn452",
                "-l",
                "libopencv_features2d452",
                "-l",
                "libopencv_flann452",
                "-l",
                "libopencv_gapi452",
                "-l",
                "libopencv_highgui452",
                "-l",
                "libopencv_imgcodecs452",
                "-l",
                "libopencv_imgproc452",
                "-l",
                "libopencv_ml452",
                "-l",
                "libopencv_objdetect452",
                "-l",
                "libopencv_photo452",
                "-l",
                "libopencv_stitching452",
                "-l",
                "libopencv_video452",
                "-l",
                "libopencv_videoio452",
                "-l",
                "opencv_videoio_ffmpeg452_64",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "调试器生成的任务。"
        }
    ],
    "version": "2.0.0"
}
```

这个文件想要看懂，就需要 `g++` 命令那些参数的相关知识了，而那些参数相关的知识，就是编译和链接。

`isDefault`：表示这是默认的构建任务，可以发现，`-l` 后面跟着的参数，就是我们下载的 OpenCV的 `bin` 目录下的**动态链接库**的文件名。

## launch.json

注意 `gdb` 文件路径即可。

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch", // 配置名称，将会在启动配置的下拉菜单中显示   
            "type": "cppdbg", // 配置类型，这里只能为cppdbg
            "preLaunchTask": "build",
            "request": "launch", //请求配置类型，可以为launch（启动）或attach（附加）      
            "program": "${fileDirname}\\${fileBasenameNoExtension}.exe",
            // 将要进行调试的程序的路径
            "args": [], // 程序调试时传递给程序的命令行参数，一般设为空即可        
            "stopAtEntry": false, // 设为true时程序将暂停在程序入口处，一般设置为false       
            "cwd": "${fileDirname}", // 调试程序时的工作目录，一般为${workspaceRoot}即代码所在目录workspaceRoot已被弃用，现改为workspaceFolder        
            "environment": [],
            "externalConsole": false, // 调试时是否显示控制台窗口  
            "MIMode": "gdb",
            "miDebuggerPath": "C:/Library/CPP/mingw64-posix/bin/gdb.exe", // miDebugger的路径，注意这里要与MinGw的路径对应  
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": false
                }
            ]
        }
    ]
}
```

# 更新 VS Code 环境变量

VS Code 有一个特别的设置，就是在外面更新了环境变量以后，VS Code 内部的命令行是不知道的。

这个问题参考 [vscode终端不能识别系统设置的环境变量？ - 朝阳的回答 - 知乎](https://www.zhihu.com/question/266858097/answer/967976291)

原因就是 VS Code 想要保存上一次关闭时候的命令行的历史记录，所以没有更新环境变量。

比如新建一个 `Destop/print-hello.exe`，并且添加到环境变量中，然后我们正常 `Win + R, cmd`，调出命令行，是可以直接运行 `print-hello.exe`。

但是，如果这个时候用 VS Code 打开一个项目，项目内部的命令行是不知道有这个环境变量的，它将不能 `print-hello`。

解决方法就是，使用一个“感知到”新的环境变量的命令行，使用指令 `code <workspace>` 重新打开项目，这个时候 VS Code 才会更新环境变量。

# 构建并运行

准备一个 `test.cpp`，内容如下，注意修改对应的图片地址。

```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
int main()
{
    Mat img = imread("./opencv.jpeg");
    imshow("image", img);
    waitKey();
    return 0;
}
```

现在文件目录是这样的：

```sh
.
├── .vscode
│   ├── c_cpp_properties.json
│   ├── launch.json
│   └── tasks.json
├── test.cpp
└── opencv.jpeg
```

点击小三角就行。

![](https://img2022.cnblogs.com/blog/1930568/202210/1930568-20221022235832549-1492485518.png)

# 总结

虽然是以配置 OpenCV 为引子，但是整个流程并不复杂：

1. OpenCV 动态链接库添加到系统环境变量
2. `tasks.json` 中填写编译参数
3. 更新 VS Code 环境变量

至于环境变量之类的，各位老手想必是轻车熟路了。

只要做好上面这三步，就能生成并调试 `.exe` 文件了。

# Bazel 使用

-   created: 2024-12-20T15:39+08:00

[toc]

## 总结

-   MODULE.bazel 用于定义项目的直接依赖和自己要发布的版本
-   BUILD 文件用于标识文件夹是一个 package，并需要在其中书写 build rules，语法类似 Python
    -   每一个 rule 产生的实例叫做 target
    -   target 可以通过绝对路径索引，路径形式如：  
        `//<package-folder>:<rule-name>`
-   rule 有 `cc_binary()` 用于编译可执行文件、 `cc_library()` 用于编译库
-   通过 WORKSPACE 文件管理依赖是历史遗留，里面要手动 load 直接依赖和间接依赖，  
    一些发行的 module 会把这些 load 封装成一个函数并调用

## 单 package 单文件：用 `cc_binary`

首先要知道的就是 WORKSPACE 和 package 的概念[^1]：

> Before you can build a project, you need to set up its workspace.  
> A workspace is a directory that holds your project's source files and Bazel's build outputs.  
> It also contains these significant files:
>
> -   The MODULE.bazel file, which identifies the directory and its contents as a Bazel workspace and lives at the root of the project's directory structure.  
>     It's also where you specify your external dependencies.
> -   One or more BUILD files, which tell Bazel how to build different parts of the project.  
>     A directory within the workspace that contains a BUILD file is a package.

简单来说，就是：

1. MODULE.bazel 告诉 bazel 这个文件夹是一个 workspace；
2. BUILD 告诉 bazel 这个文件夹是一个 package

```shell
single-package-to-executable tree .
.
├── MODULE.bazel
└── hello-world
    ├── BUILD
    └── main.cc
```

BUILD 文件内容：

```python
cc_binary(
    name="build-main",
    srcs=["main.cc"],
)
```

![](single-package-to-executable.svg)

target 是 rule 的实例，像 `cc_binary` 这个 rule 指定编一个二进制可执行文件，这个 rule 的名字叫做 `build-main`。

要触发编译，就是在指定 target，target 对应的 rule 路径为：`//<package-path>:<target-name>`

```shell
bazel build //hello-world:build-main
```

## 单 package 多文件：依赖用 `deps` 和 `cc_library`

样例见 <a href="./single-package-multi-files-to-executable" target="_blank">./single-package-multi-files-to-executable</a>

在 `cc_library` rule 中需要通过 `hdrs` 来指定头文件。

这样的好处是可以增量编译。

```shell
single-package-multi-file-to-executable git:(main) ✗ tree .
.
├── MODULE.bazel
└── hello-world
    ├── BUILD
    ├── echo.cc
    ├── echo.h
    └── main.cc
```

![](single-package-multi-files-to-executable.svg)

`main.cc` **中需要通过绝对路径引用头文件**

```cpp
#include <iostream>
#include "hello-world/echo.h" // <-- look here
using namespace std;
int main() {
    std::cout << "hello world" << std::endl;
    echo("echo hello world");
    return 0;
}
```

## 多 package 编译可执行文件

样例见 <a href="./multi-packages-to-executable/" target="_blank">./multi-packages-to-executable</a>

```shell
.
├── MODULE.bazel
├── calculator
│   ├── BUILD
│   ├── echo.cc
│   ├── echo.h
│   └── main.cc
└── math
    ├── BUILD
    ├── math.cc
    └── math.h
```

math package 要编译成 `library`， 并且需要通过 `visibility` 字段对外暴露：

```python
cc_library(
    name = "math",
    srcs = ["math.cc"],
    hdrs = ["math.h"],
    visibility = ["//calculator:__pkg__"],
)
```

calculator package 要用 `cc_binary` rule 编译， 同时需要指定 deps 为外部的 package，也是用绝对路径

```python
cc_library (
    name = "echo",
    srcs = ["echo.cc"],
    hdrs = ["echo.h"],
)


cc_binary(
    name = "calculator",
    srcs = ["main.cc"],
    deps = [
        ":echo",
        "//math:math"
    ],
)
```

编译指令：

```shell
bazel build //calculator:main
```

## make system 要面临的问题

在介绍 Bazel 如何管理外部依赖前，我们需要了解做一套 make system 要面临哪些问题。

从需求的角度来说，比如我要发布一个 app，它依赖于 Lib A 和 Lib B，  
这很简单，我只需要在 app 中声明使用 A 和 B，  
然后 make system 自动帮我下载好 A 和 B，我就可以使用了。

假设 make system 需要一个叫做 Module 的文件来分析导入和导出依赖，我们 app 的 Module 文件内容如下：

```
export(app)

import(A)
import(B)
```

当版本管理出现后，make system 就犯难了。
如果，我们要 A@1.0.0 和 B@1.0.0，他们都依赖于 C，但是

```
A@1.0.0 -> C@1.0.0
B@1.0.0 -> C@2.0.0
```

这个时候有一种非常麻烦的办法，就是我手动告诉 make system，我要 C 的哪个版本，然后 A 和 B 都用对应版本：
那我们 app 的 Module 就会变成这样：

```
export(app, 1.0.0)

import(C, 1.0.0)
import(A, 1.0.0)
import(B, 1.0.0)
```

长此以往，如果依赖越来越多，就要指定跟多的依赖，这不是一个好办法。  
要是我们只需要指定 app 直接的依赖 A 和 B，让 make system 自动选择可以的 C 就好了。

## WORKSPACE

bazel 传统的依赖指定方式是通过 `WORKSPACE` 或者 `WORKSPACE.bazel` 文件，我们称为 WORKSPACE 方式，  
它就是用上面最麻烦的、需要把间接依赖、直接依赖一个个指定出来[^2]。

不仅如此，这种方式的语法也很恶心：

1. 它连 `import` 函数都不是自带的，需要通过 `load ` 函数加载
2. `import` 函数不指定版本而是需要指定下载的 url 和 sha256

这个例子来自 [bazel-build/examples](https://github.com/bazelbuild/examples/tree/main/bzlmod/01-depend_on_bazel_module)  
假如我们要构建一个 app，它依赖于 glog，但是 glog 还依赖于 gflags，
下面就是它的 WORKSPACE file，注意语法类似于 python：

```python
# https://github.com/bazelbuild/examples/blob/main/bzlmod/01-depend_on_bazel_module/WORKSPACE

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_google_glog",
    sha256 = "eede71f28371bf39aa69b45de23b329d37214016e2055269b3b5e7cfd40b59f5",
    strip_prefix = "glog-0.5.0",
    urls = ["https://github.com/google/glog/archive/refs/tags/v0.5.0.tar.gz"],
)

# We have to define gflags as well because it's a dependency of glog.
http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/refs/tags/v2.2.2.tar.gz"],
)
```

不过 BUILD 文件就很简单：

```python
cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = ["@com_github_google_glog//:glog"],
)
```

## bzlmod

bzlmod 是目前（2024-12-20）最新的管理依赖的方式[^2]，只需要指定直接依赖就好了。  
每一个可以 export 和 import 的代码单元叫做「module」

```python
# https://github.com/bazelbuild/examples/blob/main/bzlmod/01-depend_on_bazel_module/MODULE.bazel

module(
    name = "example",
    version = "0.0.1",
)

# 1. The metadata of glog is fetched from the BCR, including its dependencies (gflags).
# 2. The `repo_name` attribute allows users to reference this dependency via the `com_github_google_glog` repo name.
bazel_dep(name = "glog", version = "0.5.0", repo_name = "com_github_google_glog")
```

这个 bazel 起名字也是够奇怪的，python 是是单文件叫做 module，允许文件夹变成 package 和 sub-package  
bazel 这儿 package 概念没变，整个发布出去的东西叫做 module。

[^1]: [Bazel Tutorial: Build a C\+\+ Project](https://bazel.build/start/cpp#getting-started)
[^2]: [External dependencies overview  \|  Bazel](https://bazel.build/external/overview#:~:text=Define%20repos%20with%20WORKSPACE)

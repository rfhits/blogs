# 管理 Python 环境

-   created: 2025-03-31T10:34+08:00
-   published: 2025-03-31T15:05+08:00
-   category: python

1. `pipx`：用于为 python 工具提供隔离的 python 环境。
   比如 ruff 作为一个 Linter/Formatter，有自己的 Python 版本依赖和包依赖，
   那如果我要使用 ruff，理论上应该专门有一个 Python 环境存放 ruff。
   pipx 就是干这个的。
2. `poetry`：Python 项目的管理工具，对标 npm。
   poetry 本身也是一个类似于 ruff 的工具，要先安装 pipx 才能用。
3. `uv`: rust 写的，对标 poetry。
4. Anaconda/Miniconda/miniforge: 提供了 `conda` 命令。Anaconda 和 Miniconda 商用收费，用 miniforge 平替。
   **poetry 文档中使用 conda 作为环境管理**

总结下来，项目开发用 conda + poetry。

参考：

1. [poetry](https://python-poetry.org/docs/main/)
2. [写 python 的环境选择 - zhihu.com](https://www.zhihu.com/question/666301305/answer/4805392058)

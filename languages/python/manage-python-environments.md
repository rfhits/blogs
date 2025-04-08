# 管理 Python 环境

-   created: 2025-03-31T10:34+08:00
-   published: 2025-03-31T15:05+08:00
-   modified: 2025-04-03T11:47+08:00
-   category: python

阅读了两篇文章：

1. [poetry](https://python-poetry.org/docs/main/)
2. [写 python 的环境选择 - zhihu.com](https://www.zhihu.com/question/666301305/answer/4805392058)

做出决定：

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

## poetry 简单使用

1. poetry 结合了「打包」和「依赖管理」两个功能，如果只是想要管理依赖，在 `pyproject.toml` 中设置 `package-mode = false`[^1]
2. poetry 可以自动识别对应的虚拟环境，非常方便
3. 使用 `poetry add {package}` 就相当于 `pip install {package}`
4. 直接 `poetry add` 安装的依赖在 `pyproject.toml`，背后的依赖在 `poetry.lock` 文件中

## 问题

### conda activate 后 which python 不是激活的对应环境

`conda activate {env}` 以后，which python 不是激活的对应环境
解决方法: `conda init {shell}` 本质编辑了对应的 `~/.{shell}rc`，
追加了一个脚本，该脚本再编辑环境变量 `$PATH`
which python 不是激活的对应环境，说明 编辑的太后面了，在脚本后面追加
`export PATH="$CONDA_PREFIX/bin:$PATH"`

参考：

[^1]: https://python-poetry.org/docs/main/basic-usage/#operating-modes

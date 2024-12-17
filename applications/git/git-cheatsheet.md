# Git 常用命令

-   modified: 2024-12-17T12:42+08:00
-   published: 2024-12-07T13:22+08:00
-   categories: git

[toc]

## Git 数据模型

[版本控制\(Git\) · the missing semester of your cs education](https://missing-semester-cn.github.io/2020/version-control/)

1. 提交记录构成有向无环图，后提交的记录依赖于前面的记录  
   比如 merge 后的提交记录会有两个 parent
2. 数据包括 tree，blob 和 commit，通过 sha1 得到哈希值存储

## Git 配置

`.git` 文件夹下有个 `config` 文件，里面记载了仓库的地址、用户名和邮箱等信息。  
如果不希望全局配置对当前仓库生效，可以直接编辑 `config` 文件。

关于查看配置的命令：

```bash
# 查看所有的配置
git config --list

# 查看某个配置，显示的全局的配置
git config user.name
git config user.email

# 某个项目根目录下使用，显示的是 全局配置+当前项目的配置
git config --list
```

### 配置邮箱和用户名

Git 的 config 会作为 `git commit`时的相关信息，但不会作为验证信息，这一点是很多新手的误区。能不能推送上去，只和 ssh、https 验证是否通过有关，和 username 及 email 无关。

也就是说，随便设置 username 和 email，再配置好 ssh，一样能推送上去。

那么为什么还要设置 username 和 email？原因是，在 GitHub 的个人主页上，所有使用非个人邮箱的推送是不会被“count”的。

在 GitHub 的 docs 上关于哪些 commit 会被计算进入主页，有这么一段话：

> Commits will appear on your contributions graph if they meet **all** of the following conditions:
>
> -   The email address used for the commits is **associated with** your GitHub account.
> -   The commits were made in a standalone repository, not a fork.
> -   The commits were made:
>     -   In the repository's default branch
>     -   In the `gh-pages` branch (for repositories with project sites)
>
> --[GitHub docs](https://docs.github.com/en/github/setting-up-and-managing-your-github-profile/why-are-my-contributions-not-showing-up-on-my-profile#commits)

也就是说，小明的 GitHub 账号是"xiaomin@email.com"，如果 git config 里把邮箱设置成"xiaohong@email.com"，那推送是无法在个人主页显示的。

虽然这有一点“面子工程”的嫌疑，但是看到自己的主页绿油油的，心里还是会有一点开心？！

为所有 git 仓库设置邮箱和用户名的命令如下：

```bash
git config --global user.name "your global username"
git config --global user.email "your global email"
```

如果你有多个 git 仓库，并且想要为它们设置不同的 username 和 email，需要在仓库的根目录下，使用以下命令进行配置：

```bash
git config user.name "your all repo username"
git config user.email "your all repo email"
```

这个配置将会被写入到 `.git/config` 这个文件里。

参考[Git 全局配置和单个仓库的用户名邮箱配置](https://blog.csdn.net/qq_29232943/article/details/103121667)

### 配置代理

查看配置

```bash
git config --global --get http.proxy
git config --global --get https.proxy
```

修改配置

```bash
# 如果本地的代理服务器用 http 协议，只需要设置这两条
# 如 clash 软件允许复制终端代理指令，查看可知本地 clash 使用的是 http
git config --global http.proxy http://127.0.0.1:7890
git config --global https.proxy http://127.0.0.1:7890

# 如果本地代理服务器使用socks5协议，设置下面两条，端口号也是 clash 默认的
git config --global http.proxy socks5://127.0.0.1:7890
git config --global https.proxy socks5://127.0.0.1:7890
```

取消配置

```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```

参考：[config a proxy to work with git](https://www.delftstack.com/howto/git/git-setup-proxy/)

## gitignore

常常会把不想添加的文件被 checked，所以有如下命令，清空 git 的 check

```bash
git rm -r --cached .
git add .
git commit -m 'update .gitignore'
```

## commit

### empty commit

记录一下 empty commit 的命令

```bash
git commit --allow-empty -m "commit message"
```

### 规范填写 commit message

[Git 如何规范填写 Commit Message_CSDN](https://blog.csdn.net/asc_123456/article/details/118070026)

```bash
<type>[optional scope]: <description>
// 空行
[optional body]
// 空行
[optional footer(s)]
```

type 填对最重要，如 fix, feat, test, refactor, perf, style, docs

| 类型     | 类别        | 说明                                                                                 |
| -------- | ----------- | ------------------------------------------------------------------------------------ |
| feat     | Production  | 新增功能                                                                             |
| fix      | Production  | bug 修复                                                                             |
| perf     | Production  | 提高代码性能的变更                                                                   |
| style    | Development | 格式化代码                                                                           |
| refactor | Production  | 其他的代码变更，不属于 feat、fix、perf、style 例如代码简化、重命名变量、删除冗余代码 |
| test     | Development | 新增或修改测试用例                                                                   |
| ci       | Development | 持续集成和部署相关的变动                                                             |
| docs     | Development | 文档类更新                                                                           |
| chore    | Development | 其他类型，比如构建流程、依赖管理或者辅助工具的变动                                   |

### 合并多个 commit

参考[git 合并多个 commit-睿站](https://www.bilibili.com/video/BV15h411f74h)

rebase 的意思是，以指定 commit 为 base

```bash
git log --oneline # 查看最近的git log
git rebase -i <要合并的commit前一个的hash>
# 将合并的 pick 改成 s
# 重新填写 commit 信息
```

## 分支和协同开发

### 本地创建切换分支

```bash
git checkout -b dev
```

现在位于 dev 分支下

### 推送到远程分支

```bash
git push -u origin dev
```

### 本地合并 dev 分支

先到把 dev 合并的分支，比如 main

```bash
git checkout main
```

然后 merge

```bash
git merge dev
```

### 删除本地和远程的 dev 分支

先删除本地分支

```bash
git branch -d dev
```

再删除远程分支

```bash
git push origin --delete dev
```

参考[Git 合并分支](https://www.jianshu.com/p/26d050497abb)

## github 加速

参考[fastgit 使用指南](https://doc.fastgit.org/zh-cn/guide.html)

## 其他

### 只需要最新的文件，不需要记录：`--depth 1`

```shell
git clone --depth 1 <repo>
```

比如要编译一个 C++ 库，只需要它对应仓库的文件就好了，不需要它的提交记录。  
像 Google 的 grpc 这个库，安装教程中就使用了这个命令。

```shell
git clone --recurse-submodules -b v1.66.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
```

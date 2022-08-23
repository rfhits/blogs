[toc]

## 个人信息配置

Git的config会作为`git commit`时的相关信息，但不会作为验证信息，这一点是很多新手的误区。能不能推送上去，只和ssh、https验证是否通过有关，和username及email无关。

也就是说，随便设置username和email，再配置好ssh，一样能推送上去。

那么为什么还要设置username和email？原因是，在GitHub的个人主页上，所有使用非个人邮箱的推送是不会被“count”的。

在GitHub的docs上关于哪些commit会被计算进入主页，有这么一段话：

> Commits will appear on your contributions graph if they meet **all** of the following conditions:
>
> * The email address used for the commits is **associated with** your GitHub account.
> * The commits were made in a standalone repository, not a fork.
> * The commits were made:
>   * In the repository's default branch
>   * In the `gh-pages` branch (for repositories with project sites)
>
> --[GitHub docs](https://docs.github.com/en/github/setting-up-and-managing-your-github-profile/why-are-my-contributions-not-showing-up-on-my-profile#commits)

也就是说，小明的GitHub账号是"xiaomin@email.com"，如果git config里把邮箱设置成"xiaohong@email.com"，那推送是无法在个人主页显示的。

虽然这有一点“面子工程”的嫌疑，但是看到自己的主页绿油油的，心里还是会有一点开心？！

为所有git仓库设置邮箱和用户名的命令如下：


```bash
git config --global user.name "your global username"
git config --global user.email "your global email"
```

如果你有多个git仓库，并且想要为它们设置不同的username和email，需要在项目的根目录下，使用以下命令进行配置：


```bash
git config  user.name "your all repo username"
git config  user.email "your all reop email" 
```

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



## 代理

```bash
git config --global https.proxy http://127.0.0.1:7890
git config --global https.proxy https://127.0.0.1:7890
git config --global http.proxy socks5://127.0.0.1:1080
git config --global https.proxy socks5://127.0.0.1:1080
```

取消
```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```
## gitignore

常常会把不想添加的文件被checked，所以有如下命令，清空git的check

```bash
git rm -r --cached .
git add . 
git commit -m 'update .gitignore'
```



## commit

### commit message

[Git如何规范填写Commit Message_CSDN](https://blog.csdn.net/asc_123456/article/details/118070026)

```bash
<type>[optional scope]: <description>
// 空行
[optional body]
// 空行
[optional footer(s)]
```
type 填对最重要，如 fix, feat, test, refactor, perf, style, docs

| 类型 | 类别 | 说明 |
| --- | --- | --- |
| feat | Production | 新增功能 |
| fix | Production | bug修复 |
| perf | Production | 提高代码性能的变更 |
| style | Development | 格式化代码 |
| refactor | Production | 其他的代码变更，不属于feat、fix、perf、style例如代码简化、重命名变量、删除冗余代码 |
| test | Development | 新增或修改测试用例 |
| ci | Development | 持续集成和部署相关的变动 |
| docs | Development | 文档类更新 |
| chore | Development | 其他类型，比如构建流程、依赖管理或者辅助工具的变动 |

### empty commit

记录一下empty commit的命令

```bash
git commit --allow-empty -m "commit message"
```

### 合并多个 commit

参考[git 合并多个 commit-睿站](https://www.bilibili.com/video/BV15h411f74h)

```bash
git log --oneline # 查看最近的git log
git rebase -i <要合并的commit前一个的hash>
# 将合并的 pick 改成 s
# 重新填写 commit 信息
```

## 协同开发

### 本地创建切换分支

```bash
git checkout -b dev
```

现在位于dev分支下

### 推送到远程分支

```bash
git push -u origin dev
```

### 本地合并dev分支

先到把dev合并的分支，比如main

```bash
git  checkout main
```

然后merge

```bash
git merge dev
```

### 删除本地和远程的dev分支

先删除本地分支

```bash
git branch -d dev
```

再删除远程分支

```bash
git push origin --delete dev
```

## 参考资料

[1] [Git全局配置和单个仓库的用户名邮箱配置](https://blog.csdn.net/qq_29232943/article/details/103121667)

[2] [Git合并分支](https://www.jianshu.com/p/26d050497abb)
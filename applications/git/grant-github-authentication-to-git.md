git clone GitHub 仓库报错未授权

-   created: 2024-06-03T20:40+08:00
-   published: 2024-06-03T20:44+08:00
-   categories: git
-   tags: bugs

GitHub 开启双重认证以后，wsl 又没有浏览器来做认证，就不能只依靠 GitHub 的账号密码来 git clone repo 了。

这里提供一种方法，安装 Github Command Line 来 clone repo。

以下是 Ubuntu 的命令：

```bash
sudo apt install gh
gh repo clone <repo>
```

然后按照步骤认证即可，一般是打开浏览器之类的。

参考链接：

1. [GitHub CLI \| Take GitHub to the command line](https://cli.github.com/manual/gh_repo_clone)
2. [About GitHub CLI \- GitHub Docs](https://docs.github.com/en/github-cli/github-cli/about-github-cli)
3. [Manual \| GitHub CLI](https://cli.github.com/manual/)

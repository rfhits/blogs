```bash
C:\Users\usr>npm --version
7.24.1
```

## 起因

作为苦逼前端，得和npm打交道，这不算什么，最苦逼的是要和各种依赖打交道。

众所周知，npm会把依赖放在`package.json`和`package-lock.json`中。

众所不知，如果这个项目被clone了，直接`npm install`可能会改写`package-lock.json`。

这一点非常让人无语。

开发者安装了各种依赖，生成了自己的`package.json`和`package-lock.json`，然后push，

另一个开发者拿到了这两个文件，`npm install`完以后，`package-lock.json`就变了，他一推送，完了，原来开发者的`package-lock.json`也变了，整个项目就跑不起来了。

没错，说的就是我和我的后端。。。

## 解决

有`package-lock.json`的时候，就不要`npm install`了，使用命令`npm ci`。

唯一让我无语的是，`npm ci`会直接删除`node_modules`，毕竟`node_modules`里面是记录了每个包的版本的，一些已经安装过的包，会被直接删除，然后重新安装一遍。当然，这一点可以通过加上cache参数解决，见参考[2]。

## 参考

1. [package-lock.json 冲突解决](https://zhuanlan.zhihu.com/p/80770507)

2. [npm ci 与 install 的区别 - 牛蛙泳的文章 - 知乎](https://zhuanlan.zhihu.com/p/369845020)

3. [npm ci命令](https://www.zhuyuntao.cn/npm-ci%E5%91%BD%E4%BB%A4)
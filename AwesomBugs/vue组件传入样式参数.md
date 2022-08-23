## 起因

做一个el-card，可以自定义宽度，而不是自动填满整个div。

就是把组件的宽度绑定到一个传入的参数上：

```html
<el-card :style="{width: this.cardData.width}">
</el-card>
```

下面是有问题的代码：

```html
<el-card :body-style="{width: 'this.cardData.width'}">
</el-card>
```

修改`body-style`为`style`，删除`width`旁的引号后，功能终于正常了。

奇怪的是el-card的介绍里说要用`body-style`，可是偏偏不起作用。
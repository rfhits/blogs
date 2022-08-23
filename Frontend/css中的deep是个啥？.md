关键词：/deep/ vue css 前端

比如要修改elementUI中某个组件的默认样式，

假设默认鼠标飘到el-button上，button会变蓝，

而我们想又要让button变红，可是全局修改又会使得所有的组件中button的style变化，

那么就要/deep/，表示就修改这个组件中的样式。

参考：

1. [Vue中的深度作用选择器/deep/](https://cloud.tencent.com/developer/article/1601554)
2. [Scoped CSS](https://vue-loader.vuejs.org/zh/guide/scoped-css.html#%E6%B7%B1%E5%BA%A6%E4%BD%9C%E7%94%A8%E9%80%89%E6%8B%A9%E5%99%A8)
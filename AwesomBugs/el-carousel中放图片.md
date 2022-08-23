`el-carousel`是一个非常恼人的组件。

它内部实现的逻辑是这样的：

```html
<el-carousel>
    <el-carousel__container>

        <el-carousel__item></el-carousel__item>

    </el-carousel__container>

    <el-carousel__indicators></el-carousel__indicators>
</el-carousel>
```

我们能够修改的只有`item`，`item`在`container`中的布局是无法修改的。

比如无法调整`item`到`indicators`的位置，会出现`item`只占据了`container`中上半部分的情况。

所以我们`/deep/`到`container`里面把它高度改了就行。

```css
.el-carousel /deep/ .el-carousel__container {
  height: 200px;
}
```
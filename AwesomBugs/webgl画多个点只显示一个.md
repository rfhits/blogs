# webgl画多个点只显示一个

## 背景

学习webgl编程指南时，看到绘制多个顶点那一章，结果画不出来。

## 解决方法

在定义顶点着色器的时候，要制定点的大小。代码如下：

```glsl
attribute vec4 a_Position;
void main() {
    gl_Position = a_Position;
    gl_PointSize = 10.0; // 不加这一行，只能画一个点
}
```

如果不指定点大小，就只能画出一个点。

更神奇的是，加完之后，刷新，再去掉，又能画三个点了。

这是什么神仙bug跺jiojio绝绝子~

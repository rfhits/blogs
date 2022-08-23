# 使用WebGL画点

webgl给我的感觉就像是老太太的裹脚布，又臭又长。层层封装，每个人代码都不一样，缺少统一的接口。

自顶向下有理论，guide讲的清楚，但是教材是自顶向下，所以要把guide的代码都搬过去。

## 着色器

顶点着色器控制点的位置和大小，片元着色器控制点的颜色。

函数调用关系：

gl.drawArrays()->顶点着色器->main->片元着色器->main

## 取出点并赋值

```glsl
var program = initShaders( gl, "vertex-shader", "fragment-shader" );
gl.useProgram( program );
gl.program = program;
```

获取program中一个点，然后传值。

```js
var a_Position = gl.getAttribLocation(gl.program, 'a_Position');
gl.vertexAttrib3f(a_Position, 0.0, 0.0, 0.0);
```


attribute是沟通的桥梁，也可以说是沟通的约定，如果js要拿到program对象里的变量，就得知道attribute变量的名字。

## 绘制多个点

缓冲区是显存里的，能被gl访问。就像手动malloc一块内存一样

1. 创建缓冲区对象：`gl.createBuffer()`
2. 绑定缓冲区对象，即指定用途
3. 将数据写入缓冲区对象
4. 将缓冲区对象分配给一个attribute变量
5. 开启attribute变量
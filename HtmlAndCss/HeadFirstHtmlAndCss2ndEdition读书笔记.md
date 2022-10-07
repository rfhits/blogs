Head First Html and Css 2nd 中文读书笔记

书籍链接：https://book.douban.com/subject/25752357/

[toc]

# 块元素和内联元素

P94

块元素：block

内联元素：inline

作者使用 `<q>`和 `<blockquote>`举例，说明块元素和内联元素的区别

块元素如 `<blockquote>`内的元素会另起一行，达到“换行”的效果，而内联元素 `<em>`、`<a>`等不会。

# 链接到页面的特定位置

P149

在html中元素的属性上加id，然后 `url#id`即可

我一直以为这是chrome自带的某种feature，现在才发现是html标准。

这个可以用于 `back to top`，只需要在页面的顶部设置一个 `id="top"`

# html5

活的标准，由w3c带来

html标准，就叫做html5，因为不是”死的标准“，未来可能会有新的特性加入，所以称之为”活的标准“。

有一个网站，网址为https://validator.w3.org/，可以验证html编写是否符合规范

html5的一个优点是允许开发人员创建web应用，比如地图

# 字体

## 字体样式

P326

@fontface

```css
@font-face {
  font-family: "name";
  src: url("link1"),
       url("link2");
}
```

找到一种字体，并为其分配一个名字，可以在自己的css样式中引用这个名字。

## 字体大小

P330

字体大小也是属于”继承“属性。

em作用和百分比是一致的，指的是相对于父级元素字体的大小。

如何设计页面的字体大小，推荐的做法是对body设置一种大小，如medium或者small，其他字体大小通过em或者百分比来调整。

## text-color?

p353

color为什么不叫作”text-color“

因为color控制的是”前景色“

# media

P400

控制在不同设备上的显示方式。

```html
<link href="lounge-mobile.css" rel="stylesheet" media="screen and (max-device-width: 480px)"
```

也可以在css上直接添加媒体查询，语法是 `@media`

# div和span

## text-align

P432

text-align只能在块级元素上设置，对inline元素生效，所以虽然叫做”text“，但是也可以对img生效

## span

P447

div是块元素的”逻辑分组“，span是内联元素的”逻辑分组”

# 伪类

P454

直观来看，提供用户交互时的样式定制，如

```css
a:hover {
  color: red;
}
```

但是，背后实现原理，是浏览器一直在检查这些元素，把它们放到对应的伪类中。

其实Google搜索结果页面，那些访问过的链接的颜色，应该就是通过这个方法实现的

# 层叠

层叠的意思就是很多css都作用于一个元素，浏览器要根据继承、重要性、默认属性等方法，决定这个元素的样式。

# flow与外边距

flow指的是浏览器如何排列这些元素，对于块级元素，从上往下，内联元素，左上往右下。

垂直外边距会重叠

水平外边距表现正常

# float

float属性让元素脱离这个流，元素在页面的垂直位置和不设置float时候一致。后续的元素将会从忽略这个元素的地方开始。

对于后续的块级元素，渲染时，这个float元素对其视而不见，会导致重叠

但是后续元素的内联元素，如文字，不会被其遮挡。

![img](https://img2022.cnblogs.com/blog/1930568/202210/1930568-20221007154730880-1815798762.png)

红色部分就是float，可以看见他脱离了文档流，导致流中其他块级元素重叠在一起

但是流中的内联元素，比如文字，又会考虑到这个浮动的元素。

float本意是图片旁边的文字围绕，不是用float来布局。

# 绝对定位

P506

```css
.annoying-ad {
    position: absolute;
    top: 100px;
    left: 100px;
}
```

会从文档流中删除之，根据top和left进行定位

流中的其他元素不知道有这么个“绝对定位”的元素存在，这可不像float，因为内联元素会调整而适应float

# 固定定位

相对于浏览器窗口的定位，不会随着用户滚动屏幕变化

比如弹出来的广告，必须手动x掉

# html5元素

nav article aside footer section time

section和div很像，section用与“含义”更丰富的块

# video

P589

对于 `<video>`的属性，没啥好说的，值得一提的是

```html
<viedo>
    <source src="video1.fmt1">
    <source src="video2.fmt2">
    <p>
        Sorry, your browser doesn't support the video we provide.
    </p>
</viedo>
```

因为当时不同浏览器支持的格式不同，如mp4、WebM，所以要根据平台选择

# 表格

P604

- `<table>`表格
- `<th>`table header
- `<tr>`table row
- `<td>`table data

P614

表格单元如 `th`和 `td`可以设置padding，也就是内边距，当时不能单独设置外边距，而是有一个border spacing属性，为整个表格设置单元格之间的间距。

斑马纹可以使用伪类nth-child设置

## 合并单元格

P622

视觉上的行的合并是通过删除行和增加行的高度实现的，列的合并同理。

`rowspan`属性可以指定 `<td>`的高度

如果第1行一个单元格高度为2，恰好下一行，这个单元格下面没有单元格，也就是 `<td>`缺失，那么视觉上就是”合并单元格“

同理，`colspan`可以指定 `<td>`的宽度，实现横向合并单元格

# 表单

P670

一个发起百度搜索的例子如下：

输入关键词后，可以实现页面的跳转

```html
<form action="https://www.baidu.com/s" method="get">
		<p>
			wd: <input type="text" name="wd"><br>
		</p>
    	<input type="submit">
</form>
```

<form action="https://www.baidu.com/s" method="get">
		<p>
			wd: <input type="text" name="wd"><br>
		</p>
    	<input type="submit">
</form>

会向url提交一个表单，

比如 `<input type="text" name="wd">`，这个form里含有的一个键值对就是 `wd: <value u input>`

输入的类型有很多，比如单选 `radio`，下拉框单选：`select`

## 表单的布局

P683

表单通常使用表格布局，但是不是通过使用 `<table>`实现，而是通过 `<div>`和 `<p>`使其表现得像一张表格

每一行都是一个div

```css
div.tableRow {
  display: table-row;
}

div.tableRow p {
  display: table-cell;
  vertical-align: top;
  padding: 3px;
}
```

# 兼容性

P698

不同厂商有自己的css设计，我们可能看到如下代码：

```css
#box:hover {
    transform: rotate(45deg);
    -webkit-transform: rotate(45deg);
    -moz-transform: rotate(45deg);
    -o-transform: rotate(45deg);
    -ms-transform: rotate(45deg);
}
```

可能不同浏览器对 `roate(45deg)`有不同的样式实现

webkit是chrome的，moz是火狐的，o是opera的

# 动画

只是简单提了一嘴，我们知道有这么个东西

```html
<!DOCTYPE html>
<html>

<head>
	<title>CSS Transforms</title>
	<style>
		#box {
			position: absolute;
			top: 100px;
			left: 100px;
			width: 200px;
			height: 200px;
			background-color: red;
			transition: transform 2s;
		}

		#box:hover {
			transform: rotate(45deg);

		}
	</style>
</head>

<body>
	<div id="box"></div>
</body>

</html>
```

# 感想

国庆7天，囫囵吞枣地看完了。

我觉得最重要的是学到了”flow“的概念，剩下的就是一些”概念“，比如”块级元素“等，最后才是一些新的标签和用法。

新手友好型，可是讲得不深。

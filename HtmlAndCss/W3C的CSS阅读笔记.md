W3C 的 CSS 阅读笔记

阅读地址：https://www.w3school.com.cn/css.index.asp

# 颜色

- RGB: 使用光源，Hex 只是 RGB 三个数字 16 进制表示的粘合
- HSL
  - hue: 色相，色轮上的颜色
  - saturation: 饱和度
  - lightness: 亮度

# 图像

取值为 `fixed/scroll`，前者会使得图像不随页面滚动

```css
 background-attachment: fixed;
```

# 边距

> 您可以将 `margin` 属性设置为 `auto`，以使元素在其容器中水平居中。

若要将宽度保持为 300 px，无论填充量如何，那么您可以使用 `box-sizing` 属性。这将导致元素保持其宽度。如果增加内边距，则可用的内容空间会减少。

# 文本

`text-transform` 属性用于指定文本中的大写和小写字母。

它可用于将所有内容转换为大写或小写字母，或将每个单词的首字母大写

视口（Viewport）是浏览器窗口的大小。 1 vw = 视口宽度的 1％。如果视口为 50 厘米宽，则 1 vw 为 0.5 厘米。

# Display

块级元素总是从新行开始，并占据可用的全部宽度（尽可能向左和向右伸展）。

 `display: inline` 相比，主要区别在于 `display: inline-block` 允许在元素上设置宽度和高度。

`display: none`，元素不可见，不占据原来空间

`visibility: hidden`：元素不可见，但是还是占据原来的空间

`max-width` 和 `width` 区别：

前者当浏览器宽度过小时，元素的宽度也会变化；后者元素宽度是死的，并且浏览器会添加scrollBar

# position

relative: 相对自己定位

absolute: 相对父元素

`position：absolute/fixed` 元素开启绝对定位和固定定位之后才会脱离文档流

`position: sticky`，一种使用 relative 和 fixed 的混合定位，当元素滚动到那个对应位置的时候，浏览器就会把它黏在那个位置

# 溢出

> overflow 属性控制对太大而区域无法容纳的内容的处理方式。

# 居中

https://www.w3school.com.cn/css/css_align.asp

flex肯定是万能的

## 块级元素居中

`margin: auto`，再为元素设置宽度，不然div默认是填满整个水平的

## 垂直居中

父级元素设置 padding

> 另一个技巧是使用*其值*等于 `height` 属性值的 `line-height` 属性

## 字体

- `text-align: center`
- height 和 line height 相等

# 属性选择器

https://www.w3school.com.cn/css/css_attribute_selectors.asp

选中具有某个属性的元素

```css
a[target="_blank"] {
  background-color: yellow;
}
```

> 举例来说，如果希望对指向 W3School 的所有链接应用样式，不必为所有这些链接指定 class，再根据这个类编写样式，而只需编写以下规则：

```css
a[href*="w3school.com.cn"] {color: red;}
```

有点像正则表达式

| 选择器                                                                                             | 例子                | 例子描述                                                 |
| :------------------------------------------------------------------------------------------------- | :------------------ | :------------------------------------------------------- |
| [[*attribute*\]](https://www.w3school.com.cn/cssref/selector_attribute.asp)                        | [target]            | 选择带有 target 属性的所有元素。                         |
| [[*attribute*=*value*\]](https://www.w3school.com.cn/cssref/selector_attribute_value.asp)          | [target=_blank]     | 选择带有 target="_blank" 属性的所有元素。                |
| [[*attribute*~=*value*\]](https://www.w3school.com.cn/cssref/selector_attribute_value_contain.asp) | [title~=flower]     | 选择带有包含 "flower" 一词的 title 属性的所有元素。      |
| [[*attribute*\|=*value*\]](https://www.w3school.com.cn/cssref/selector_attribute_value_start.asp)  | [lang\|=en]         | 选择带有以 "en" 开头的 lang 属性的所有元素。             |
| [[*attribute*^=*value*\]](https://www.w3school.com.cn/cssref/selector_attr_begin.asp)              | a[href^="https"]    | 选择其 href 属性值以 "https" 开头的每个`<a>` 元素。      |
| [[*attribute*$=*value*\]](https://www.w3school.com.cn/cssref/selector_attr_end.asp)                | a[href$=".pdf"]     | 选择其 href 属性值以 ".pdf" 结尾的每个`<a>` 元素。       |  |
| [[*attribute\*=value*\]](https://www.w3school.com.cn/cssref/selector_attr_contain.asp)             | a[href*="w3school"] | 选择其 href 属性值包含子串 "w3school" 的每个`<a>` 元素。 |

# 伪类

用于定义元素的特殊状态

first-child：当元素成为另一个元素的第一个子元素时，生效的css

伪类配合选择器使用

```css
p {
    display: none;
}

div:hover p {
    color: red;
    display: inline-block;
}
```

# 伪元素

CSS 伪元素用于设置元素指定部分的样式。也就是元素部分内容的样式。

```css
p::first-letter {
  color: #ff0000;
  font-size: xx-large;
}
```

把段落的第一个字母大写，很像论文里使用的。

指定的内容也不定是元素的一部分，下面这个例子在H1标题前插入一张图片：

```css
h1::before {
  content: url(smiley.gif);
}
```

`::selection`用于指定被选中内容的样式，比如“选中一段文字”，然后再进行复制粘贴

```css
::selection {
  color: red; 
  background: yellow;
}
```

# 导航栏

导航栏一般基于 `ul ol a`

首先是删除默认格式：

```css
ul {
  list-style-type: none;
  margin: 0;
  padding: 0;
}
```

## 垂直导航栏

https://www.w3school.com.cn/css/css_navbar_vertical.asp

布局：

- 竖排，display block
- 固定在侧栏，position fixed
- 当过长时，overflow auto，自动添加 scrollbar

交互：

- 当前处于某个导航目录下，要为该目录添加 active 类
- 其他要设置 hover

```css
li a.active {
  background-color: #4CAF50;
  color: white;
}

li a:hover:not(.active) {
  background-color: #555;
  color: white;
}
```

## 水平导航栏

- 横排：display: inline，或者float

# drop down

展开的内容的 position 设置为 `position: absolute`，从文档流中删除，这样就会根据自己的父元素定位。

展开的内容 `display`一开始设置为 `none`，不显示，当 hover 到父元素时才 `display: block`

```html
<div class="dropdown">
    <button>
        Dropdown
    </button>
    <div class="dropdown-content">
        <a></a>
        <a></a>
    </div>
</div>

<style>
    a {
        display: inline;
    }
    .dropdown-content { 
        display: none;
    }
    .dropdown:hover .dropdown-content {
        display: absolute;
    }
</style>
```

# 图像精灵

https://www.w3school.com.cn/css/css_image_sprites.asp

一个网页如果有很多图片，那么就会向服务器发起多个请求。

如果一个网页只对应寥寥几张图片，我们需要的图片，只是这个图片上“裁剪”一小块的内容，就能减少请求数量。

# 计数器

https://www.w3school.com.cn/css/css_counters.asp

看个例子，在html中实现如下效果：

```sh
1. 
2.
  2.1
  2.2
3.
  3.1
```

```css
body {
  counter-reset: section; /*创建一个叫做 section 的计数器 */
}

h2::before {
  counter-increment: section; /*让这个计数器遇到 h2 就加一*/
  content: "Section " counter(section) ": "; /*在 h2 前展示内容*/
}
```

创建变量并使用

- `counter-reset` - 创建或重置计数器
- `counter-increment` - 递增计数器值
- `content` - 插入生成的内容
- `counter()` 或 `counters()` 函数 - 将计数器的值添加到元素

# 网站布局

https://www.w3school.com.cn/css/css_website_layout.asp

## float

讨论分栏，对于古早的浏览器，要分栏的内容，分配一个 `class="column"`，然后在 css 中设置 `.column: float`，使用浮动布局，分栏的数量取决于 `width: x%`。

但是这样带来一个问题：float 会从文档流中删除，如果父元素内部**只**放置一堆 float  的元素，父元素高度就没了，就像下面这样：

```html
<div class="father"> 
    <div class="float"></div>
    <div class="float"></div>
</div>
```

解决方法：父元素那一堆 float 元素的脚下，再放置一个 clear: both 的块级元素，因为这样，这个元素就必须显示出来，就把父元素撑开了。

```html
<div class="father"> 
    <div class="float"></div>
    <div class="float"></div>
    <div style="clear: both;"></div>
</div>
```

通过伪类来实现：

```html
<div class="father"> 
    <div class="float"></div>
    <div class="float"></div>
</div>

<style>
    .father:after {
        clear: both;
    }
</style>
```

## 响应式布局

如果要实现响应式布局，使用 `@media`，更改 `column` 的宽度。

更加现代的方式是使用 `flex` 布局，但是 `< ie10` 不支持。

# css渐变

## 线性渐变

https://www.w3school.com.cn/css/css3_gradients.asp

方向和颜色

```css
background-image: linear-gradient(direction, color-stop1, color-stop2, ...);
```

- direction: 默认从上到下，可取值为 `to [right/left] [top/bottom]`，也可以填一个角度
- color-stop-x: 支持rgba，重点是a哦

## 径向渐变

https://www.w3school.com.cn/css/css3_gradients_radial.asp

# 转换

https://www.w3school.com.cn/css/css3_2dtransforms.asp

transform属性，让元素相对于原来元素有个偏移

| translate(*x*,*y*) | 定义 2D 转换，沿着 X 和 Y 轴移动元素。   |
| ------------------ | ---------------------------------------- |
| translateX(*n*)    | 定义 2D 转换，沿着 X 轴移动元素。        |
| translateY(*n*)    | 定义 2D 转换，沿着 Y 轴移动元素。        |
| scale(*x*,*y*)     | 定义 2D 缩放转换，改变元素的宽度和高度。 |
| scaleX(*n*)        | 定义 2D 缩放转换，改变元素的宽度。       |
| scaleY(*n*)        | 定义 2D 缩放转换，改变元素的高度。       |
| rotate(*angle*)    | 定义 2D 旋转，在参数中规定角度。         |
| ...                | ....                                     |

# 过渡

> CSS 过渡允许您在给定的时间内平滑地改变属性值。

我们要明确：

- 监听何种属性
- 过渡延迟
- 过渡需要多少时间
- 过渡是否平滑，还是先快后慢，还是……

```css
div {
  transition-property: transform;
  transition-duration: 2s;
  transition-timing-function: linear;
  transition-delay: 1s;
}
```

简写：

```css
div {
  transition: transform 2s linear 1s;
}
```

我把 `transition` 理解为一种“监听”，比如监听元素宽度变化，高级一点就是监听元素 `transform` 的变化。

定义好元素初始状态，以及目标状态，最后加上 transition 即可。

下面例子，鼠标在元素上，transform。

```css
.trans {
    width: 200px;
    height: 300px;
    background-color: red;
    margin: auto;
    transition: transform 2s;
}

.trans:hover {
    transform: rotate(20deg)
}
```

# 动画

https://www.w3school.com.cn/css/css3_animations.asp

- 给动画起个名字
- 为该动画指定关键帧
- 延迟、动画应运行的次数、速度曲线
- 是否改变播放后元素的状态

```css
/* 动画代码 */
@keyframes example {
  0%   {background-color: red;}
  25%  {background-color: yellow;}
  50%  {background-color: blue;}
  100% {background-color: green;}
}

/* 应用动画的元素 */
div {
  width: 100px;
  height: 100px;
  background-color: red;
  animation-name: example;
  animation-duration: 4s;
}
```

# tooltip

https://www.w3school.com.cn/css/css_tooltip.asp

鼠标移动到元素上，弹出一个悬浮框，这叫做 tooltip

难点在于定位和小箭头

- 定位配合 relative 和 absolute
- 小箭头是通过伪类和 border-color 对四个角落颜色的控制实现的

```css
.tooltip .tooltiptext::after {
    content: "";
    position: absolute;
    top: 50%;
    right: 100%;
    margin-top: -5px;
    border-width: 5px;
    border-style: solid;
    border-color: transparent black transparent transparent;
}
```

# button

https://www.w3school.com.cn/css/css3_buttons.asp

改变 button 的默认样式是基本操作，还有更复杂的样式

悬浮按钮变色：transform

悬浮显示">>"：使用伪类

立体感，点击时候下压：`:active`时候，transform x轴向下

# css变量

https://www.w3school.com.cn/css/css3_variables.asp

比如定义一个颜色变量，然后用的时候引用这个变量就行，修改颜色的时候，可以避免一直复制粘贴颜色的 RGB。

变量分全局和局部，全局变量定义要在 `:root`。

```css
:root {
  --blue: #1e90ff;
  --white: #ffffff;
}

body {
  background-color: var(--blue);
}
```

# misc

## border-sizing

一个 element 元素的宽度 = content-width + padding + border

导致宽度 != width，比较违和

> 如果在元素上设置了 `box-sizing: border-box;`，则宽度和高度会包括内边距和边框

> 将其应用于所有元素是安全且明智的

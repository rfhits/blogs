现代JavaScript教程第二部分读书笔记

书籍网址：https://zh.javascript.info

# 浏览器

https://zh.javascript.info/browser-environmen

DOM：文档对象模型，比如操作body等tag

BOM：浏览器对象模型，可以获得浏览器的信息，如运行在什么平台，Mac or Win等

我们可以在浏览器的console中运行代码，查看效果

标签被称为“元素节点”，标签内的文本形成“文本节点”，为了html书写视觉体验，在tag前的换行，也算文本节点

就算是`.html`文档中的注释，也有对应的节点，`<!doctpye>`也有对应的节点

## 遍历dom

https://zh.javascript.info/dom-navigation

利用元素的“导航属性”，洋文叫做`navigation`

```js
    for (let i = 0; i < document.body.childNodes.length; i++) {
      alert( document.body.childNodes[i] ); // Text, DIV, Text, UL, ..., SCRIPT
    }
```

`childNodes`返回的是一个“类数组的可迭代对象”，不是数组，不过可以使用Array.from转换成数组

> 在 `childNodes` 中我们可以看到文本节点，元素节点，甚至包括注释节点（如果它们存在的话）。

使用这些`node`命名的获取node的属性，可能会获取“注释”等node，我们可能更加关心`element`，也就是标签对应的“元素节点”。

`parentNode`和`parentElement`区别在于，`<html>`，前者返回`document`，后者返回`null`

只需要使用`children`，就能拿到element

```js
for (let elem of document.body.children) {
    alert(elem); // DIV, UL, DIV, SCRIPT
}
```

### table

https://zh.javascript.info/dom-navigation#dom-navigation-tables

table有自己独有的导航属性，如`.rows`，`.cells`

```html
<script>
  // 获取带有 "two" 的 td（第一行，第二列）
  let td = table.rows[0].cells[1];
  td.style.backgroundColor = "red"; // highlight it
</script>
```

## 搜索元素

总结的太好了

| 方法名                   | 搜索方式     | 可以在元素上调用？ | 实时的？ |
| ------------------------ | ------------ | ------------------ | -------- |
| `querySelector`          | CSS-selector | ✔                  | -        |
| `querySelectorAll`       | CSS-selector | ✔                  | -        |
| `getElementById`         | `id`         | -                  | -        |
| `getElementsByName`      | `name`       | -                  | ✔        |
| `getElementsByTagName`   | tag or `'*'` | ✔                  | ✔        |
| `getElementsByClassName` | class        | ✔                  | ✔        |

# dom节点类

EventTarget, Node, Element, Document都是用来描述Dom的class

Node也有自己的属性和方法

- nodeName和tagName
- innerHTML: 属性允许将元素中的 HTML 获取为字符串形式。

```html
<body>
  <p>A paragraph</p>
  <div>A div</div>

  <script>
    alert( document.body.innerHTML ); // 读取当前内容
    document.body.innerHTML = 'The new BODY!'; // 替换它
  </script>

</body>
```

在浏览器中的console试一下，整个页面都变了。直接改变了文档的结构和内容

- outerHTML

尝试写入outerHTML只会修改document结构，不会对原有的对象进行改变

## dom属性

```html
<body id="body" type="...">
  <input id="input" type="text">
  <script>
    alert(input.type); // text
    alert(body.type); // undefined：DOM 属性没有被创建，因为它不是一个标准的特性
  </script>
</body>
```

> 对于元素节点，大多数标准的 HTML 特性（attributes）会自动变成 DOM 对象的属性（properties）

得符合“标准”，才能成为属性，也就是能够通过`.`访问到

但是对于这些特性，我们也有自己的访问方法，如`elem.hasAttribute(name)`

## dataset

我们可以定义自己的非标准的特性，然后获取这个元素，根据非标准特性的值，来确定对这个元素的操作

这样的弊端是，自己的非标准特性以后可能会成为标准。

> **所有以 “data-” 开头的特性均被保留供程序员使用。它们可在 `dataset` 属性中使用。**

```html
<body data-about="Elephants">
<script>
  alert(document.body.dataset.about); // Elephants
</script>
```

# 插入新元素

```html
<style>
.alert {
  padding: 15px;
  border: 1px solid #d6e9c6;
  border-radius: 4px;
  color: #3c763d;
  background-color: #dff0d8;
}
</style>

<script>
  let div = document.createElement('div');
  div.className = "alert";
  div.innerHTML = "<strong>Hi there!</strong> You've read an important message.";

  document.body.append(div);
</script>
```

1. 构造元素
2. 指定属性
3. 插入

插入的方法有很多，如`append`，`prepend`，下面是各种方法插入的例子：

```sh
before
1. prepend
2. 0
3. 1
4. 2
5. append
after
```

这就是构造新元素进行插入

尝试修改innerHTML，`<`会被转义，所以不行，可以使用`elem.insertAdjacentHTML(where, html)`。

# 修改元素所属的类

https://zh.javascript.info/styles-and-classes

## classList

`classList` 的方法：

- `elem.classList.add/remove(class)` —— 添加/移除类。
- `elem.classList.toggle(class)` —— 如果类不存在就添加类，存在就移除它。
- `elem.classList.contains(class)` —— 检查给定类，返回 `true/false`。

通过classList，修改一个元素对应的类

## style

元素的style也被映射成对象

```
background-color  => elem.style.backgroundColor
z-index           => elem.style.zIndex
border-left-width => elem.style.borderLeftWidth
```

想要以字符串的形式设置完整的样式，可以使用特殊属性 `style.cssText`

# Window



```js
alert( window.innerWidth ); // 整个窗口的宽度
alert( document.documentElement.clientWidth ); // 减去滚动条宽度后的窗口宽度
```



由于历史原因，获取文档的高度比较烦：

```js
let scrollHeight = Math.max(
  document.body.scrollHeight, document.documentElement.scrollHeight,
  document.body.offsetHeight, document.documentElement.offsetHeight,
  document.body.clientHeight, document.documentElement.clientHeight
);

alert('Full document height, with scrolled out part: ' + scrollHeight);
```

滚动：

- `scrollBy(x,y)`：相对当前位置滚动
- `scrollTo(pageX, pageY)`: 滚动到绝对左边
- `elem.scrollToView(top)`：top为true，elem与窗口的顶部/底部对齐

# 坐标

相对浏览器窗口定位和相对页面定位

- 相对于窗口：`position: fixed`，`clientX/clientY`
- 相对于文档：`position: absolute`，`pageX/pageY`

获取一个元素相对浏览器`client`的坐标：`elem.getBoundingClientRect()`

```js
> elem.getBoundingClientRect()
< DOMRect {x: 0, y: 0, width: 100, height: 100, top: 0, …}
bottom: 100
height: 100
left: 0
right: 100
top: 0
width: 100
x: 0
y: 0
[[Prototype]]: DOMRect
```

经常通过`create`创建元素，然后给元素添加样式，最后通过这个定位显示元素的位置

# 事件

为tag添加`on<event>`绑定事件，event有：click、focus等

例子：`<input value="Click me" *onclick**="alert('Click!')"* type="button">`

事件函数中的this是这个dom元素

## addEventListener

需求，点击时候，调用两个函数

可以为元素添加监听事件

```html
<input id="elem" type="button" value="Click me"/>

<script>
  function handler1() {
    alert('Thanks!');
  };

  function handler2() {
    alert('Thanks again!');
  }

  elem.onclick = () => alert("Hello");
  elem.addEventListener("click", handler1); // Thanks!
  elem.addEventListener("click", handler2); // Thanks again!
</script>
```

如果要移除处理程序，使用`removeEventListener`。

## 对象处理事件

```html
<button id="elem">Click me</button>

<script>
  class Menu {
    handleEvent(event) {
      // mousedown -> onMousedown
      let method = 'on' + event.type[0].toUpperCase() + event.type.slice(1);
      this[method](event);
    }

    onMousedown() {
      elem.innerHTML = "Mouse button pressed";
    }

    onMouseup() {
      elem.innerHTML += "...and released.";
    }
  }

  let menu = new Menu();
  elem.addEventListener('mousedown', menu);
  elem.addEventListener('mouseup', menu);
</script>
```

# 冒泡和捕获

event.target可以获取哪个元素触发了

event.currentTarget是当前触发元素的this

除非真的需要，否则没必要阻止冒泡

捕获：点击父元素，事件往下传递到底的过程

> 事件首先通过祖先链向下到达元素（捕获阶段），然后到达目标（目标阶段），最后上升（冒泡阶段），在途中调用处理程序。

这么说来，一个元素会被一个事件“穿透”两次。

既可以在捕获的时候调用函数，也可以在冒泡的时候调用函数

# 事件委托

https://zh.javascript.info/event-delegation

需求：

一个父元素，里面有很多子元素，每个子元素都需要监听事件，那么可以直接由父元素监听事件

子元素通过属性，标记自己需要哪一种action

当事件发生的时候，由父组件通过标记的action，决定执行哪一种逻辑

这些的前提就是冒泡，所以没真的需求，不要阻止冒泡

# 浏览器默认行为

浏览器有默认行为：如点击`<a>`，就打开标签页

如何阻止？

```html
<a href="/" onclick="return false">Click here</a>
or
<a href="/" onclick="event.preventDefault()">here</a>
```

只要`onEvent()`的最后return了false，就可以不执行默认行为

有一个易错点，

`<a href="/" onclick="return false">Click here</a>` 是work的，但是

`<a href="/" onclick="FucntionReturnFalse()">Click here</a>`这样不行，必须显式地 `return false`

# 冒泡时候检查是否处理

一般情况下，不阻止冒泡。

那么如何告诉后面的元素，某个浏览器默认的事件已经被处理过了呢？

可以检查`event.defaultPrevented`，如果这个事件的默认行为已经被阻止，那么这个值为真，

后续冒泡到的元素检查到这个值，就可以直接return。

# 创建自定义事件

观察下面的代码，`new Event`创建对象，

`let event = new Event(type, bubbles=false, cancelable=false)`

`dispatchEvent()`相当于在元素上分配事件

```js
// 按键精灵，每 2s 创建一个点击事件，并分配到 button 上
handleClick = () => {
    alert("u just click");
}
let button = document.querySelector("button");
setInterval(() => {
    let event = new Event("click");
    button.dispatchEvent(event);
}, 2000);
```



对于自定义事件，我们应该使用 `CustomEvent` 构造器。它有一个名为 `detail` 的附加选项，我们应该将事件特定的数据分配给它。然后，所有处理程序可以以 `event.detail` 的形式来访问它。

# UI 事件

## 鼠标事件

事件触发是有顺序的，如 `mousedown mouseup click`

组合键，event发生的时候，会通过属性记录有哪些按键被触发，如 `event.shiftKey` 等

onCopy可以阻止用户的复制

如果不想用户在双击的时候选中文本，可以令`onmousedown return false`

### 移动鼠标

mouseover/mouseout，event具有 relatedTarget 属性

注意这个事件会冒泡

## 进入/离开元素

mouseenter 和 mouseleave

这俩事件不会冒泡

## 键盘事件

> 当一个按键被按下时，会触发 `keydown` 事件，而当按键被释放时，会触发 `keyup` 事件。

event.key: 不同语言返回不同

event.code: 不同布局返回相同，只和物理位置有关

## 滚动

可以监听滚动：

`addEventListener("scroll", fn)`

我认为重点是各种坐标，如

`window.innerHeight`

`elem.getBoundingClientRect()`

## 表单

focus 和 blur

数据更新时候，触发 change

对输入值进行修改后，就会触发 `input` 事件。

oncopy 和 onpaste 可以监听复制和粘贴事件

`form.submit()`用于手动提交表单

# HTML 加载

https://zh.javascript.info/onload-ondomcontentloaded

生命周期：

- `DOMContentLoaded`: 生成了 DOM 树，不过可能没有图片
- `Load`: 加载完成，包括图片、外部样式等
- `beforeunload/unload`: 当用户正在离开页面时

> 当浏览器处理一个 HTML 文档，并在文档中遇到 `<script>` 标签时，就会在继续构建 DOM 之前运行它。

> 如果访问者触发了离开页面的导航（navigation）或试图关闭窗口，`beforeunload` 处理程序将要求进行更多确认。

> `document.readyState` 是文档的当前状态，可以在 `readystatechange`事件中跟踪状态更改：
>
> - `loading` —— 文档正在被加载。
> - `interactive` —— 文档已被解析完成，与 `DOMContentLoaded` 几乎同时发生，但是在 `DOMContentLoaded` 之前发生。
> - `complete` —— 文档和资源均已加载完成，与 `window.onload` 几乎同时发生，但是在 `window.onload` 之前发生。

## 脚本

当浏览器加载 HTML 时遇到 `<script>...</script>` 标签，浏览器就不能继续构建 DOM。它必须立刻执行此脚本。

对于外部脚本 `<script src="..."></script>` 也是一样的：浏览器必须等脚本下载完，并执行结束，之后才能继续处理剩余的页面。

`defer` 特性告诉浏览器不要等待脚本。相反，浏览器将继续处理 HTML，构建 DOM。脚本会“在后台”下载，然后等 DOM 构建完成后，脚本才会执行。

**具有 `defer` 特性的脚本保持其相对顺序，就像常规脚本一样。**

defer 会确保浏览器脚本的执行顺序，async 不会。

## 动态添加脚本

create `<script>`，然后 append 到文档里面，这样的脚本是 async

但是这是可以通过 `script.async = false` 设置的。

## 脚本也有事件

onload 和 onerror

```js
let script = document.createElement('script');

// 可以从任意域（domain），加载任意脚本
script.src = "https://cdnjs.cloudflare.com/ajax/libs/lodash.js/4.3.0/lodash.js"
document.head.append(script);

script.onload = function() {
  // 该脚本创建了一个变量 "_"
  alert( _.VERSION ); // 显示库的版本
};
script.onerror = function() {
  alert("Error loading " + this.src); // Error loading https://example.com/404.js
};
```

# 杂项

## mutation

https://zh.javascript.info/mutation-observer

可以通过 mutation 监控dom 上元素的变化

比如动态加载并渲染

比如移除外部脚本可能加载的小广告等

## 选择 selection

太多api了

## 事件循环

https://zh.javascript.info/event-loop

> **事件循环** 的概念非常简单。它是一个在 JavaScript 引擎等待任务，执行任务和进入休眠状态等待更多任务这几个状态之间转换的无限循环。

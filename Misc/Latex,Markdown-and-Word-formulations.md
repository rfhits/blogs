Latex、Markdown 和 word 公式

Datetime: 2023-04-23T15:34+08:00

Categories: Misc

阅读的最好方式是使用 `Ctrl/Command + F`，查找自己想要的公式、字符或其他。

比如搜索 `密度` 或 `柔`，就能找到对应的希腊字母 `\rho`，即 $\rho$。

本文最早发布于知乎，后来发现知乎官方不提供导出文章的功能，那么，再见了，知乎。

[toc]

# 在 word 中插入 Latex 公式

我劝别，最典型的反面教材是，好好的公式，如 `\Omega`，利用 word 自带的 Unicode Profession 转换成 Latex Linear 再转换为 Profession，就变了！？

<iframe src="https://player.bilibili.com/player.html?aid=570340828&bvid=BV1Sv4y1E79U&cid=1106624859&page=1&high_quality=1&danmaku=0&autoplay=0" allowfullscreen="allowfullscreen" width="100%" height="500" scrolling="no" frameborder="0" sandbox="allow-top-navigation allow-same-origin allow-forms allow-scripts"></iframe>

老老实实 Unicode 手动输入吧，本来是没有备份公式的需求的，主要是那些字体什么的。

这或许，就是 Microsoft 的屎山吧。。。

# 使用知乎探索公式编辑

可以使用知乎来查找自己想要的 Latex 符号对应的文本，步骤如下：

在知乎上新建一篇文章

![](https://picx.zhimg.com/v2-63ba4c50752d7474a0636f5611a63d45_720w.jpg?source=d16d100b)

然后插入公式

![](https://pic1.zhimg.com/v2-c9a62dfdc0d8ea4a41a28a9d3f258863_720w.jpg?source=d16d100b)

选中以后就是自己要的 plain text 了

![](https://picx.zhimg.com/v2-2eb2a1f6009a7344be5d2b89e6d35e8e_720w.jpg?source=d16d100b)

# 希腊字母

$\rho$ `\rho` 柔，密度

$\xi$ `\xi` 中值定理，ξ，可系

# 抽象

$\rightarrow$ `\rightarrow` 右箭头

$\Rightarrow$ `\Rightarrow` 右双箭头，大箭头

$\leftarrow$ `\leftarrow` 左箭头

$\Leftrightarrow$ `\Leftrightarrow` 双向箭头

$\tilde{x}$ `\tilde{x}` 头上的波浪线

$\langle \rangle$ `\langle \rangle` 尖括号、元组。是 left/right + angle 的缩写

# 集合

$\cap$ `\cap` 交

$\cup$ `\cup` 并

$\emptyset$ `\emptyset` 空集

$\subseteq$ `\subseteq` 包含，子集

$\subsetneqq$ `\subsetneqq` 真包含，真子集

$\subset$ `\subset` 很难说属于哪一类，参考 [子集 - wikipedia](https://zh.wikipedia.org/wiki/%E5%AD%90%E9%9B%86)

# 几何

$\cong$ `\cong` 全等

$\bot$ `$\bot$` 垂直

# 数理逻辑

$\forall$ `\forall` 任意

$\exists$ `\exists` 存在

# 运算符

$\oplus$ `\oplus` 模加法，圆圈加法

$\otimes$ `\otimes` 圆圈乘法，圆圈

$\prod_{A}^{B}$ `\prod_{A}^{B}` 连乘，product

$\times$ `\times` 叉乘，乘号，不要因为点乘是 cdot，就以为叉乘是啥 dot

$\sum$ `\sum` 求和

$\pm$ `\pm` 正负号

$<$ `<` `\lt` 小于，less than

$>$ `>` `\gt` 大于，greater than

$\le$ `\le` 小于等于，less or equal

$\ge$ `\ge` 大于等于，greater or equal

$$
\lim_{a \rightarrow b}{x}
$$

`$$\lim_{a \rightarrow b}{x}$$` 极限，注意是`$$...$$`，四个 `$`，内联写法`$...$`将导致「趋于」在右下角而非正下方，如 $\lim_{a \rightarrow b}{x}$

$\infty$ `\infty` 无限

$\approx$ `\approx` 约等于

$\sqrt[a]{b}$ `\sqrt[a]{b}`方根

$\vert x \vert$ $\lvert a \rvert$ `\vert a \vert` `\lvert a \rvert` 绝对值，带 l 和 r 无法被 word 识别。

---

$\lVert \vec{x} \rVert$，$\Vert x \Vert$，$\parallel x \parallel$

`\lVert \vec{x} \rVert` `\parallel x \parallel` 向量长度、范数，`parallel` 和 `Vert` 可以用在 word，带 l 和 r 无法被 word 识别。

word 中应该使用 `\norm` 表示 $||$

# 字体

参考 [字母的各种字体及 latex 表示方法](https://blog.csdn.net/wzz110011/article/details/124753129)

$\mathcal{A}$ `\mathcal{A}` calligraphy 艺术体，书法，手写（我看起来挺像的），集合，set

$\mathcal{A} \mathcal{B} \mathcal{C} \mathcal{D} \mathcal{E} \mathcal{F} \mathcal{G} \mathcal{H} \mathcal{I} \mathcal{J} \mathcal{K} \mathcal{L} \mathcal{M} \mathcal{N} \mathcal{O} \mathcal{P} \mathcal{Q} \mathcal{R} \mathcal{S} \mathcal{T} \mathcal{U} \mathcal{V} \mathcal{W} \mathcal{X} \mathcal{Y} \mathcal{Z}$

$\mathcal{a} \mathcal{b} \mathcal{c} \mathcal{d} \mathcal{e} \mathcal{f} \mathcal{g} \mathcal{h} \mathcal{i} \mathcal{j} \mathcal{k} \mathcal{l} \mathcal{m} \mathcal{n} \mathcal{o} \mathcal{p} \mathcal{q} \mathcal{r} \mathcal{s} \mathcal{t} \mathcal{u} \mathcal{v} \mathcal{w} \mathcal{x} \mathcal{y} \mathcal{z}$

$\mathscr{A}$ `\mathscr{A}` script，花体，手写，艺术字，这个小写好像和其他一样

$\mathscr{A} \mathscr{B} \mathscr{C} \mathscr{D} \mathscr{E} \mathscr{F} \mathscr{G} \mathscr{H} \mathscr{I} \mathscr{J} \mathscr{K} \mathscr{L} \mathscr{M} \mathscr{N} \mathscr{O} \mathscr{P} \mathscr{Q} \mathscr{R} \mathscr{S} \mathscr{T} \mathscr{U} \mathscr{V} \mathscr{W} \mathscr{X} \mathscr{Y} \mathscr{Z}$

$\mathscr{a} \mathscr{b} \mathscr{c} \mathscr{d} \mathscr{e} \mathscr{f} \mathscr{g} \mathscr{h} \mathscr{i} \mathscr{j} \mathscr{k} \mathscr{l} \mathscr{m} \mathscr{n} \mathscr{o} \mathscr{p} \mathscr{q} \mathscr{r} \mathscr{s} \mathscr{t} \mathscr{u} \mathscr{v} \mathscr{w} \mathscr{x} \mathscr{y} \mathscr{z}$

$\mathbb{A}$ `\mathbb{A}` blackboard 空心，黑板体，实数集合，自然数集合，正整数集合

$\mathbb{A} \mathbb{B} \mathbb{C} \mathbb{D} \mathbb{E} \mathbb{F} \mathbb{G} \mathbb{H} \mathbb{I} \mathbb{J} \mathbb{K} \mathbb{L} \mathbb{M} \mathbb{N} \mathbb{O} \mathbb{P} \mathbb{Q} \mathbb{R} \mathbb{S} \mathbb{T} \mathbb{U} \mathbb{V} \mathbb{W} \mathbb{X} \mathbb{Y} \mathbb{Z}$

$\mathbb{a} \mathbb{b} \mathbb{c} \mathbb{d} \mathbb{e} \mathbb{f} \mathbb{g} \mathbb{h} \mathbb{i} \mathbb{j} \mathbb{k} \mathbb{l} \mathbb{m} \mathbb{n} \mathbb{o} \mathbb{p} \mathbb{q} \mathbb{r} \mathbb{s} \mathbb{t} \mathbb{u} \mathbb{v} \mathbb{w} \mathbb{x} \mathbb{y} \mathbb{z}$

word 中，只有 `Ctrl + B/I` 来调整已经输入的字体，或者就使用 Latex 先输入 `\mathcal`，再转 Unicode。

---- 遇到写不出来的就更新，虽然写完毕设大概率这辈子就不会碰了 ----

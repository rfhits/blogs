[toc]

---

+ flex
  + 内部元素间距：justify-content
  + [一排的最后一个div占据剩下所有空间：flex-grow:1](https://www.cnblogs.com/yingshiyv/p/14008913.html)
+ text
  + div 中字体左右居中：text-align: center
  + [单行文本溢出](https://blog.csdn.net/Shivy_/article/details/121789793)
  + [多行文本溢出](https://zhuanlan.zhihu.com/p/91499900)
  + [px em rem](https://www.zhihu.com/question/270742667/answer/2416384694)
  + [垂直居中](http://www.divcss5.com/css3-style/c57297.shtml)
    + line-height 和父元素同高
+ div
  + [圆形：border-radius: 50%](https://blog.csdn.net/weixin_43883917/article/details/109662077)
  + border
    + [边框颜色 border-style:solid; border-color: red](https://m.php.cn/article/485702.html)
    + [边框阴影](https://www.cnblogs.com/xuedong09/p/3158644.html)
+ canvas
  + [画图](https://www.jianshu.com/p/254a5fa6c03b)
+ position
  + [知乎教学视频](https://www.zhihu.com/question/19926700/answer/2083545234)
  + absolute: 相对于整个页面定位，需要配合left和top使用，表示和页面的间距
  + relative: 相对于自己原来的位置定位，原来的位置还是会占据空间的
  + 父元素 relative，然后啥都不加；子元素 absolute ，就能实现子相对于父的定位
+ 组件/常用操作
  + 多个div水平排列，超出屏幕宽度，可以水平滑动
+ 背景字体配色
  + iPhone 卡片的背景颜色：#f2f1f6
  + iPhone 分割线：#c6c6c6
  + 携程价格蓝色：#368aee
  + 携程时间橙色：#ff7f00
  + 马蜂窝字体选择蓝：#4884f6
  + 携程出发地目的地连线灰：#dddddd
  + 携程连线上的字的灰：#a7a7a7

# text

## 单行文本溢出隐藏

一行文字太长，用省略号隐藏

```css
 .content {
    width: 300px;
    border: 1px solid red;
    /* 强制不换行 */
    white-space: nowrap;
    /* 文字用省略号代替超出的部分 */
    text-overflow: ellipsis;
    /* 匀速溢出内容，隐藏 */
    overflow: hidden;   
  }
```

# 组件/常用操作

## 多个div超出宽度，水平滑动

最外层：`overflow: hidden;white-space: nowrap;overflow-x:auto;`

里面的多个div：`display: inline-block;`

```html
 <!-- tags -->
<view
class="tags"
style="margin-left: 10rpx;overflow: hidden;white-space: nowrap;overflow-x:auto;"
>
<van-tag
    wx:for="{{ question.tags }}"
    bindtap='onTapTag'
    data-content='{{item}}'
    type="primary"
    size="large"
    color="#f6f6f6"
    text-color="#808080"
    wx:key="index"
    style="margin-left: 10rpx; display: inline-block;"
>{{
    item
}}</van-tag>
</view>
```

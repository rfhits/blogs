JavaScript 常用操作

在我写这篇文章时，我还是个小白

所以记录**我认为**常用的语法

-   数组
    -   尾部插入：push
    -   已知 value 获取 index
    -   [删除元素](https://www.cnblogs.com/shangrao/p/12832572.html)
        -   索引删除：`splice(index,len,[item])` ：index:数组开始下标；len: 替换/删除的长度；item:替换的值，删除操作的话 item 为空
    -   数组合并：`arr1.push.apply(arr1,arr2)`
-   字符串
    -   子串：`substr(start, length)`: 指定 start 获取 length 个字符
    -   trim(): 返回删除两边空白的新的字串，
    -   字符串是否包含：.indexOf(subStr)，返回第一个匹配的位置，如果不包含，则 -1
    -   时间字符串的比较 `localeCompare()`，比如 `'23:59:00'.localeCompare('00:01:01')`
-   数字
    -   正整数：`/(^[1-9]\d*$)/`
    -   金额的正则：`(^[1-9]([0-9]{0,15})(\.[0-9]{1,2})?$)|(^[0-9]{1}(\.[0-9]{1,2})?$)`
-   date
    -   `new Date(str)`: str 格式比较随意
    -   获取毫秒数：`date.getTime()`
    -   获取时间差的天、时、分：% / 24 60 60 1000
-   null 和 undefined：链接跳转时候，被转化成了字符串，用 === 判断
-   异步函数
    -   [for 循环中发送请求](https://blog.csdn.net/baidu_41651062/article/details/119255762)
-   Promise
    -   [Promise.all 用法](https://blog.csdn.net/Yetian_2000/article/details/112019727)
-   索引查找：`findIndex()` 方法返回传入一个测试条件（函数）符合条件的数组第一个元素位置。

```js
let index = array.findIndex((item) => item.id == "3")
console.info(index) //返回值 2

// https://blog.csdn.net/u012320487/article/details/123840469
```

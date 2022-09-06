## 起因

本来是root里有很多个card组件，然后，root的height设置成100%，结果没法包住所有的card。

关键是不把height设置成100%，搜索结果，也就是card过少的时候，页面的底部又会有空白，这时候又需要100%。

## 解决

将cards放到一个子div中，设置好width 100%，然后设置card们居中，这样再设置div的background-color就能覆盖掉白色了。
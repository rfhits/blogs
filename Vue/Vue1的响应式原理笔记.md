参考：

https://www.bilibili.com/video/BV1G54y1s7xV

https://juejin.cn/post/6932659815424458760

# 递归向下创建ob

Object.defineProperty方法，可以自定义get和set

一个对象：

```json
{
    a: {
     
	},
    b: 5
}
```

如果子属性a是对象，则`a.__ob__= new Observer`，并递归，每一个是对象的属性，都要有`__ob__`

observe(obj):

1. 若obj上已经有`__ob__`则结束
2. 调用`ob = Observer(obj)`，结束

Observer(obj):

1. 为obj上创建`__ob__`
2. 对obj.keys，defineReactive(obj, key)

defineReactive(obj, key):

1. oberve(obj.key)，为子元素分配ob
2. 利用set和get“代理”属性的读取和修改

## 改写array

新弄一个proto，改写数组的7个方法，再把这个proto的`__ptoro__`设置成Array.prototype

当我们在上一步对着子属性walk的时候，如果遇到了一个数组，就在为其设置ob的时候，把array的`__proto__`改成我们自己定义的新的prototype，这样就能拦截那七个方法

# 依赖收集

https://juejin.cn/post/6932659815424458760

每一个obs上，都有一个dep（dependence）对象，dep是订阅发布者中的发布者，内有subs数组，收集订阅者

watcher用来读取数据，然后触发getter，被dep收集，

Watcher需求：

`watcher(obj, 'p.q.r', callbackFn)`

监听obj的属性，当变化的时候，调用callback

`watcher.update()`

watch 读取数据的时候，把自己挂载到全局对象，可以是window上的target上去

读取数据，自然触发obj.get，此时Dep就能通过window获取到watcher
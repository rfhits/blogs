
出问题的代码，

```js
    path: '/user/:username/',
    name: 'User',
    component: User,
    redirect: '/user/:username/posts'
```

输入`user/100`后，跳转到了`user/:username/posts`，100没有传递给重新向的路由。


解决方法：

```js
    path: '/user/:username/',
    name: 'User',
    component: User,
    redirect: to => {
      const {hash, params, query} = to
      if (params.username) {
        return '/user/'+params.username+'/posts'
      }
    },
```

具体内容，见vue文档和示例：

[重定向和别名](https://router.vuejs.org/zh/guide/essentials/redirect-and-alias.html#%E9%87%8D%E5%AE%9A%E5%90%91)

[高级用法](https://github.com/vuejs/vue-router/blob/dev/examples/redirect/app.js)

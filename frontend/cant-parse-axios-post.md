# 后端无法解析 axios 发送的请求

-   created: 2021-09-21 22:59
-   published: 2021-09-21 22:59
-   modified: 2024-12-24T19:09+08:00
-   categories: frontend
-   tags: bug

前端通过 `axios.post(url, data)` 发送的 `data` 参数如果是一个对象（JS Object）的话，  
axios 会修改 ContentType 为 UTF-8。

```javascript
if (utils.isObject(data)) {
    setContentTypeIfUnset(headers, 'application/json;charset=utf-8');
    return JSON.stringify(data)
}
```

所以前端实际上发了个字符串到后端。

所以后端调用`request.POST.get(key)`是得不到东西的，因为 `request.POST` 不是一个对象（字典），没有什么 key，value 的概念。

要将字符串重新转为对象，调用 json 包就行：

```python
data = json.loads(request.body)
data.get('xxx')
```

这样就能获取到前端传递的对象了。

参考：

-   [源码分析](https://blog.csdn.net/csdn_yudong/article/details/79668655)
-   [解决方法](https://www.cnblogs.com/hooo-1102/p/12090527.html)

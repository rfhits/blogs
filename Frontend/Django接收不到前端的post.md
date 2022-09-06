前端通过post发送的请求如果是一个对象的话，axios会修改ContentType，

![image](https://img2020.cnblogs.com/blog/1930568/202109/1930568-20210921225209897-225736537.png)

所以发了个字符串到后端。

所以调用`request.POST.get("key")`是得不到东西的，因为`request.POST`不是一个对象（字典），没有什么key，value的概念。

所以要将字符串重新转为对象，调用json包就行：

```python
data = json.loads(request.body)
data.get('xxx')
```

这样就能获取到value了。

感谢大佬的源码分析：
https://blog.csdn.net/csdn_yudong/article/details/79668655

感谢大佬的解决方法：
https://www.cnblogs.com/hooo-1102/p/12090527.html
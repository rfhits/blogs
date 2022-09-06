> Tomcat 是 Servlet 的容器
> ——佚名

+ 初识 Tomcat
+ 在 IDEA 中开发 Tomcat 项目
+ Servlet

# 初识 Tomcat

就是下载了 Tomcat ，然后 start 一下，就可以访问 8080 端口。  
这时，不考虑背后的逻辑，我们可以使用 Tomcat 编写简单的静态网页。

# Maven, IDEA 和 Tomcat

[IDEA中的Maven操作-睿站-狂神说](https://www.bilibili.com/video/BV12J411M7Sj?p=6)

在 IDEA 中新建一个Maven项目，套用 Apache 的 webapp 模板，然后可以打成 war 包。

参考: [jar包和war包的介绍和区别](https://www.jianshu.com/p/3b5c45e8e5bd)

原来 Tomcat 可以自动识别 war 包，这样方便了我们的开发，不用在下载的 Tomcat 的文件目录中做开发。

# Servlet

`Servlet` 只是一个接口而已，抽象类 `GenericServlet` 实现了这个接口

```java
public interface Servlet {
    void init(ServletConfig var1) throws ServletException;
    ServletConfig getServletConfig();
    void service(ServletRequest var1, ServletResponse var2) throws ServletException, IOException;
    String getServletInfo();
    void destroy();
}

public abstract class GenericServlet implements Servlet, ServletConfig, Serializable{}

public abstract class HttpServlet extends GenericServlet{}
```
我们可以编写自己的 HttpServlet 子类，重写其中的 `doGet`, `doPut`, `doPost` 等方法，然后在`Maven` 中的 `pom.xml` 中，将自己编写的 Servlet 和 url 映射（Mapping）起来。

然后，添加 Tomcat ，配置路径，就能够在页面上看到我们的 Servlet 的效果了，详见[HelloServlet-睿站-狂神说](https://www.bilibili.com/video/BV12J411M7Sj?p=8)

# 关于 Tomcat 和 Vue 之间的思考

前端总是绕不开这几个要素：

+ 路由：某个 url 请求到了哪个 html
+ 交互逻辑：点击按钮，出发的一系列 http request
+ 数据保存：比如社交网站在用户登录后要保存用户的用户名等数据

路由，在 Vue 中有 router，在 Tomcat 中用 .xml

交互逻辑，在Vue中有 js/ts，在 Tomcat 中用 jsp
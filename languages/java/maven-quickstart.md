# maven 快速上手

-   created: 2025-01-14T11:30+08:00
-   published: 2025-02-06T17:20+08:00
-   category: java

[Maven Getting Started Guide – Maven](https://maven.apache.org/guides/getting-started/index.html)

核心是 pom.xml，需要加依赖直接在 dependency 里面加就好了

打包命令：

```shell
maven package
```

这是根据 pom 中的 build.plugins 来决定怎么打包。
默认打出来的 jar 需要依赖库才可以运行。
如果想要把动态库一起打包进去，就需要加插件。

运行 jar：

```shell
java -jar {path_to_jar}
```

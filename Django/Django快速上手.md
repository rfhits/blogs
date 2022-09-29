Django快速上手

[toc]

# 创建 project 和 app

django 将整个项目视作一个 project，每个 project 是一个容器，这个容器里面可以有很多的 app。

django 的一大设计理念是“可拔插”，app 可以直接迁移到别的 project，app 的配置文件也可以迁移到别的 app

首先通过`django-admin startproject <project_name>`创建一个项目，然后我们cd到这个 project 里，项目结构如下：

```bash
|-- <project_name>
|   |-- __init__.py
|   |-- asgi.py
|   |-- settings.py
|   |-- urls.py
|   `-- wsgi.py
`-- manage.py
```

这就是一个最简单的项目，可以看作一个容器，要放置自己的app

接下来通过 `django startapp <app_name>`创建一个app，整个文件夹结构如下：

```bash
.
|-- <project_name>
|   |-- __init__.py
|   |-- asgi.py
|   |-- settings.py
|   |-- urls.py
|   `-- wsgi.py
|-- <app_name>
|   |-- __init__.py
|   |-- admin.py
|   |-- apps.py
|   |-- migrations
|   |   `-- __init__.py
|   |-- models.py
|   |-- tests.py
|   `-- views.py
`-- manage.py

```

可以看到项目里就有两个文件夹，一个是和项目同名的文件夹，一个是创建的app文件夹

```bash
<project_name>
|-- settings.py # 项目的全局配置文件
|-- urls.py # 访问后端资源时候，先到这个文件里找路由
```

# 创建自己的 Model

我们在Django中，可以直接通过写 python 而不是写 sql 来操作数据库，这是因为 Django 在 python 对象和数据库表之间提供了一层抽象。

我们可以专注于ER图，通过OO的思想，ER图中的每一个实体（entity），就是一个对象，在 django 中叫做 `Model`，继承自 `Models.model`。





---- 未完待续 ----

# Python module 的相对导入

python module relative import

[toc]

Created: 2024-09-22T11:16+08:00
Published: 2024-09-22T13:20+08:00

Category: Python

第一次编写 Python package 时候，很可能会需要不同文件夹下的 python 文件相互 import。
如果不了解 Python 的 relative import 机制，很可能会导致如下两种错误：

```shell
ModuleNotFoundError: No module named 'xxx'
ImportError: attempted relative import with no known parent package
```

本文通过一个 petstore 例子，告诉大家如何写 package。

有四个需求：

1. Cat 和 Dog 两个类，继承自 Pet 类
2. 我们需要在 `main.py` 中自定义类继承 `Pet`
3. 我们需要在 `main.py` 中使用 `Cat` 类
4. cat.py 里面有一个 `if __name__ == "__main__`，我们可以直接执行 `cat.py`

# 项目结构

```shell
├─main.py
│
└─petstore
    │  __init__.py
    │
    ├─pet
    │  │  cat.py
    │  │  dog.py
    │  │  pet.py
    │  └─ __init__.py
    │
    └─store
           __init__.py
```

## main.py

```python
# main.py

print("we are running: ", __name__)

import sys

from petstore.pet import Pet
from petstore.pet.cat import Cat


class Bird(Pet):
    def __init__(self) -> None:
        super().__init__()

    def eat(self):
        print("bird eat")


if __name__ == "__main__":
    c = Cat()
    c.eat()

    b = Bird()
    b.eat()
```

## pet.py

```Python
# pet.py

import abc


class Pet(abc.ABC):
    def __init__(self) -> None:
        pass

    @abc.abstractmethod
    def eat(self):
        pass

```

## cat.py

```python
# cat.py
print("importing cat")
print("cat.py.__name__:", __name__)
print("done\n")

from .pet import Pet # <- attention here!!!


class Cat(Pet):
    def __init__(self) -> None:
        pass

    def eat(self):
        print("cat eat")


if __name__ == "__main__":
    c = Cat()
    c.eat()

```

## dog.py

```python
print("importing dog")
print("dog.py.__name__:", __name__)
print("done\n")

from .pet import Pet


class Dog(Pet):
    def __init__(self) -> None:
        pass

    def eat(self):
        print("dog eat")

```

## `pet/__init__.py`

```python
"""
这个包处理宠物管理功能。
"""

from .pet import Pet
from .cat import Cat
from .dog import Dog

__all__ = ['Pet', 'Cat', 'Dog']
```

# `ModuleNotFoundError: No module named 'pet'`

如果将 `from .pet import Pet` 写作 `from pet import Pet`，
并且直接在项目 根目录 `python ./main.py`，就会报这个错。

因为 python 文件直接作为命令行参数执行的时候，
解释器就认为其是一个 module 的 root[^1]，**没有传递任何关于 petstore 的 package structure**
当执行到 `cat.py` 中的 `from pet import Pet` 时候，
就会从 sys.path 中查找 `pet`，目前的路径是根目录，没有 pet，所以找不到就报错。

## solution 1

如果这时候，我们在 `main.py` 中加上：

```python
import sys
sys.path.append("./petstore/pet")
```

就可以让 `cat.py` 中 `from pet import pet` 生效。

## solution 2

直接使用 sys.path 毕竟不好，将路径 hard encode 到了文件中，
更好的写法是写作 `from .pet import Pet`
这样 `python main.py` 时候，可以正确解析 petstore package 的结构。

# `ImportError: attempted relative import with no known parent package`

这个错误发生在使用了 relative import 后，
直接运行使用 relative import 的 python 文件。
如 `python .\petstore\pet\cat.py`

```shell
python .\petstore\pet\cat.py
importing cat
cat.py.**name**: **main**
done

Traceback (most recent call last):
File "~\petstore\pet\cat.py", line 5, in <module>
from .pet import Pet
ImportError: attempted relative import with no known parent package
```

因为直接运行的文件将视作 package 的 root，所以没法 relative import。

## solution

想要直接运行 `cat.py`，可以使用命令 `python -m petstore.pet.cat`

关键是参数 `-m`

```shell
> python -m petstore.pet.cat

importing cat
cat.py.__name__: petstore.pet.cat
done

importing dog
dog.py.__name__: petstore.pet.dog
done

<frozen runpy>:128: RuntimeWarning: 'petstore.pet.cat' found in sys.modules after import of package 'petstore.pet', but prior to execution of 'petstore.pet.cat'; this may result in unpredictable behaviour
importing cat
cat.py.__name__: __main__
done

cat eat
```

可以看到 `petstore.pet.cat` 这个 module 被 解析了两次，第一次其 `__name__` 为 `petstore.pet.cat`，第二次为 `__main__`。

总之，不应该在 module 内部运行 script[^2]。

# 总结

那么正确的做法应该是什么呢？

在同一个 package 中，只使用 relative import  
要测试 package 中的 module，在外部进行测试，module 内不要有 `if __name__ == "__main__"`

复述下需求的解决方法：

```shell
├─main.py
│
└─petstore
    │  __init__.py
    │
    ├─pet
    │  │  cat.py
    │  │  dog.py
    │  │  pet.py
    │  └─ __init__.py
    │
    └─store
           __init__.py
```

1. Cat 和 Dog 两个类，继承自 Pet 类
   使用 `from .pet import Pet`
2. 我们需要在 `main.py` 中自定义类继承 `Pet`
   将 petstore 视作一个 package，直接 `from petstore.pet import Pet`
3. 我们需要在 `main.py` 中使用 `Cat` 类
   将 petstore 视作一个 package，直接 `from petstore.pet.cat import Cat`
4. cat.py 里面有一个 `if __name__ == "__main__`，我们可以直接执行 `cat.py`
   因为 `cat.py` 中已经使用了 relative import 所以使用命令 `python -m petstore.pet.cat` 来运行

[^1]: https://stackoverflow.com/a/73149/20752995
[^2]: https://stackoverflow.com/a/8195271/20752995

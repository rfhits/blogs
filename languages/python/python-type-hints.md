# 在 Python 中使用 Type Hints

-   created: 2024-10-21T16:48+08:00
-   published: 2024-10-22T13:08+08:00
-   categories: python

在 vscode 中 settings 搜索 Pylance 的 `Type Checking Mode` 即可开启

## 多态可以使用 `TypeVar`

工厂方法使用 TypeVar 作为函数参数的类型注释，可以做到传入什么类型，返回什么类型。

```python
from typing import Type, TypeVar


class Pet:
    def __init__(self):
        pass

    def set_name(self, name):
        self.name = name


class Dog(Pet):
    def __init__(self):
        super().__init__()

    def bark(self):
        echo("woof")


class Cat(Pet):
    def __init__(self):
        super().__init__()

    def meow(self):
        echo("meow")


PetClass = TypeVar("PetClass", bound=Pet)

def create_pet(pet_class: Type[PetClass]) -> PetClass:
    p = pet_class()
    p.set_name("foo")
    return pet_class()


if __name__ == "__main__":
    d = create_pet(Dog)
    d.bark()
    c = create_pet(Cat)
    c.meow()
```

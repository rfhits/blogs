print("importing cat")
print("cat.py.__name__:", __name__)
print("done\n")

from .pet import Pet
from ..store.desk import Desk


class Cat(Pet):
    def __init__(self) -> None:
        pass

    def eat(self):
        print("cat eat")

    def __str__(self):
        return "cat"


if __name__ == "__main__":
    c = Cat()
    c.eat()
    d = Desk()

    d.put_on(c)

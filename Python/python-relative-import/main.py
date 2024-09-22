print("we are running: ", __name__)

from petstore.pet import Pet
from petstore.pet.dog import Dog
from petstore.pet.cat import Cat


class Bird(Pet):
    def __init__(self) -> None:
        super().__init__()

    def eat(self):
        print("bird eat")


if __name__ == "__main__":
    d = Dog()
    d.eat()

    c = Cat()
    c.eat()

    b = Bird()
    b.eat()

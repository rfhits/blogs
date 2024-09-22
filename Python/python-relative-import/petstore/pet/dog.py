print("importing dog")
print("dog.py.__name__:", __name__)
print("done\n")


from .pet import Pet
# from petstore.pet import Pet


class Dog(Pet):
    def __init__(self) -> None:
        pass

    def eat(self):
        print("dog eat")



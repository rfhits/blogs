import abc


class Pet(abc.ABC):
    def __init__(self) -> None:
        pass

    @abc.abstractmethod
    def eat(self):
        pass

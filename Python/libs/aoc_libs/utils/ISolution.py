from abc import abstractmethod, ABC


class ISolution(ABC):
    @abstractmethod
    def Part1(self):
        pass

    @abstractmethod
    def Part2(self):
        pass

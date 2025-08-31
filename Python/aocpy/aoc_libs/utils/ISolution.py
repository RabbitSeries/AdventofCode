from abc import abstractmethod, ABC
from io import TextIOWrapper


class ISolution(ABC):
    AOC_FINISH_LINE = "\t\t\t------All 50 stars AoC Finished!!!!------"

    @abstractmethod
    def Part1(self, f: TextIOWrapper):
        pass

    @abstractmethod
    def Part2(self, f: TextIOWrapper):
        pass

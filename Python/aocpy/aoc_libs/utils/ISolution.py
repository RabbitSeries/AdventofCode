from abc import abstractmethod, ABC
from typing import IO


class ISolution(ABC):
    AOC_FINISH_LINE = "\t\t\t------All 50 stars AoC Finished!!!!------"

    @abstractmethod
    def Part1(self, f: IO[str]):
        pass

    @abstractmethod
    def Part2(self, f: IO[str]):
        pass

from abc import abstractmethod, ABC


class ISolution(ABC):
    AOC_FINISH_LINE = "\t\t\t------All 50 stars AoC Finished!!!!------"
    @abstractmethod
    def Part1(self):
        pass

    @abstractmethod
    def Part2(self):
        pass

from functools import cache, reduce
import math

from aocpy.aoc_libs.utils.ISolution import ISolution


class Monkey:
    def __init__(self, operation: str, nanominator: int, goOnTrue: int, goOnFalse: int) -> None:
        self.operation, self.goOnTrue, self.goOnFalse = operation, goOnTrue, goOnFalse
        self.devisible = nanominator

    def __repr__(self) -> str:
        return f"Monkey('{self.operation}',{self.devisible},{self.goOnTrue},{self.goOnFalse})"

    def getNew(self, old: int):
        return eval(self.operation)


class MonkeyInTheMiddle(ISolution):

    @cache
    def Play(self, item: int, mkId: int, devider: int = 3):
        delta = [0] * len(self.Monkeys)
        while True:
            mk = self.Monkeys[mkId]
            delta[mkId] += 1
            item = (mk.getNew(item) // devider) % self.mod
            nextId = mk.goOnTrue if item % mk.devisible == 0 else mk.goOnFalse
            if nextId <= mkId:
                break
            else:
                mkId = nextId
        return (delta, (item, nextId))

    def Round(self, round: int, devider: int):
        resList = [0] * len(self.Monkeys)
        items = self.items
        for _ in range(round):
            nextItems = []
            for item, mkId in items:
                delta, nextItem = self.Play(item, mkId, devider)
                nextItems.append(nextItem)
                resList[:] = [a + b for a, b in zip(resList, delta)]
            items = nextItems
        return resList

    def Part1(self, f):
        raw = [data.splitlines() for data in f.read().split('\n\n')]
        self.Monkeys = tuple((Monkey(operation.split("=")[-1],
                                     int(devider.split()[-1]),
                                     int(goOnTrue.split()[-1]),
                                     int(goOnFalse.split()[-1]))
                              for _, _, operation, devider, goOnTrue, goOnFalse
                              in raw))
        self.items = [(item, int(mkId.split(':')[0].split()[1]))
                      for mkId, items, *_ in raw for item in map(int, items.split(":")[1].split(","))]
        self.mod = math.lcm(*[mk.devisible for mk in self.Monkeys])
        print("Part 1:", reduce(lambda i, n: i * n, sorted(self.Round(20, 3), reverse=True)[0:2], 1))

    def Part2(self, f):
        print("Part 2:", reduce(lambda i, n: i * n, sorted(self.Round(10000, 1), reverse=True)[0:2], 1))


if __name__ == "__main__":
    solution = MonkeyInTheMiddle()
    with open("Day11/input.txt") as f:
        solution.Part1(f)
    with open("Day11/input.txt") as f:
        solution.Part2(f)

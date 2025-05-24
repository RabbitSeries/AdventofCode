from functools import reduce, lru_cache
import math


class Monkey:
    def __init__(self, operation: str, nanominator: int, goOnTrue: int, goOnFalse: int) -> None:
        self.operation, self.goOnTrue, self.goOnFalse = operation, goOnTrue, goOnFalse
        self.devisible = nanominator

    def __repr__(self) -> str:
        return f"Monkey('{self.operation}',{self.devisible},{self.goOnTrue},{self.goOnFalse})"

    def getNew(self, old: int):
        return eval(self.operation)


@lru_cache(maxsize=None)
def Play(item: int, mkId: int, Monkeys: tuple[Monkey, ...], modulo: int, devider: int = 3):
    delta = [0] * len(Monkeys)
    while True:
        mk = Monkeys[mkId]
        delta[mkId] += 1
        item = (mk.getNew(item) // devider) % modulo
        nextId = mk.goOnTrue if item % mk.devisible == 0 else mk.goOnFalse
        if nextId <= mkId:
            break
        else:
            mkId = nextId
    return (delta, (item, nextId))


def Round(round: int, items: list[tuple[int, int]], modulo: int, Monkeys: tuple[Monkey, ...], devider: int):
    resList = [0] * len(Monkeys)
    for _ in range(round):
        nextItems = []
        for item, mkId in items:
            delta, nextItem = Play(item, mkId, Monkeys, modulo, devider)
            nextItems.append(nextItem)
            resList[:] = [a + b for a, b in zip(resList, delta)]
        items = nextItems
    return resList


def main():
    with open("input.txt") as f:
        raw = [data.splitlines() for data in f.read().split('\n\n')]
    Monkeys = tuple((Monkey(operation.split("=")[-1],
                            int(devider.split()[-1]),
                            int(goOnTrue.split()[-1]),
                            int(goOnFalse.split()[-1]))
                     for _, _, operation, devider, goOnTrue, goOnFalse
                     in raw))
    items = [(item, int(mkId.split(':')[0].split()[1]))
             for mkId, items, *_ in raw for item in map(int, items.split(":")[1].split(","))]
    mod = math.lcm(*[mk.devisible for mk in Monkeys])
    print("Part 1:", reduce(lambda i, n: i * n, sorted(Round(20, items, mod, Monkeys, 3), reverse=True)[0:2], 1))
    print("Part 2:", reduce(lambda i, n: i * n, sorted(Round(10000, items, mod, Monkeys, 1), reverse=True)[0:2], 1))


if __name__ == '__main__':
    main()

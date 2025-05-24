import copy
from functools import reduce


class Monkey:
    def __init__(self, operation: str,
                 nanominator: int,
                 goOnTrue: int,
                 goOnFalse: int) -> None:
        (self.operation, self.goOnTrue, self.goOnFalse) = (
            operation, goOnTrue, goOnFalse)
        self.devisible = nanominator

    def __repr__(self) -> str:
        return f"Monkey('{self.operation}',{self.devisible},{self.goOnTrue},{self.goOnFalse})"

    def getNew(self, old: int):
        return eval(self.operation)


def Play(item: int, mkId: int, Monkeys: list[Monkey], modulo: int, devider: int = 3):
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


def gcd(a, b):
    return a if b == 0 else gcd(b, a % b)


def lcm(a, b):
    return a * b // gcd(a, b)


def Round(round: int, resList: list[int], items: list[tuple[int, int]], modulo: int, Monkeys: list[Monkey], devider: int):
    for r in range(round):
        nextItems = []
        for item, mkId in items:
            delta, nextItem = Play(item, mkId, Monkeys, modulo, devider)
            nextItems.append(nextItem)
            for i, dlt in enumerate(delta):
                resList[i] += dlt
        items = nextItems


def main():
    with open("input.txt") as f:
        MksRaw = [data.splitlines() for data in f.read().split('\n\n')]
        Monkeys = [Monkey(operation.split("=")[-1],
                          int(devider.split()[-1]),
                          int(goOnTrue.split()[-1]),
                          int(goOnFalse.split()[-1]))
                   for _, _, operation, devider, goOnTrue, goOnFalse
                   in MksRaw]
        items = [(item, int(mkId.split(':')[0].split()[1]))
                 for mkId, items, _, _, _, _ in MksRaw for item in map(int, items.split(":")[1].split(","))]
        modulo = reduce(lambda i, mk: lcm(i, mk.devisible), Monkeys, 1)
        resList = [0] * len(Monkeys)
        Round(20, resList, copy.deepcopy(items), modulo, Monkeys, 3)
        print("Part 1:", reduce(lambda i, n: i * n, sorted(resList, reverse=True)[0:2], 1))
        resList = [0] * len(Monkeys)
        Round(10000, resList, items, modulo, Monkeys, 1)
        print("Part 2:", reduce(lambda i, n: i * n, sorted(resList, reverse=True)[0:2], 1))


if __name__ == '__main__':
    main()

from collections import deque
from functools import cache
import re
from aoc_libs.utils.ISolution import ISolution
import math
import numpy as np
from queue import PriorityQueue


class BluePrint:
    robots = ["ore", "clay", "obsidian", "geode"]

    def __init__(self, data: str):
        self.prototypes = tuple(tuple(int(m.group(1)) if (m := re.search(fr"(\d+) {r_type}", d)) else 0 for r_type in self.robots)
                                for d in data.split(":")[1].strip().split(".") if d)
        self.max = tuple(map(int, np.max(self.prototypes, axis=0)))

    def __repr__(self) -> str:
        return ". ".join([f"Each {rbt} robot costs " + ' and '.join([f'{c} {rbt}' for c in cost if c != 0])
                          for rbt, cost in zip(self.robots, self.prototypes)]) + "."

    def __hash__(self) -> int:
        return hash(self.prototypes)

    def __eq__(self, value: object) -> bool:
        if not isinstance(value, BluePrint):
            return False
        return id(self) == id(value) or self.prototypes == value.prototypes


class NotEnoughMinerals(ISolution):
    def __init__(self):
        with open("Day19/input.txt") as f:
            self.bluePrints = [BluePrint(line) for line in f.read().splitlines()]

    def __repr__(self) -> str:
        return "\n".join([f"Blueprint {i}: {bp}" for i, bp in enumerate(self.bluePrints, 1)])

    @cache
    def DigDigDig(self, countDown: int, blp: BluePrint,
                  rbt: tuple[int, ...] = (1, 0, 0, 0),
                  minerals: tuple[int, ...] = (0, 0, 0, 0)) -> int:
        if countDown <= 0:
            return minerals[-1]
        maxGeode = minerals[-1] + rbt[-1] * countDown
        for i in reversed(range(4)):
            cost = blp.prototypes[i]
            if i != 3 and rbt[i] >= blp.max[i]:
                continue
            delta = [max(0, c-m) for c, m in zip(cost, minerals)]
            if any(d != 0 and r == 0 for d, r in zip(delta, rbt)):
                continue
            # time required to build the next robot
            time = max(0 if d == 0 else math.ceil(d/r) for d, r in zip(delta, rbt)) + 1
            if countDown > time:
                nextRobots = tuple(r + (1 if j == i else 0) for j, r in enumerate(rbt))
                nextMinerals = tuple(min(r*time+m-c, blpm*(countDown-time)) if blpm != 0 else r*time+m-c
                                     for r, m, c, blpm in zip(rbt, minerals, cost, blp.max))  # Increase cache hit rate
                maxGeode = max(maxGeode, self.DigDigDig(countDown-time, blp, nextRobots, nextMinerals))
        return maxGeode

    def solve(self, countDown: int, last: int | None = None):
        res = []
        for i, blp in enumerate(self.bluePrints[0:last], 1):
            res.append((i, self.DigDigDig(countDown, blp)))
            print(res)
        return res

    def Part1(self):
        print(f'Part 1: {sum(a*b for a, b in self.solve(24))}')

    def Part2(self):
        print(f'Part 2: {math.prod(b for _, b in self.solve(32, 3))}')

    def test(self):
        data = '''Blueprint 1:
  Each ore robot costs 4 ore.
  Each clay robot costs 2 ore.
  Each obsidian robot costs 3 ore and 14 clay.
  Each geode robot costs 2 ore and 7 obsidian.

Blueprint 2:
  Each ore robot costs 2 ore.
  Each clay robot costs 3 ore.
  Each obsidian robot costs 3 ore and 8 clay.
  Each geode robot costs 3 ore and 12 obsidian.'''
        blps = self.bluePrints
        self.bluePrints = [BluePrint(d) for d in data.split("\n\n")]
        self.Part1()
        self.bluePrints = blps


if __name__ == "__main__":
    solution = NotEnoughMinerals()
    # solution.test()
    # This is so f**king slow
    solution.Part1()
    solution.Part2()

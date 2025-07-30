from functools import lru_cache
import re
from aoc_libs.utils.ISolution import ISolution
import math


class BluePrint:
    robots = ["ore", "clay", "obsidian", "geode"]

    def __init__(self, data: str):
        self.prototypes = tuple(tuple(int(m.group(1)) if (m := re.search(fr"(\d+) {r_type}", d)) else 0 for r_type in self.robots)
                                for d in data.split(":")[1].strip().split(".") if d)

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

    @lru_cache(None)
    def DigDigDig(self, countDown: int, blp: BluePrint,
                  rbt: tuple[int, ...] = (1, 0, 0, 0),
                  minerals: tuple[int, ...] = (0, 0, 0, 0)) -> int:
        if countDown <= 0:
            return minerals[-1]
        maxGeode = minerals[-1]
        for i, cost in enumerate(blp.prototypes):
            delta = [max(0, r-m) for r, m in zip(cost, minerals)]
            if any(d != 0 and r == 0 for d, r in zip(delta, rbt)):
                maxGeode = max(maxGeode, minerals[-1] + rbt[-1]*countDown)
                continue
            time = max(max(0, 0 if d == 0 else math.ceil(d/r)) for d, r in zip(delta, rbt))+1
            # time required to build the next robot
            if countDown > time:
                nextRobots = tuple([*rbt[0:i], rbt[i]+1, *rbt[i+1:]])
                nextMinerals = tuple(r*time+m-c for r, m, c in zip(rbt, minerals, cost))
                maxGeode = max(maxGeode, self.DigDigDig(countDown-time, blp, nextRobots, nextMinerals))
            else:
                maxGeode = max(maxGeode, minerals[-1] + rbt[-1]*countDown)
        return maxGeode

    def Part1(self):
        res = []
        for i, blp in enumerate(self.bluePrints, 1):
            res.append((i, self.DigDigDig(24, blp)))
            print(res)
        print(f'Part 1: {sum([i*c for i, c in res])}')

    def Part2(self):
        pass

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
    # solution.Part2()

import re
from typing import Literal
import numpy as np


class BluePrint:
    robots = ["ore", "clay", "obsidian", "geode"]

    def __init__(self, robotDatas: list[str]):
        self.costs = np.zeros((4, 4), dtype=int)
        for cost, robotData in zip(self.costs, robotDatas):
            for j, mineral in enumerate(self.robots):
                pattern = re.compile(Rf"(\d+)(?: {mineral})")
                res = next(pattern.finditer(robotData), None)
                cost[j] = 0 if res is None else int(res.group(1))

    def __repr__(self) -> str:
        return ". ".join([f"Each {robot} robot costs "
                          + ' and '.join([f'{n} {name}' for n, name in zip(cost, self.robots) if n != 0])
                          for robot, cost in zip(self.robots, self.costs)]) + "."


class NotEnoughMinerals:
    def __init__(self):
        with open("Day19/input.txt") as f:
            self.bluePrints = [BluePrint(line.split(":")[1].split(".")) for line in f.read().splitlines()]

    def __repr__(self) -> str:
        return "\n".join([f"Blueprint {i}: {bluePrint}" for i, bluePrint in enumerate(self.bluePrints, 1)])


if __name__ == "__main__":
    print(NotEnoughMinerals())

import re
import numpy as np


class BluePrint:
    robots = ["ore", "clay", "obsidian", "geode"]

    def __init__(self, data: list[str]):
        self.costs = np.array([[int(m.group(1)) if (m := re.search(fr"(\d+) {r_type}", d)) else 0 for r_type in self.robots]
                               for d in data])

    def __repr__(self) -> str:
        return ". ".join([f"Each {rbt} robot costs " + ' and '.join([f'{c} {rbt}' for c in cost if c != 0])
                          for rbt, cost in zip(self.robots, self.costs)]) + "."


class NotEnoughMinerals:
    def __init__(self):
        with open("Day19/input.txt") as f:
            self.bluePrints = [BluePrint(line.split(":")[1].split(".")) for line in f.read().splitlines()]

    def __repr__(self) -> str:
        return "\n".join([f"Blueprint {i}: {bp}" for i, bp in enumerate(self.bluePrints, 1)])


if __name__ == "__main__":
    print(NotEnoughMinerals())

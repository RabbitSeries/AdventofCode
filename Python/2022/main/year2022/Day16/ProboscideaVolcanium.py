from __future__ import annotations
import re
from queue import Queue
# from functools import lru_cache


class ProboscideaVolcanium:
    # This can be developed to a drone exploration algorithm if the map is not known ahead of time
    def __init__(self, limit_time: int):
        self.SPath: dict[str, dict[str, int]] = {}  # Compress the graph
        self.limit = limit_time

    def readFile(self):
        with open("Day16/input.txt") as f:
            input = f.read().splitlines()
            pattern = re.compile(R"Valve (.+) has flow rate=(\d+); .* valve(?:s|)\w? (.+)")
            self.Valves: dict[str, tuple[int, list[str]]] = {valve: (int(rate), tunnels.split(", ")) for valve, rate, tunnels in [
                match.groups() for match in [pattern.match(line) for line in input] if match is not None]}
            self.NonZeroValves = {valve: i for i, valve in enumerate(
                [valve for valve, (rate, _) in self.Valves.items() if rate != 0])}

    def BFS(self, entry: str) -> dict[str, int]:
        cost: dict[str, int] = {}
        visited: set[str] = {entry}
        q: Queue[tuple[str, int]] = Queue()
        q.put((entry, 0))
        while not q.empty():
            curValve, curCost = q.get()
            for nextValve in self.Valves[curValve][1]:
                if nextValve in visited:
                    continue
                visited.add(nextValve)
                if nextValve in self.NonZeroValves:
                    cost.setdefault(nextValve, curCost + 1)
                q.put((nextValve, curCost + 1))
        return cost

    # @lru_cache(maxsize=64)

    def dfs(self, curValve: str, opened: int, time: int, stat: int):
        if time <= 0:
            return stat
        maxStat = stat
        for nextValve, travelCost in self.SPath[curValve].items():
            bitmask = 1 << self.NonZeroValves[nextValve]
            if bitmask & opened == 0:
                nextTime = time - travelCost - 1
                nextStat = stat + self.Valves[nextValve][0] * (nextTime)
                maxStat = max(maxStat, self.dfs(nextValve, opened | bitmask, nextTime, nextStat))
        return maxStat

    def Part1(self):
        self.readFile()
        for NonZeroValve in self.NonZeroValves.keys():
            self.SPath.setdefault(NonZeroValve, self.BFS(NonZeroValve))
        self.SPath.setdefault("AA", self.BFS("AA"))
        print(f"Part 1: {self.dfs("AA", 0, self.limit, 0)}")


if __name__ == "__main__":
    solution = ProboscideaVolcanium(30)
    solution.Part1()

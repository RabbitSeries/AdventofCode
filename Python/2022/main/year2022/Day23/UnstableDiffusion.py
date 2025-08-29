from matplotlib import axis
import numpy
from aoc_libs.utils.ISolution import ISolution


class UnstableDiffusion(ISolution):
    directions = [[(x, sides) if y == 0 else (sides, y) for sides in [-1, 0, 1]]
                  for x, y in zip([-1, 1, 0, 0],
                                  [0, 0, -1, 1])]

    def __init__(self):
        with open("Day23/input.txt") as f:
            data = f.read().splitlines()
        self.elves: list[tuple[int, int]] = []
        for i, line in enumerate(data):
            for j, col in enumerate(line):
                if col == "#":
                    self.elves.append((i, j))

    @staticmethod
    def vecAdd(vL: tuple[int, ...], vR: tuple[int, ...]):
        return tuple(x1+x2 for x1, x2 in zip(vL, vR))

    def has_companion(self, p: tuple[int, int], seats: set[tuple[int, int]], mainDir: int | None = None):
        for sides in self.directions if mainDir is None else [self.directions[mainDir]]:
            for side in sides:
                if self.vecAdd(p, side) in seats:
                    return True
        return False

    def proporse_move(self, p: tuple[int, int], seats: set[tuple[int, int]], priority: int):
        for pDir in range(priority, priority+4):
            if not self.has_companion(p, seats, pDir % 4):
                return self.vecAdd(p, self.directions[pDir % 4][1])
        return None

    def propose(self, priority: int):
        proposes: dict[tuple[int, int], list[int]] = {}
        seats = set(self.elves)
        for i, elf in enumerate(self.elves):
            if self.has_companion(elf, seats) and (move := self.proporse_move(elf, seats, priority)) is not None:
                proposes.update({move: (proposes[move] if move in proposes else []) + [i]})
        filtered: list[tuple[int, tuple[int, int]]] = []
        for k, v in proposes.items():
            if len(v) == 1:
                filtered.append((v[0], k))
        return filtered

    # def printCanvas(self):
    #     minXY = numpy.min(self.elves, axis=0)
    #     maxXY = numpy.max(self.elves, axis=0)
    #     rows, cols = maxXY[0]-minXY[0]+1, maxXY[1]-minXY[1]+1
    #     canvas = [[" " for _ in range(cols)] for _ in range(rows)]
    #     for elf in self.elves:
    #         canvas[elf[0]-minXY[0]][elf[1]-minXY[1]] = "#"
    #     with open("Day23/output.txt", "w") as f:
    #         f.writelines("".join(line)+"\n" for line in canvas)

    def Part1(self):
        for priority in range(10):
            for i, move in self.propose(priority):
                self.elves[i] = move
        minXY, maxXY = numpy.min(self.elves, axis=0), numpy.max(self.elves, axis=0)
        print(f"Part 1: {(maxXY[0]-minXY[0]+1)*(maxXY[1]-minXY[1]+1) - len(set(self.elves))}")

    def Part2(self):
        priority = 10
        while filtered_proposes := self.propose(priority):
            for i, move in filtered_proposes:
                self.elves[i] = move
            priority += 1
        print(f"Part 2: {priority+1}")


if __name__ == "__main__":
    solution = UnstableDiffusion()
    solution.Part1()
    solution.Part2()

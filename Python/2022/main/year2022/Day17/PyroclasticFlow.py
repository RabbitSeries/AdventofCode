from functools import lru_cache

point2D = tuple[int, int]

# TODO This won't solve test case's part2, but solves my input, I think the cache hit part should be more precise
# Maybe create a snapshot of top xx lines and hash it of FLowMap will
# ensure the matching, the key is to find a unique index in 2022 (?) cycles.
# Then continue to find this index in the next 2022 cycle.
# My case has a full line rock trait, so, logically, this will allow further
# cube's falling to be circulated.


class Rock():
    def __init__(self, location: point2D, points: list[point2D]) -> None:
        self.location = location  # bottom left corner
        self.points = points

    rockId = 0
    prototypes: list[list[str]] = [
        ["####"],
        [".#.",
         "###",
         ".#."],
        ["..#",
         "..#",
         "###"],
        ["#",
         "#",
         "#",
         "#"],
        ["##",
         "##"]
    ]

    #  Upside down coordinate
    #  -------> Col <-left ->right
    #  |
    #  |
    #  |
    #  v Row ^ down
    #        |

    def down(self):
        return Rock((self.location[0] - 1, self.location[1]), self.points)

    def left(self):
        return Rock((self.location[0], self.location[1] - 1), self.points)

    def right(self):
        return Rock((self.location[0], self.location[1] + 1), self.points)

    @staticmethod
    @lru_cache()
    def cacheShape(rockId: int) -> list[point2D]:
        points: list[point2D] = []
        shape = Rock.prototypes[rockId]
        for r, line in enumerate(shape[-1::-1]):
            for c, col in enumerate(line):
                if col == "#":
                    points.append((r, c))
        return points

    @staticmethod
    def next(spawn: point2D):  # Rock factory
        res = Rock(spawn, Rock.cacheShape(Rock.rockId))
        Rock.rockId = (Rock.rockId + 1) % 5
        return res

    def locate(self):
        for point in self.points:
            yield (self.location[0] + point[0], self.location[1] + point[1])


class PyroclasticFlow:

    def __init__(self, WIDTH=7):
        with open("Day17/input.txt") as f:
            self.input = f.read().strip()
        self.cmdIdx = 0
        self.WIDTH = WIDTH
        self.FlowMap: list[list[str]] = []

    def nextCmd(self):
        cmd = self.input[self.cmdIdx]
        self.cmdIdx = (self.cmdIdx + 1) % len(self.input)
        return cmd

    def getHeight(self) -> int:
        rawHeight = len(self.FlowMap)
        for row in self.FlowMap[-1::-1]:
            if '#' in row:
                break
            rawHeight = rawHeight - 1
        return rawHeight

    def getSpawn(self):
        topRow = self.getHeight() - 1
        for i in range(4):
            if topRow + 1 + i >= len(self.FlowMap):
                self.FlowMap.append([' '] * self.WIDTH)
        return (topRow + 4, 2)

    def settle(self, rock: Rock):
        for p in rock.locate():
            while p[0] >= len(self.FlowMap):
                self.FlowMap.append([' '] * self.WIDTH)
            self.FlowMap[p[0]][p[1]] = '#'

    def isRock(self, p: point2D):
        return p[0] >= 0 and p[0] < len(self.FlowMap) and p[1] >= 0 and p[1] < self.WIDTH and self.FlowMap[p[0]][p[1]] == '#'

    def blocked(self, rock: Rock):
        for p in rock.locate():
            if p[0] < 0 or p[1] < 0 or p[1] >= self.WIDTH or self.isRock(p):
                return True
        return False

    def simulate(self):
        spwanRock = Rock.next(self.getSpawn())
        while True:
            nextRock = spwanRock.left() if self.nextCmd() == "<" else spwanRock.right()
            if not self.blocked(nextRock):
                spwanRock = nextRock
            down = spwanRock.down()
            if self.blocked(down):
                self.settle(spwanRock)
                return
            spwanRock = down

    # rockid, cmdIdx -> settled, height
    memo: dict[tuple[int, int], tuple[int, int]] = {}

    def solve(self, f: int, to: int):
        settled, cycle, heightPerCycle = 0, 1, 0
        for settled in range(f, to):
            self.simulate()
            curHeight = self.getHeight()
            if ' ' not in self.FlowMap[curHeight - 1]:
                index = (Rock.rockId, self.cmdIdx)
                if index in self.memo:
                    cycleInfo = self.memo[index]
                    cycle, heightPerCycle = settled - cycleInfo[0], curHeight - cycleInfo[1]
                    break
                self.memo.update({index: (settled, curHeight)})
        remainSim = (to - settled - 1) % cycle
        height = self.getHeight() + (heightPerCycle * ((to - settled - 1) // cycle))
        if remainSim != 0:
            self.FlowMap = []
            for settled in range(0, remainSim):
                self.simulate()
            height = height + self.getHeight()
        return height

    def Part1(self):
        print(f"Part 1: {self.solve(0, 2022)}")

    def Part2(self):
        print(f"Part 2: {self.solve(2022, 1000000000000)}")

    def printMap(self):
        with open("Day17/output.txt", "w") as f:
            f.writelines(["".join(line) + "\n" for line in self.FlowMap[-1::-1]])


if __name__ == "__main__":
    solution = PyroclasticFlow()
    solution.Part1()
    solution.Part2()

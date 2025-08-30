from __future__ import annotations
from heapq import heappush, heappop
from time import time
from aoc_libs.utils.ISolution import ISolution


class Step:
    def __init__(self, time: int, pos: tuple[int, int], parent: BlizzardBasin):
        self.time, self.pos, self.parent = time, pos, parent

    def __repr__(self):
        return f"Step({self.time}, {self.pos})"

    def __lt__(self, rhs: Step):
        # if self.time != rhs.time:
        return self.time < rhs.time
        # TODO why this won't reduce search time?
        # return self.ManhattanDistance(self.pos, self.parent.end) < self.ManhattanDistance(rhs.pos, self.parent.end)

    def __eq__(self, rhs: Step):
        return self.time == rhs.time and self.pos == rhs.pos

    def __hash__(self):
        return hash((self.time, self.pos))

    def paveTo(self, pos: tuple[int, int]):
        return Step(self.time+1, pos, self.parent)

    def reaches(self):
        return self.pos == self.parent.end

    # @staticmethod
    # def ManhattanDistance(pFrom: tuple[int, ...], pTo: tuple[int, ...]):
    #     return sum(abs(x1-x2) for x1, x2 in zip(pFrom, pTo))


class SecondStep(Step):
    def __init__(self, time: int, pos: tuple[int, int], parent: BlizzardBasin, rTimes: int = 0):
        super().__init__(time, pos, parent)
        self.rTimes = rTimes

    def __eq__(self, rhs: SecondStep):
        return super().__eq__(rhs) and self.rTimes == rhs.rTimes

    def paveTo(self, pos: tuple[int, int]):
        rTimes = self.rTimes
        if rTimes == 0 and pos == self.parent.end:
            rTimes = 1
        elif rTimes == 1 and pos == self.parent.start:
            rTimes = 2
        return SecondStep(self.time+1, pos, self.parent, rTimes)

    # def __lt__(self, rhs: SecondStep):
    #     if self.time != rhs.time:
    #         return self.time < rhs.time
    #     if self.rTimes != rhs.rTimes:
    #         return self.rTimes > rhs.rTimes
    #     if self.rTimes == 0 or self.rTimes == 2:
    #         return self.ManhattanDistance(self.pos, self.parent.end) < self.ManhattanDistance(rhs.pos, self.parent.end)
    #     else:
    #         return self.ManhattanDistance(self.pos, self.parent.start) < self.ManhattanDistance(rhs.pos, self.parent.start)

    def reaches(self):
        return self.rTimes == 2 and super().reaches()

    def __hash__(self):
        return hash((super().__hash__(), self.rTimes))


class BlizzardBasin(ISolution):
    directions = {
        "<": (0, -1),
        ">": (0, 1),
        "v": (1, 0),
        "^": (-1, 0)
    }

    def __init__(self):
        with open("Day24/input.txt") as f:
            self.valley = f.read().splitlines()
        self.rows, self.cols = len(self.valley), len(self.valley[0])
        self.start, self.end = (0, 1), (self.rows-1, self.cols-2)

    def isValid(self, p: tuple[int, int]):
        return p[0] >= 0 and p[1] >= 0 and p[0] < self.rows and p[1] < self.cols and self.valley[p[0]][p[1]] != "#"

    def modulo(self, p: tuple[int, int]):
        return ((p[0]-1) % (self.rows-2)+1, (p[1]-1) % (self.cols-2)+1)

    def is_safe_at(self, p: tuple[int, int], t: int):
        if p == self.start or p == self.end:
            return True
        for k, v in self.directions.items():
            rvtPos = self.modulo((p[0]-v[0]*t, p[1]-v[1]*t))
            if self.valley[rvtPos[0]][rvtPos[1]] == k:
                return False
        return True

    def escape_blizzards(self, beginStep: Step):
        pq = [beginStep]
        step_time = {beginStep, }
        while len(pq) > 0:
            here = heappop(pq)
            t, pos = here.time, here.pos
            if here.reaches():
                return t
            for direc in self.directions.values():  # move
                nPos = (pos[0]+direc[0], pos[1]+direc[1])
                nStep = here.paveTo(nPos)
                if self.isValid(nPos) and self.is_safe_at(nPos, t+1):
                    if nStep not in step_time:
                        heappush(pq, nStep)
                        step_time.add(nStep)
            waitStep = here.paveTo(pos)
            if self.is_safe_at(pos, t+1) and waitStep not in step_time:
                heappush(pq, waitStep)
                step_time.add(waitStep)

    def Part1(self):
        print(f"Part 1: {self.escape_blizzards(Step(0, self.start, self))}")

    def Part2(self):
        print(f"Part 2: {self.escape_blizzards(SecondStep(0, self.start, self, 0))}")

    # def Play(self):
    #     p, t = self.start, 0
    #     # while instruction := input():
    #     for cmd in "vv ^>>v<^> vv>>>vv":
    #         # cmd = {"w": "^", "s": "v", "a": "<", "d": ">", " ": " "}.get(instruction, None)
    #         if cmd is None:
    #             print("Wrong Instruction")
    #             continue
    #         if cmd != " ":
    #             nPos = (p[0]+self.directions[cmd][0], p[1]+self.directions[cmd][1])
    #             if self.is_safe_at(nPos, t+1):
    #                 p = nPos
    #                 t += 1
    #                 if p == self.end:
    #                     print("Reaching end! I feel invincible")
    #                     break
    #             else:
    #                 print(f"Not safe at attemp {t} {"minutes" if t > 1 else "minute"}")
    #         else:
    #             if self.is_safe_at(p, t+1):
    #                 t += 1
    #             else:
    #                 print(f"Not safe waiting at attemp {t} {"minutes" if t > 1 else "minute"}")


if __name__ == "__main__":
    solution = BlizzardBasin()
    now = time()
    solution.Part1()
    solution.Part2()
    end = time()
    print(f"Used: {end-now}")

    # solution.Play()

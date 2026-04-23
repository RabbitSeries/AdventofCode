from heapq import heappush, heappop
from typing import Callable

from aocpy.aoc_libs.utils.ISolution import ISolution


class HillClimbingAlgorithm(ISolution):
    def Part1(self, f):
        self.AlphaMap = f.read().splitlines()
        self.ROW, self.COL = len(self.AlphaMap), len(self.AlphaMap[0])
        self.s, self.e = self.FindSE()
        print(f'Part 1: {self.Dijkstra(self.s, self.e, self.posEqual, True)}')

    def Part2(self, f):
        print(f'Part 2: {self.Dijkstra(self.e, self.s, self.alphaEqual, False)}')

    def FindSE(self):
        s = e = tuple()
        for row in range(len(self.AlphaMap)):
            for col, alpha in enumerate(self.AlphaMap[row]):
                if alpha == 'S':
                    s = (row, col)
                    self.AlphaMap[row] = self.AlphaMap[row][0:col] + 'a' + self.AlphaMap[row][col + 1:]
                if alpha == 'E':
                    e = (row, col)
                    self.AlphaMap[row] = self.AlphaMap[row][0:col] + 'z' + self.AlphaMap[row][col + 1:]
        return (s, e)

    dx = [0, 0, 1, -1]
    dy = [1, -1, 0, 0]

    def isValid(self, q: tuple[int, int]):
        return q[0] >= 0 and q[1] >= 0 and q[0] < self.ROW and q[1] < self.COL

    def isUpHill(self, p: tuple[int, int], q: tuple[int, int]):
        return ord(self.AlphaMap[q[0]][q[1]]) <= (ord(self.AlphaMap[p[0]][p[1]]) + 1)

    def posEqual(self, p: tuple[int, int], e: tuple[int, int]):
        return p == e

    def alphaEqual(self, p: tuple[int, int], e: tuple[int, int]):
        return self.AlphaMap[p[0]][p[1]] == self.AlphaMap[e[0]][e[1]]

    def Dijkstra(self, s: tuple[int, int], e: tuple[int, int], destChecker: Callable[[tuple[int, int], tuple[int, int]], bool], upHill: bool) -> int:
        res = 0
        pq: list[tuple[int, tuple[int, int]]] = [(0, s)]
        dist = [[-1] * self.COL for _ in range(self.ROW)]
        dist[s[0]][s[1]] = 0
        while len(pq) > 0:
            curDis, p = heappop(pq)
            if dist[p[0]][p[1]] != -1 and curDis > dist[p[0]][p[1]]:
                continue
            if destChecker(p, e):
                res = curDis
                break
            for x, y in zip(self.dx, self.dy):
                q = (p[0] + x, p[1] + y)
                nextDis = curDis + 1
                if self.isValid(q) and (self.isUpHill(p, q) if upHill else self.isUpHill(*[p, q][::-1])):
                    if dist[q[0]][q[1]] == -1 or nextDis < dist[q[0]][q[1]]:
                        dist[q[0]][q[1]] = nextDis
                        heappush(pq, (nextDis, q))
        return res


if __name__ == "__main__":
    solution = HillClimbingAlgorithm()
    with open("Day12/input.txt") as f:
        solution.Part1(f)
    with open("Day12/input.txt") as f:
        solution.Part2(f)

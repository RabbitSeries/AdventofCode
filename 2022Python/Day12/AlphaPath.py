from heapq import heappush, heappop
from typing import Callable

Point2D = tuple[int, int]
with open('input.txt') as f:
    AlphaMap = f.read().splitlines()
    ROW, COL = len(AlphaMap), len(AlphaMap[0])


def FindSE():
    s = e = Point2D()
    for row in range(len(AlphaMap)):
        for col, alpha in enumerate(AlphaMap[row]):
            if alpha == 'S':
                s = (row, col)
                AlphaMap[row] = AlphaMap[row][0:col] + 'a' + AlphaMap[row][col + 1:]
            if alpha == 'E':
                e = (row, col)
                AlphaMap[row] = AlphaMap[row][0:col] + 'z' + AlphaMap[row][col + 1:]
    return (s, e)


dx = [0, 0, 1, -1]
dy = [1, -1, 0, 0]


def isValid(q: Point2D):
    return q[0] >= 0 and q[1] >= 0 and q[0] < ROW and q[1] < COL


def isUpHill(p: Point2D, q: Point2D):
    return ord(AlphaMap[q[0]][q[1]]) <= (ord(AlphaMap[p[0]][p[1]]) + 1)


def posEqual(p: Point2D, e: Point2D):
    return p == e


def alphaEqual(p: Point2D, e: Point2D):
    return AlphaMap[p[0]][p[1]] == AlphaMap[e[0]][e[1]]


def Dijkstra(s: Point2D, e: Point2D, destChecker: Callable[[Point2D, Point2D], bool], upHill: bool) -> int:
    res = 0
    pq: list[tuple[int, Point2D]] = [(0, s)]
    dist = [[-1] * COL for _ in range(ROW)]
    dist[s[0]][s[1]] = 0
    while len(pq) > 0:
        curDis, p = heappop(pq)
        if dist[p[0]][p[1]] != -1 and curDis > dist[p[0]][p[1]]:
            continue
        if destChecker(p, e):
            res = curDis
            break
        for x, y in zip(dx, dy):
            q = (p[0] + x, p[1] + y)
            nextDis = curDis + 1
            if isValid(q) and (isUpHill(p, q) if upHill else isUpHill(*[p, q][::-1])):
                if dist[q[0]][q[1]] == -1 or nextDis < dist[q[0]][q[1]]:
                    dist[q[0]][q[1]] = nextDis
                    heappush(pq, (nextDis, q))
    return res


def main():
    s, e = FindSE()
    print(f'Part 1: {Dijkstra(s, e, posEqual, True)}')
    print(f'Part 2: {Dijkstra(e, s, alphaEqual, False)}')


if __name__ == '__main__':
    main()

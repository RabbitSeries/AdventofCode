from aoc_libs.utils.ISolution import ISolution

from functools import cache
import re


class MonkeyMap(ISolution):
    unit_vec = [(x, y) for x, y in zip([0, 1, 0, -1], [1, 0, -1, 0])]

    def __init__(self) -> None:
        with open("Day22/input.txt") as f:
            mapData, insData = f.read().split("\n\n")
        self.map = mapData.splitlines()
        self.instructions: list[tuple[str, int]] = [(t, int(s)) for t, s in re.findall(R"([A-Z]*)(\d+)", insData)]
        self.row = len(self.map)
        self.start = (0, next(i for i, c in enumerate(self.map[0]) if c != ' '))

    @cache
    def boundVec(self, pos: tuple[int, ...], dir: int):
        bounds = [pos]*2
        for i, d in enumerate([(dir+2) % 4, dir]):
            nPos = self.next(pos, d)
            while nPos[0] >= 0 and nPos[0] < self.row and nPos[1] >= 0 and nPos[1] < len(self.map[nPos[0]]) and self.map[nPos[0]][nPos[1]] != ' ':
                bounds[i] = nPos
                nPos = self.next(nPos, d)
        return tuple(bounds)

    def next(self, pos: tuple[int, ...], dir: int):
        return tuple(c+d for c, d in zip(pos, self.unit_vec[dir]))

    def boundNext(self, pos: tuple[int, ...], dir: int):

        def vecLen(begin: tuple[int, ...], end: tuple[int, ...], unitVec: tuple[int, ...]):
            vector = (r1-r2 for r1, r2 in zip(end, begin))
            return next(crdn//unit for crdn, unit in zip(vector, unitVec) if unit != 0)

        (begin, end) = self.boundVec(pos, dir)
        pos = self.next(pos, dir)
        boundLen, nPosLen = vecLen(begin, end, self.unit_vec[dir]), vecLen(begin, pos, self.unit_vec[dir])
        return begin if nPosLen > boundLen else pos

    def forward(self, pos: tuple[int, ...], dir: int, step: int) -> tuple[int, ...]:
        i = 0
        nPos = self.boundNext(pos, dir)
        while self.map[nPos[0]][nPos[1]] == '.' and i < step:
            pos = nPos
            nPos = self.boundNext(nPos, dir)
            i += 1
        return pos

    def password(self, pos: tuple[int, ...], dir: int):
        # Generator expressions must be parenthesized if not sole argument Pylance
        return sum(p*multiplier for p, multiplier in zip((p+1 for p in pos), [1000, 4])) + dir

    def Part1(self):
        dir = 0
        pos = self.start
        for turn, step in self.instructions:
            if turn == 'R':
                dir = (dir+1) % 4
            elif turn == 'L':
                dir = (dir-1) % 4
            pos = self.forward(pos, dir, step)
        print(f'Part 1: {self.password(pos, dir)}')

    def Part2(self):
        pass


if __name__ == "__main__":
    solution = MonkeyMap()
    solution.Part1()

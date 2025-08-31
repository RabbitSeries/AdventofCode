from itertools import accumulate

from aocpy.aoc_libs.utils.ISolution import ISolution


class TreetopTreeHouse(ISolution):

    dx = [0, 0, 1, -1]
    dy = [1, -1, 0, 0]

    def Part1(self, f):
        lines = f.read().splitlines()
        self.heights = [[int(e) for e in line] for line in lines]
        self.row, self.col = len(self.heights), len(self.heights[0])
        horizonDP = [list(accumulate(h, max)) for h in self.heights]
        horizonDPR = [list(accumulate(h[::-1], max))[::-1] for h in self.heights]
        verticalDP = [list(accumulate([self.heights[i][j] for i in range(self.row)], max)) for j in range(self.col)]
        vertialDPR = [list(accumulate([self.heights[i][j] for i in range(self.row - 1, -1, -1)], max))[::-1]
                      for j in range(self.col)]
        part1 = [(i, j) for i in range(self.row) for j in range(self.col) if i == 0 or j == 0 or (i == self.row - 1) or (j == self.col - 1)
                 or self.heights[i][j] > min(horizonDP[i][j - 1], horizonDPR[i][j + 1], verticalDP[j][i - 1], vertialDPR[j][i + 1])]
        print("Part 1:", len(part1))

    def Part2(self, f):
        part2 = list(accumulate([c for i in range(self.row) for c in [self.count(i, j) for j in range(self.col)]], max))[-1]
        print("Part 2:", part2)

    def count(self, i: int, j: int):
        res = [1, 1, 1, 1]
        for k in range(4):
            n = (i + self.dx[k], j + self.dy[k])
            while n[0] > 0 and n[1] > 0 and n[0] < self.row - 1 and n[1] < self.col - 1:
                if self.heights[n[0]][n[1]] >= self.heights[i][j]:
                    break
                res[k] += 1
                n = (n[0] + self.dx[k], n[1] + self.dy[k])
        return list(accumulate(res, lambda i, e: i * e, initial=1))[-1]


if __name__ == "__main__":
    solution = TreetopTreeHouse()
    with open("Day08/input.txt") as f:
        solution.Part1(f)
    with open("Day08/input.txt") as f:
        solution.Part2(f)

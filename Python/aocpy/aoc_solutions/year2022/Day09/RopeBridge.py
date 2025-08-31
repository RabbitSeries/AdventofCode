from abc import abstractmethod
from aocpy.aoc_libs.utils.ISolution import ISolution


class RopeBridge(ISolution):

    dx = [0, 0, -1, 1]
    dy = [-1, 1, 0, 0]
    char2direc = {'L': 0, 'R': 1, 'U': 2, 'D': 3}

    @staticmethod
    def sign(x: int):
        return 1 if x > 0 else -1 if x < 0 else 0

    def move(self, python: list[tuple[int, int]], dir: int, n: int):
        for _ in range(n):
            python[0] = (python[0][0] + self.dx[dir], python[0][1] + self.dy[dir])
            for j, p in enumerate(python[1:], 1):
                pre = python[j - 1]
                if self.ChebyshevDis(p, pre) > 1:
                    python[j] = (p[0] + self.sign(pre[0] - p[0]), p[1] + self.sign(pre[1] - p[1]))
                else:
                    break  # No futher move required
                if j + 1 == len(python):
                    yield python[-1]

    @staticmethod
    def ChebyshevDis(x: tuple[int, int], y: tuple[int, int]):
        return max(abs(x[0] - y[0]), abs(x[1] - y[1]))

    def solve(self, pythonLen: int):
        python = [(0, 0)] * pythonLen
        visited: set[tuple[int, int]] = {python[-1]}
        for direc, n in self.commands:
            visited = visited.union(visited, list(self.move(python, direc, n)))
        return len(visited)

    def Part1(self, f):
        self.commands = [(self.char2direc.get(direc, 0), int(n))
                         for line in f.read().strip().splitlines()
                         for direc, n in [line.split()]]
        print("Part 1:", self.solve(2))

    def Part2(self, f):
        print("Part 2:", self.solve(10))


if __name__ == "__main__":
    solution = RopeBridge()
    with open("Day09/input.txt") as f:
        solution.Part1(f)
    with open("Day09/input.txt") as f:
        solution.Part2(f)

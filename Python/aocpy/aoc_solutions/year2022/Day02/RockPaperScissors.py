from aocpy.aoc_libs.utils.ISolution import ISolution


class RockPaperScissors(ISolution):
    def Part1(self, f):
        self.pairs = [list(map(lambda x: ord(x[0]) - ord(x[1]), zip(line.strip().split(), 'AX')))
                      for line in f.read().splitlines()]
        self.part1 = self.part2 = 0
        for a, b in self.pairs:
            self.part1 += ((6 if (b - 1) % 3 == a else 3 if b == a else 0) + b + 1)
            self.part2 += (6 + (a + 1) % 3 + 1 if b == 2 else 3 + a + 1 if b == 1 else (a - 1) % 3 + 1)
        print("Part 1:", self.part1)

    def Part2(self, f):
        print("Part 2:", self.part2)


if __name__ == "__main__":
    solution = RockPaperScissors()
    with open("Day02/input.txt") as f:
        solution.Part1(f)
    with open("Day02/input.txt") as f:
        solution.Part2(f)

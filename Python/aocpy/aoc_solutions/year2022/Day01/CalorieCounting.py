from aocpy.aoc_libs.utils.ISolution import ISolution


class CalorieCounting(ISolution):
    def Part1(self, f):
        self.groups = [sum(map(int, linGroup.split())) for linGroup in f.read().split("\n\n")]
        print(f"Part 1: {max(self.groups)}")

    def Part2(self, f):
        print(f"Part 2: {sum(sorted(self.groups, reverse=True)[:3])}")


if __name__ == "__main__":
    solution = CalorieCounting()
    with open("Day01/input.txt") as f:
        solution.Part1(f)
    with open("Day01/input.txt") as f:
        solution.Part2(f)

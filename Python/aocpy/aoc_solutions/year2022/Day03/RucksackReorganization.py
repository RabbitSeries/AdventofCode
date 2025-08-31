from abc import abstractmethod
from aocpy.aoc_libs.utils.ISolution import ISolution


class RucksackReorganization(ISolution):
    @staticmethod
    def priority(c: str) -> int:
        return ord(c) - ord('A') + 27 if c.isupper() else ord(c) - ord('a') + 1

    @staticmethod
    def intersection(iterable: list[str]) -> set[str]:
        return set(iterable[0]).intersection(*iterable[1:])

    def Part1(self, f):
        self.lines = [line.strip() for line in f.read().split()]
        part1 = sum(sum(map(self.priority, {c for c in line.strip()[:len(line) // 2]} & {c for c in line.strip()[len(line) // 2:]}))
                    for line in self.lines)
        print("Part 1:", part1)

    def Part2(self, f):
        part2 = sum(sum(map(self.priority, self.intersection(self.lines[i:i + 3]))) for i in range(0, len(self.lines), 3))
        print("Part 2:", part2)


if __name__ == "__main__":
    solution = RucksackReorganization()
    with open("Day03/input.txt") as f:
        solution.Part1(f)
    with open("Day03/input.txt") as f:
        solution.Part2(f)

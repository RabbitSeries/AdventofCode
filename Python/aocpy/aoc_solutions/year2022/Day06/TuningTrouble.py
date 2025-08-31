from aocpy.aoc_libs.utils.ISolution import ISolution


class TuningTrouble(ISolution):
    def find_next_unique_list(self, unique_len):
        return next((i for i in range(unique_len, len(self.data)) if len(set(self.data[i - unique_len:i])) == unique_len), len(self.data))

    def Part1(self, f):
        self.data = f.read().rstrip()
        print("Part 1:", self.find_next_unique_list(4))

    def Part2(self, f):
        print("Part 2:", self.find_next_unique_list(14))


if __name__ == "__main__":
    solution = TuningTrouble()
    with open("Day06/input.txt") as f:
        solution.Part1(f)
    with open("Day06/input.txt") as f:
        solution.Part2(f)

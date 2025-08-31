import re

from aocpy.aoc_libs.utils.ISolution import ISolution


class SupplyStacks(ISolution):
    def Part1(self, f):
        init, data = [block.splitlines() for block in f.read().split('\n\n')]
        pattern = re.compile(r'\d+')
        self.pipelines: list[list[str]] = [[] for _ in range(1 + (len(init[0]) + 1) // 4)]
        for line in init:
            for id, elem in enumerate([line[i:i + 4].strip(' []') for i in range(0, len(line), 4)], 1):
                if elem:
                    self.pipelines[id] = [elem, *self.pipelines[id]]
        self.data = [list(map(int, pattern.findall(movement))) for movement in data]
        # print("Part 1:", partsolve1(copy.deepcopy(pipelines)))
        print("Part 1:", self.solve([q.copy() for q in self.pipelines], self.data))

    def Part2(self, f):
        print("Part 2:", self.solve(self.pipelines, self.data, False))

    def solve(self, pipelines: list[list[str]], data: list[list[int]], reverse: bool = True) -> str:
        for n, f, t in data:
            strip = len(pipelines[f]) - min(len(pipelines[f]) - 1, n)
            pipelines[t] += pipelines[f][-1:strip - 1:-1] if reverse else pipelines[f][strip:]
            pipelines[f] = pipelines[f][:strip]
        return ''.join(list(pipeline[-1] for pipeline in pipelines if pipeline))


if __name__ == "__main__":
    solution = SupplyStacks()
    with open("Day05/input.txt") as f:
        solution.Part1(f)
    with open("Day05/input.txt") as f:
        solution.Part2(f)

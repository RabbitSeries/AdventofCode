import re


def solve(pipelines: list[list[str]], data: list[list[int]], reverse: bool = True) -> str:
    for n, f, t in data:
        strip = len(pipelines[f]) - min(len(pipelines[f]) - 1, n)
        pipelines[t] += pipelines[f][-1:strip - 1:-1] if reverse else pipelines[f][strip:]
        pipelines[f] = pipelines[f][:strip]
    return ''.join(list(pipeline[-1] for pipeline in pipelines if pipeline))


with open("input.txt") as f:
    init, data = [block.rstrip().splitlines() for block in f.read().split('\n\n')]
pattern = re.compile(r'\d+')
pipelines: list[list[str]] = [[] for _ in range(0, 1 + (len(init[0]) + 1) // 4)]

for line in init:
    for id, elem in enumerate([line[i:i + 4].strip(' []') for i in range(0, len(line), 4)], 1):
        if elem:
            pipelines[id] = [elem] + pipelines[id]
data = [list(map(int, pattern.findall(movement))) for movement in data]

print("Part 1:", solve([q.copy() for q in pipelines], data))  # print("Part 1:", partsolve1(copy.deepcopy(pipelines)))
print("Part 2:", solve(pipelines, data, False))

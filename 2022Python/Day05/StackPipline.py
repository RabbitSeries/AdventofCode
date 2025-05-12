import re
from collections import deque
from typing import *
with open("input.txt") as f:
    pattern = re.compile(r'\d+')
    init, data = [block.rstrip().splitlines() for block in f.read().split('\n\n')]
    pipelines: List[List[str]] = [[] for _ in range(0, 1 + (len(init[0]) + 1) // 4)]

    for line in init:
        for id, elem in enumerate([line[i:i + 4].strip(' []') for i in range(0, len(line), 4)], 1):
            if elem:
                pipelines[id] = [elem] + pipelines[id]
    data: List[List[int]] = [list(map(int, pattern.findall(movement))) for movement in data]


def solve(pipelines: List[List[str]], reverse: bool = True) -> str:
    for n, f, t in data:
        strip = len(pipelines[f]) - min(len(pipelines[f]) - 1, n)
        pipelines[t] += pipelines[f][-1:strip - 1:-1] if reverse else pipelines[f][strip:]
        pipelines[f] = pipelines[f][:strip]
    return ''.join(list(pipeline[-1] for pipeline in pipelines if pipeline))


print("Part 1:", solve([q.copy() for q in pipelines]))  # print("Part 1:", partsolve1(copy.deepcopy(pipelines)))
print("Part 2:", solve(pipelines, False))

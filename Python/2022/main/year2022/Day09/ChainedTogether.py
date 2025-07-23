dx = [0, 0, -1, 1]
dy = [-1, 1, 0, 0]


def sign(x: int):
    return 1 if x > 0 else -1 if x < 0 else 0


def move(python: list[tuple[int, int]], dir: int, n: int):
    for _ in range(n):
        python[0] = (python[0][0] + dx[dir], python[0][1] + dy[dir])
        for j, p in enumerate(python[1:], 1):
            pre = python[j - 1]
            if ChebyshevDis(p, pre) > 1:
                python[j] = (p[0] + sign(pre[0] - p[0]), p[1] + sign(pre[1] - p[1]))
            else:
                break  # No futher move required
            if j + 1 == len(python):
                yield python[-1]


def ChebyshevDis(x: tuple[int, int], y: tuple[int, int]):
    return max(abs(x[0] - y[0]), abs(x[1] - y[1]))


def solve(pythonLen: int, commands: list[tuple[int, int]]):
    python = [(0, 0)] * pythonLen
    visited: set[tuple[int, int]] = {python[-1]}
    for direc, n in commands:
        visited = visited.union(visited, list(move(python, direc, n)))
    return len(visited)


char2direc = {'L': 0, 'R': 1, 'U': 2, 'D': 3}
with open("Day09/input.txt") as f:
    commands = [(char2direc.get(direc, 0), int(n)) for line in f.read().strip().splitlines() for direc, n in [line.split()]]
print("Part 1:", solve(2, commands))
print("Part 2:", solve(10, commands))

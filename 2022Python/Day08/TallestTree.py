from itertools import accumulate

dx = [0, 0, 1, -1]
dy = [1, -1, 0, 0]

with open("input.txt") as f:
    lines = f.read().splitlines()
    heights = [[int(e) for e in line] for line in lines]
    row, col = len(heights), len(heights[0])
    horizonDP = [list(accumulate(l, max)) for l in heights]

    horizonDPR = [list(accumulate(reversed(l), max))[::-1] for l in heights]

    vertialDP = [list(accumulate([heights[i][j] for i in range(row)], max)) for j in range(col)]

    vertialDPR = [list(accumulate(reversed([heights[i][j] for i in range(row)]), max))[::-1] for j in range(col)]

    part1 = [(i, j) for i in range(row)
             for j in range(col) if i == 0 or j == 0 or (i == row - 1) or (j == col - 1)
             or heights[i][j] > min(horizonDP[i][j - 1], horizonDPR[i][j + 1], vertialDP[j][i - 1], vertialDPR[j][i + 1])]
    print("Part 1:", len(part1))

    def count(i: int, j: int):
        res = [1, 1, 1, 1]
        for k in range(4):
            n = (i + dx[k], j + dy[k])
            while n[0] > 0 and n[1] > 0 and n[0] < row - 1 and n[1] < col - 1:
                if heights[n[0]][n[1]] >= heights[i][j]:
                    break
                res[k] += 1
                n = (n[0] + dx[k], n[1] + dy[k])
        return list(accumulate(res, lambda i, e: i * e, initial=1))[-1]

    part2 = list(accumulate([c for i in range(row) for c in [count(i, j) for j in range(col)]], max))[-1]
    print("Part 2:", part2)

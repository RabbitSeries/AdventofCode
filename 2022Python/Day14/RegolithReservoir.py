EMPTY, BLOCK, SAND = 0, 1, 2
sandbox: list[list[int]]
ROW: int
COL: int
count: int


def transform(x: int, y: int, minXY: tuple[int, int]):
    return (x - minXY[0], y - minXY[1])


def printSandbox():
    with open("output.txt", "w") as output:
        output.writelines(["".join(map(lambda cell: " #O"[cell], line)) + "\n"
                           for line in sandbox])


def expandColumn(back: bool):
    global COL
    for line in sandbox[0:-1]:
        line.insert(COL if back else 0, EMPTY)
    sandbox[-1].insert(COL if back else 0, BLOCK)
    COL += 1


def expandRow(CELL: int):
    sandbox.append([CELL] * COL)
    global ROW
    ROW += 1


def sandSimulate(entrence: tuple[int, int], stopAtOverflow: bool = True):
    count = 0
    x, y = entrence
    while sandbox[x][y] == EMPTY:
        if x + 1 < ROW and sandbox[x + 1][y] == EMPTY:
            x = x + 1
            continue
        if x + 1 < ROW and y >= 1 and sandbox[x + 1][y - 1] == EMPTY:
            x, y = x + 1, y - 1
            continue
        if x + 1 < ROW and y + 1 < COL and sandbox[x + 1][y + 1] == EMPTY:
            x, y = x + 1, y + 1
            continue
        if x + 1 < ROW and y + 1 < COL and y >= 1:
            sandbox[x][y] = SAND
            x, y = entrence
            count += 1
        elif stopAtOverflow:
            break
        else:
            if y + 1 == COL:
                expandColumn(True)
            elif y == 0:
                y += 1
                expandColumn(False)
                entrence = (entrence[0], entrence[1] + 1)
    return count


def part1(originXY: tuple[int, int]):
    global count
    count = sandSimulate(originXY)
    print(f'Part 1: {count}')


def part2(originXY: tuple[int, int]):
    global count
    expandRow(EMPTY)
    expandRow(BLOCK)
    count += sandSimulate(originXY, False)
    print(f'Part 2: {count}')
    printSandbox()


def main():
    global sandbox, ROW, COL
    with open("input.txt") as file:
        data = [[(int(x), int(y)) for y, x in [node.strip().split(",") for node in line.split("->")]]
                for line in file.read().splitlines()]
        data.append([(0, 500)])
    minXY = min([x for line in data for x, _ in line]), min([y for line in data for _, y in line])
    maxXY = max([x for line in data for x, _ in line]), max([y for line in data for _, y in line])
    data.pop()
    ROW, COL = (maxXY[0] - minXY[0] + 1, maxXY[1] - minXY[1] + 1)
    sandbox = [[EMPTY] * COL for _ in range(ROW)]
    for line in data[-1::-1]:
        for s, e in list(zip(line[0:-1], line[1:])):
            t_s, t_e = (transform(s[0], s[1], minXY), transform(e[0], e[1], minXY))
            for r in range(min(t_s[0], t_e[0]), max(t_s[0], t_e[0]) + 1):
                for c in range(min(t_s[1], t_e[1]), max(t_s[1], t_e[1]) + 1):
                    sandbox[r][c] = BLOCK

    originXY = transform(0, 500, minXY)
    part1(originXY)
    part2(originXY)


if __name__ == "__main__":
    main()

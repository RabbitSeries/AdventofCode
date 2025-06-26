EMPTY = 0
BLOCK = 1
SAND = 2
sandbox: list[list[int]] = list()
with open("input.txt") as file:
    data = [[(x, y) for node in line.split("->") for y, x in [tuple(map(int, node.strip().split(",")))]]
            for line in file.read().splitlines()]
    data.append([(0, 500)])

minXY = min([x for line in data for x, _ in line]), min([y for line in data for _, y in line])
maxXY = max([x for line in data for x, _ in line]), max([y for line in data for _, y in line])

data.pop()

ROW, COL = (maxXY[0] - minXY[0] + 1, maxXY[1] - minXY[1] + 1)

sandbox = [[EMPTY] * COL for _ in range(ROW)]


def transform(x: int, y: int):
    return (x - minXY[0], y - minXY[1])


def expandSandbox():
    for line in data[-1::-1]:
        for s, e in list(zip(line[0:-1], line[1:])):
            t_s, t_e = (transform(s[0], s[1]), transform(e[0], e[1]))
            for r in range(min(t_s[0], t_e[0]), max(t_s[0], t_e[0]) + 1):
                for c in range(min(t_s[1], t_e[1]), max(t_s[1], t_e[1]) + 1):
                    sandbox[r][c] = BLOCK


def printSandbox():
    with open("output.txt", "w") as output:
        output.writelines(["".join(map(lambda cell:
                                       " " if cell == EMPTY else
                                       "#" if cell == BLOCK else "O", line)) + "\n"
                           for line in sandbox])


expandSandbox()
# printSandbox()
originXY = transform(0, 500)


def expandFrontColumn():
    for line in sandbox[0:-1]:
        line.insert(0, EMPTY)
    sandbox[-1].insert(0, BLOCK)
    global COL
    COL += 1


def expandBackColumn():
    for line in sandbox[0:-1]:
        line.append(EMPTY)
    sandbox[-1].append(BLOCK)
    global COL
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
            x = x + 1
            y = y - 1
            continue
        if x + 1 < ROW and y + 1 < COL and sandbox[x + 1][y + 1] == EMPTY:
            x = x + 1
            y = y + 1
            continue
        if x + 1 < ROW and y + 1 < COL and y >= 1:
            sandbox[x][y] = SAND
            x, y = entrence
            count += 1
        elif stopAtOverflow:
            break
        else:
            if y + 1 >= COL:
                expandBackColumn()
            elif y < 1:
                y += 1
                expandFrontColumn()
                entrence = (entrence[0], entrence[1] + 1)
    return count


count = sandSimulate(originXY)
print(f'Part 1: {count}')
expandRow(EMPTY)
expandRow(BLOCK)
count += sandSimulate(originXY, False)
print(f'Part 2: {count}')
printSandbox()

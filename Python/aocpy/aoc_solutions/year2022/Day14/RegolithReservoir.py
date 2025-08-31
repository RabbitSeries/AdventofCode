from aocpy.aoc_libs.utils.ISolution import ISolution


class RegolithReservoir(ISolution):

    @staticmethod
    def transform(x: int, y: int, minXY: tuple[int, int]):
        return (x - minXY[0], y - minXY[1])

    # def printSandbox(self):
    #     with open("Day14/output.log", "w") as output:
    #         output.writelines(["".join(map(lambda cell: " #O"[cell], line)) + "\n"
    #                            for line in self.sandbox])

    def expandColumn(self, back: bool):
        for line in self.sandbox[0:-1]:
            line.insert(self.COL if back else 0, self.EMPTY)
        self.sandbox[-1].insert(self.COL if back else 0, self.BLOCK)
        self.COL += 1

    def expandRow(self, CELL: int):
        self.sandbox.append([CELL] * self.COL)
        self.ROW += 1

    def sandSimulate(self, entrence: tuple[int, int], stopAtOverflow: bool = True):
        count = 0
        x, y = entrence
        while self.sandbox[x][y] == self.EMPTY:
            if x + 1 < self.ROW and self.sandbox[x + 1][y] == self.EMPTY:
                x = x + 1
                continue
            if x + 1 < self.ROW and y >= 1 and self.sandbox[x + 1][y - 1] == self.EMPTY:
                x, y = x + 1, y - 1
                continue
            if x + 1 < self.ROW and y + 1 < self.COL and self.sandbox[x + 1][y + 1] == self.EMPTY:
                x, y = x + 1, y + 1
                continue
            if x + 1 < self.ROW and y + 1 < self.COL and y >= 1:  # Can't be settled
                self.sandbox[x][y] = self.SAND
                x, y = entrence
                count += 1
            elif stopAtOverflow:
                break
            else:
                if y + 1 == self.COL:
                    self.expandColumn(True)
                elif y == 0:
                    y += 1
                    self.expandColumn(False)
                    entrence = (entrence[0], entrence[1] + 1)
        return count

    EMPTY, BLOCK, SAND = 0, 1, 2

    def Part1(self, f):
        self.count = 0
        data = [[(int(x), int(y)) for y, x in [node.strip().split(",") for node in line.split("->")]]
                for line in f.read().splitlines()]
        data.append([(0, 500)])
        minXY = min([x for line in data for x, _ in line]), min([y for line in data for _, y in line])
        maxXY = max([x for line in data for x, _ in line]), max([y for line in data for _, y in line])
        data.pop()
        self.ROW, self.COL = (maxXY[0] - minXY[0] + 1, maxXY[1] - minXY[1] + 1)
        self.sandbox = [[self.EMPTY] * self.COL for _ in range(self.ROW)]
        for line in data[-1::-1]:
            for s, e in list(zip(line[0:-1], line[1:])):
                t_s, t_e = (self.transform(s[0], s[1], minXY), self.transform(e[0], e[1], minXY))
                for r in range(min(t_s[0], t_e[0]), max(t_s[0], t_e[0]) + 1):
                    for c in range(min(t_s[1], t_e[1]), max(t_s[1], t_e[1]) + 1):
                        self.sandbox[r][c] = self.BLOCK

        self.originXY = self.transform(0, 500, minXY)
        # self.printSandbox()

        self.count = self.sandSimulate(self.originXY)
        print(f'Part 1: {self.count}')

    def Part2(self, f):
        self.expandRow(self.EMPTY)
        self.expandRow(self.BLOCK)
        self.count += self.sandSimulate(self.originXY, False)
        print(f'Part 2: {self.count}')


if __name__ == "__main__":
    solution = RegolithReservoir()
    with open("Day14/input.txt") as f:
        solution.Part1(f)
    with open("Day14/input.txt") as f:
        solution.Part2(f)

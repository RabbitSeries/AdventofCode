from aocpy.aoc_libs.utils.ISolution import ISolution


class CathodeRayTube(ISolution):
    def Part1(self, f):
        instructions = [cmd for instruction in f.read().splitlines() for cmd in instruction.split()]
        x = 1
        part1 = 0
        self.screen = [[' ' for _ in range(40)] for _ in range(6)]
        SpritePositions = []
        for cycle, cmd in enumerate(instructions, 0):
            self.screen[cycle // 40][cycle % 40] = '# ' if cycle % 40 >= x - 1 and cycle % 40 <= x + 1 else '  '
            part1 += 0 if (cycle + 1 - 20) % 40 else x * (cycle + 1)
            try:
                x += int(cmd)
            except Exception:
                continue
        print('Part 1: {:d}'.format(part1))

    def Part2(self, f):
        print("Part 2:")
        for row in self.screen:
            print("".join(row))


if __name__ == "__main__":
    solution = CathodeRayTube()
    with open("Day10/input.txt") as f:
        solution.Part1(f)
    with open("Day10/input.txt") as f:
        solution.Part2(f)

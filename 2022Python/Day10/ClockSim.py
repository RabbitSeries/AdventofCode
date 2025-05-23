with open("input.txt") as f:
    instructions = [cmd for instruction in f.read().splitlines() for cmd in instruction.split()]
    x = 1
    part1 = 0
    screen = [[' ' for _ in range(40)] for _ in range(6)]
    SpritePositions = []
    for cycle, cmd in enumerate(instructions, 0):
        screen[cycle // 40][cycle % 40] = '# ' if cycle % 40 >= x - 1 and cycle % 40 <= x + 1 else '  '
        part1 += 0 if (cycle + 1 - 20) % 40 else x * (cycle + 1)
        try:
            x += int(cmd)
        except Exception:
            continue
    print('Part 1: {:d}'.format(part1))
    for row in screen:
        print("".join(row))

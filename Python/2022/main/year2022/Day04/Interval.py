with open("Day04/input.txt") as f:
    lines = f.read().splitlines()
intervals = [tuple(map(int, pair.split('-'))) for line in lines for pair in line.split(',')]
intervalPair = list(zip(intervals[::2], intervals[1::2]))
part1 = len([[a, b] for a, b in intervalPair if (a[0] <= b[0] and a[1] >= b[1]) or (a[0] >= b[0] and a[1] <= b[1])])
part2 = len([[a, b] for a, b in intervalPair if max(a[0], b[0]) <= min(a[1], b[1])])
print(f'Part 1: ${part1}')
print(f'Part 2: ${part2}')

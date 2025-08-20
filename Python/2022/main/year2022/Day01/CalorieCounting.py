with open("Day01/input.txt") as f:
    groups = [sum(map(int, linGroup.split())) for linGroup in f.read().split("\n\n")]
print("Part 1:", max(groups))
print("Part 2:", sum(sorted(groups, reverse=True)[:3]))

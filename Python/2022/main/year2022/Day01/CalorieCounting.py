with open("Day01/input.txt") as f:
    groups = [sum(map(int, linGroup.split())) for linGroup in f.read().split("\n\n")]
print(f"Part 1: {max(groups)}")
print(f"Part 2: ${sum(sorted(groups, reverse=True)[:3])}")

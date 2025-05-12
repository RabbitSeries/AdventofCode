part1 = part2 = 0
for line in open("input.txt"):
    a, b = map(lambda x: ord(x[0]) - ord(x[1]), zip(line.strip().split(), 'AX'))
    part1 += ((6 if (b - 1) % 3 == a else 3 if b == a else 0) + b + 1)
    part2 += (6 + (a + 1) % 3 + 1 if b == 2 else 3 + a + 1 if b == 1 else (a - 1) % 3 + 1)
print(part1)
print(part2)

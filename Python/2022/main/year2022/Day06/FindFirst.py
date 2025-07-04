def find_next_unique_list(data, unique_len):
    return next((i for i in range(unique_len, len(data)) if len(set(data[i - unique_len:i])) == unique_len), len(data))


with open("input.txt") as f:
    data = f.read().rstrip()
print("Part 2:", find_next_unique_list(data, 4))
print("Part 2:", find_next_unique_list(data, 14))

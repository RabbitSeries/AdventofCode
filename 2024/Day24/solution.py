from typing import Dict, Tuple, Set, List
from time import perf_counter
from itertools import combinations

def parse_input(input_text: str) -> Tuple[Dict[str, int], Dict[str, Tuple[str, str, str]]]:
    initial_values = {}
    gates = {}
    
    parts = input_text.strip().split('\n\n')
    
    for line in parts[0].splitlines():
        wire, value = line.split(': ')
        initial_values[wire] = int(value)
    
    for line in parts[1].splitlines():
        inputs, output = line.split(' -> ')
        parts = inputs.split()
        if len(parts) == 3:
            gates[output] = (parts[1], parts[0], parts[2])
    
    return initial_values, gates

def evaluate_gate(op: str, a: int, b: int) -> int:
    if op == 'AND':
        return a & b
    elif op == 'OR':
        return a | b
    elif op == 'XOR':
        return a ^ b
    raise ValueError(f"Unknown operation: {op}")

def simulate_circuit(initial_values: Dict[str, int], gates: Dict[str, Tuple[str, str, str]]) -> Dict[str, int]:
    wires = initial_values.copy()
    pending = set(gates.keys())
    
    while pending:
        made_progress = False
        for wire in list(pending):
            op, in1, in2 = gates[wire]
            if in1 in wires and in2 in wires:
                wires[wire] = evaluate_gate(op, wires[in1], wires[in2])
                pending.remove(wire)
                made_progress = True
        if not made_progress and pending:
            return None
    return wires

def get_binary_value(wires: Dict[str, int], prefix: str) -> int:
    bits = sorted([w for w in wires if w.startswith(prefix)])
    value = 0
    for bit in reversed(bits):
        value = (value << 1) | wires[bit]
    return value

def find_swapped_wires(input_text: str, show_progress: bool = False) -> str:
    initial_values, gates = parse_input(input_text)
    
    def find_gate(ta: str, tb: str, top: str) -> str:
        for output, (op, a, b) in gates.items():
            if op == top and ((a == ta and b == tb) or (a == tb and b == ta)):
                return output
        return None
    
    swapped = []
    carry = None
    z_count = max(int(wire[1:]) for wire in gates if wire.startswith('z')) + 1
    
    for i in range(z_count):
        pos = f"{i:02d}"
        sum_1 = find_gate(f"x{pos}", f"y{pos}", "XOR")
        carry_1 = find_gate(f"x{pos}", f"y{pos}", "AND")
        
        if carry is not None:
            carry_2 = find_gate(carry, sum_1, "AND")
            if carry_2 is None:
                carry_1, sum_1 = sum_1, carry_1
                swapped.extend([sum_1, carry_1])
                carry_2 = find_gate(carry, sum_1, "AND")
            
            sum_2 = find_gate(carry, sum_1, "XOR")
            if sum_1 is not None and sum_1.startswith("z"):
                sum_1, sum_2 = sum_2, sum_1
                swapped.extend([sum_1, sum_2])
            
            if carry_1 is not None and carry_1.startswith("z"):
                carry_1, sum_2 = sum_2, carry_1
                swapped.extend([carry_1, sum_2])
            
            if carry_2 is not None and carry_2.startswith("z"):
                carry_2, sum_2 = sum_2, carry_2
                swapped.extend([carry_2, sum_2])
            
            new_carry = find_gate(carry_1, carry_2, "OR")
            if new_carry is not None and new_carry.startswith("z") and new_carry != f"z{z_count-1:02d}":
                new_carry, sum_2 = sum_2, new_carry
                swapped.extend([new_carry, sum_2])
            
            carry = new_carry
        else:
            carry = carry_1
    
    return ",".join(sorted(set(w for w in swapped if w)))

def solve(input_text: str, track_performance: bool = False) -> Tuple[int, str]:
    if track_performance:
        start = perf_counter()
    
    part1 = simulate_circuit(*parse_input(input_text))
    z_value = get_binary_value(part1, 'z')
    
    if track_performance:
        mid = perf_counter()
    
    part2 = find_swapped_wires(input_text)
    
    if track_performance:
        end = perf_counter()
        print(f"\nPerformance:")
        print(f"├─ Part 1: {(mid-start):.3f} seconds")
        print(f"├─ Part 2: {(end-mid):.3f} seconds")
        print(f"└─ Total:  {(end-start):.3f} seconds")
    
    return z_value, part2

def main():
    
    with open('input.txt', 'r') as file:
        input_text = file.read()
    part1, part2 = solve(input_text, track_performance=True)
    print(f"Part 1: {part1}")
    print(f"Part 2: {part2}")

if __name__ == "__main__":
    main()
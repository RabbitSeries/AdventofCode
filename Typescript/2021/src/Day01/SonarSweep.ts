import { readFileSync } from "node:fs"
import { EOL } from "node:os"
const data = readFileSync("Day01/input.txt").toString().split(EOL).map(Number)
const windows = data.slice(0, -2).map((_, i) => data.slice(i, i + 3).reduce((a, b) => a + b, 0))
const countIncrease = (arr: number[]) => arr.filter((num, i) => i > 0 && num > arr[i - 1]).length
console.log("Part 1: " + countIncrease(data))
console.log("Part 2: " + countIncrease(windows))
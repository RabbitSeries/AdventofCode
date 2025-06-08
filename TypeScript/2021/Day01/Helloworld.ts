import input from "./input.txt?raw"
const data = input.split("\n").map(s => parseInt(s))
const prefixSum = [...Array<number>(data.length - 2)]
for (const [i] of data.entries()) {
    prefixSum[i] = 0
    for (let j = 0; j < 3; j++) {
        prefixSum[i] += data[i + j]
    }
}
console.log("Part 1: " + data.filter((num, i) => i > 0 && num > data[i - 1]).length)
console.log("Part 2: " + prefixSum.filter((num, i) => i > 0 && num > prefixSum[i - 1]).length)

import { readFile } from "fs/promises";

const posList = await readFile("Day07/input.txt").then(data => data.toString().trim()).then(line => line.split(",")).then(row => row.map(pos => parseInt(pos)))

const submarines = new Map<number, number>()

for (const pos of posList) {
    submarines.set(pos, (submarines.get(pos) ?? 0) + 1)
}

function minAlignFuel(submarines: Map<number, number>, fuelCost: (distance: number) => number) {
    let minFuel: number = Infinity
    const [min, max] = submarines.keys().reduce((minMax, b) => [Math.min(minMax[0], b), Math.max(minMax[1], b)], [Infinity, -Infinity])
    for (let align = min; align <= max; align++) {
        let cost = 0
        for (const [other, count] of submarines.entries()) {
            cost += fuelCost(Math.abs(other - align)) * count
        }
        if (minFuel === undefined || cost < minFuel) {
            minFuel = cost
        }
    }
    return minFuel
}

console.log(minAlignFuel(submarines, (distance) => distance))
console.log(minAlignFuel(submarines, (distance: number) => (1 + distance) * distance / 2))

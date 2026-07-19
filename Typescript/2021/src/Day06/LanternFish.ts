import { readFile } from 'node:fs/promises'

const ages = await readFile('Day06/input.txt')
    .then(line => line.toString().trim().split(',').map(age => parseInt(age)))

const lanternFishes: Map<number, number> = new Map()

for (const age of ages) {
    lanternFishes.set(age, (lanternFishes.get(age) ?? 0) + 1)
}

function propagation(lanternFishes: Map<number, number>, days: number) {
    let fishes = lanternFishes
    while (days--) {
        lanternFishes = new Map()
        for (const [k, v] of fishes.entries()) {
            if (k) {
                lanternFishes.set(k - 1, (lanternFishes.get(k - 1) ?? 0) + v)
            }
            else {
                lanternFishes.set(6, (lanternFishes.get(6) ?? 0) + v)
                lanternFishes.set(8, (lanternFishes.get(8) ?? 0) + v)
            }
        }
        fishes = lanternFishes
    }
    return fishes.values().reduce((a, b) => a + b)
}

console.log(propagation(lanternFishes, 80))
console.log(propagation(lanternFishes, 256))

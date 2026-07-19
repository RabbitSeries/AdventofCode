import { readFile } from 'node:fs/promises'
import { EOL } from 'node:os'

export async function main() {
    const data = await readFile('Day14/input.txt', 'utf8')
    const blocks = data.split(`${EOL}${EOL}`)
    let polymers = new Map<string, number>()
    for (let i = 0; i <= blocks[0].length; i++) {
        const zipWithNext = (i ? blocks[0][i - 1] : ' ')
          + (blocks[0].length - i ? blocks[0][i] : ' ')
        polymers.set(zipWithNext, polymers.getOrInsert(zipWithNext, 0) + 1)
    }
    const rules = blocks[1].split(`${EOL}`).reduce((ruleMap, rule) => {
        const m = rule.match(/(\w\w) -> (\w)/)
        if (m) {
            ruleMap.set(m[1], m[2])
        }
        return ruleMap
    }, new Map<string, string>())
    function getStat() {
        const count = Array(26).fill(0)
        for (const [pair, pairN] of polymers) {
            if (pair[0] !== ' ') {
                count[pair.charCodeAt(0) - 'A'.charCodeAt(0)] += pairN
            }
            if (pair[1] !== ' ') {
                count[pair.charCodeAt(1) - 'A'.charCodeAt(0)] += pairN
            }
        }
        count.sort((a, b) => b - a)
        // Each one is counted twice
        // This can be proved using Mathematical Induction.
        return count[0] / 2 - count[count.findLastIndex(v => v !== 0)] / 2
    }
    for (let i = 0; i < 40; i++) {
        if (i === 10) {
            console.log(getStat())
        }
        const newPolymers: typeof polymers = new Map()
        for (const [pair, pairN] of polymers) {
            if (rules.has(pair)) {
                const pair1 = pair[0] + rules.get(pair)!
                const pair2 = rules.get(pair)! + pair[1]
                newPolymers.set(pair1,
                    newPolymers.getOrInsert(pair1, 0) + pairN)
                newPolymers.set(pair2,
                    newPolymers.getOrInsert(pair2, 0) + pairN)
            }
            else {
                newPolymers.set(pair, pairN)
            }
        }
        polymers = newPolymers
    }
    console.log(getStat())
}

await main()

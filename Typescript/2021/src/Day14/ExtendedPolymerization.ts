import { readFile } from 'node:fs/promises'
import { EOL } from 'node:os'

export async function main() {
    const data = await readFile('Day14/input.txt', 'utf8')
    const blocks = data.split(`${EOL}${EOL}`)
    let polymer = [...blocks[0]]
    const rules = blocks[1].split(`${EOL}`).reduce((ruleMap, rule) => {
        const m = rule.match(/(\w\w) -> (\w)/)
        if (m) {
            ruleMap.set(m[1], m[2])
        }
        return ruleMap
    }, new Map<string, string>())
    for (let i = 0; i < 10; i++) {
        const processed: string[] = []
        for (let j = 1; j < polymer.length; j++) {
            const key = polymer[j - 1] + polymer[j]
            if (rules.has(key)) {
                processed.push(polymer[j - 1], rules.get(key)!)
            }
            else {
                processed.push(polymer[j - 1])
            }
        }
        processed.push(polymer[polymer.length - 1])
        polymer = processed
    }
    const count = Array(26).fill(0)
    for (const ch of polymer) {
        count[ch.charCodeAt(0) - 'A'.charCodeAt(0)]++
    }
    count.sort((a, b) => b - a)
    console.log(count[0] - count[count.findLastIndex(v => v !== 0)])
}

await main()

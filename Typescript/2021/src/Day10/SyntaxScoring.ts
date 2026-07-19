import { readFile } from 'fs/promises'
import { EOL } from 'os'
const navSys = await readFile('Day10/input.txt')
    .then(data => data.toString().trimEnd()).then(data => data.split(`${EOL}`))
const bracketMapping = new Map([
    ['(', ')'],
    ['[', ']'],
    ['{', '}'],
    ['<', '>'],
])
const corruputedScroes = new Map([
    [')', 3],
    [']', 57],
    ['}', 1197],
    ['>', 25137],
])
const completeScores = new Map([
    [')', 1],
    [']', 2],
    ['}', 3],
    ['>', 4],
])
function corruputedChar(line: string,
    processRemain?: (stack: string[]) => void) {
    const stack: string[] = []
    for (let i = 0; i < line.length; i++) {
        if (bracketMapping.has(line[i])) {
            stack.push(line[i])
        }
        else if (!stack.length
          || line[i] !== bracketMapping.get(stack[stack.length - 1])) {
            return line[i]
        }
        else {
            stack.pop()
        }
    }
    // For part 2's autocomplete input list.
    if (processRemain) {
        processRemain(stack)
    }
    return null
}
const autoCompleteList: string[][] = []
console.log(navSys.map(line => corruputedChar(line,
    (stack: string[]) => autoCompleteList.push(stack)))
    .filter(v => v !== null)
    .map(v => corruputedScroes.get(v) ?? 0)
    .reduce((a, b) => a + b, 0))
function completeScore(stack: string[]) {
    let score = 0
    while (stack.length) {
        score *= 5
        score += completeScores.get(bracketMapping.get(stack.pop()!)!)!
    }
    return score
}
const scores = autoCompleteList.map(stack => completeScore(stack))
scores.sort((a, b) => a - b)
console.log(scores[Math.floor(scores.length / 2)])

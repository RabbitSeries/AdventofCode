import { readFile } from "fs/promises";
import { EOL } from "os";

const octopusMap = await readFile("Day11/input.txt")
    .then(data => data.toString().trimEnd().split(`${EOL}`))
    .then(lines => lines.map(line => [...line].map(level => parseInt(level))))
const [h, w] = [octopusMap.length, octopusMap[0].length]
const oKey = (i: number, j: number) => `${i},${j}`
const dx = [-1, 1, 0, 0, -1, -1, 1, 1]
const dy = [0, 0, -1, 1, -1, 1, -1, 1]
const isValid = (i: number, j: number) => i >= 0 && i < h && j >= 0 && j < w

function blink(octopusMap: number[][]) {
    let b = 0
    const q: [number, number][] = []
    const visited = new Set<string>()
    for (let i = 0; i < h; i++) {
        for (let j = 0; j < w; j++) {
            if (++octopusMap[i][j] > 9) {
                q.push([i, j])
                visited.add(oKey(i, j))
            }
        }
    }
    // This is actually a dfs because pop and push in js is FILO.
    while (q.length) {
        const [x, y] = q.pop()!
        b++
        // Just reset to 0.
        octopusMap[x][y] = 0
        for (let k = 0; k < 8; k++) {
            const [nx, ny] = [x + dx[k], y + dy[k]]
            const key = oKey(nx, ny)
            // Blinked octopus will not receive recharge on this step.
            // So perform blink check before recharge energy level.
            if (isValid(nx, ny) && !visited.has(key) && (++octopusMap[nx][ny] > 9)) {
                q.push([nx, ny])
                visited.add(key)
            }
        }
    }
    return b
}
let total = 0
let step = 0
while (step++ < 100) {
    total += blink(octopusMap)
}
console.log(total)
while (blink(octopusMap) !== w * h) {
    step++
}
console.log(step)

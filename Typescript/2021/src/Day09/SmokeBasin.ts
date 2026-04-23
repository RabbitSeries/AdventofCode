import { readFile } from "fs/promises";
import { EOL } from "os";

const map = await readFile("Day09/input.txt").then(data => data.toString().trimEnd().split(`${EOL}`))
// helpers
const [w, h] = [map[0].length, map.length]
const isValid = (x: number, y: number) => x >= 0 && x < h && y >= 0 && y < w
const pointKey = (x: number, y: number) => `${x},${y}`
const dx = [-1, 1, 0, 0]
const dy = [0, 0, -1, 1]

function mapLowPoints(map: string[]) {
    const points: [number, number][] = []
    for (let i = 0; i < h; i++) {
        for (let j = 0; j < w; j++) {
            let isLowPoint = true
            const p = map[i].charCodeAt(j)
            for (let k = 0; k < 4; k++) {
                const [x, y] = [i + dx[k], j + dy[k]]
                if (isValid(x, y) && map[x].charCodeAt(y) <= p) {
                    isLowPoint = false
                    break
                }
            }
            if (isLowPoint) {
                points.push([i, j])
            }
        }
    }
    return points
}

const lowPoints = mapLowPoints(map)

console.log(lowPoints.reduce((prev, p) => prev + parseInt(map[p[0]][p[1]]) + 1, 0))

function basin(i: number, j: number, visited: Set<string>) {
    const q = [[i, j]]
    let size = 0
    while (q.length) {
        size++
        const [x, y] = q.pop()!
        const p = map[x].charCodeAt(y)
        for (let k = 0; k < 4; k++) {
            const [nx, ny] = [x + dx[k], y + dy[k]]
            const key = pointKey(nx, ny)
            if (isValid(nx, ny) && !visited.has(key) && map[nx][ny] !== "9" && p < map[nx].charCodeAt(ny)) {
                visited.add(key)
                q.push([nx, ny])
            }
        }
    }
    return size
}

const basins: number[] = []
const visited: Set<string> = new Set()
for (const p of lowPoints) {
    const key = pointKey(p[0], p[1])
    if (!visited.has(key)) {
        visited.add(key)
        const b = basin(p[0], p[1], visited)
        if (b) {
            basins.push(b)
        }
    }
}

basins.sort((a, b) => b - a)

console.log(Iterator.from(basins).take(3).reduce((prev, v) => prev * v, 1))

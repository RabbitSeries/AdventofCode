import { /* open, */ readFile/* , constants */ } from "fs/promises";
import { EOL } from "os";

const lines = await readFile("Day05/input.txt")
    .then(data => data.toString().trim().split(EOL).map(line => {
        const posList = line.match(/(\d+),(\d+).*?(\d+),(\d+)/)?.slice(1).map(n => parseInt(n))
        return posList ? {
            x1: posList[0],
            y1: posList[1],
            x2: posList[2],
            y2: posList[3]
        } : undefined
    }).filter(m => m !== undefined))


function count_overlap(cross_only: boolean) {
    const process_lines = cross_only ? lines.filter(m => m.x1 === m.x2 || m.y1 === m.y2) : lines
    const overlap_zone = Array.from({ length: 1000 }, () => Array.from({ length: 1000 }, () => 0))
    for (const line of process_lines) {
        if (line.x1 === line.x2) {
            for (let y = Math.min(line.y1, line.y2); y <= Math.max(line.y1, line.y2); y++) {
                overlap_zone[y][line.x1]++
            }
        } else {
            const tan = (line.y2 - line.y1) / (line.x2 - line.x1)
            for (let dx = 0; dx <= Math.abs(line.x2 - line.x1); dx++) {
                const v_dx = dx * Math.sign(line.x2 - line.x1)
                overlap_zone[Math.floor(line.y1 + tan * v_dx)][line.x1 + v_dx]++
            }
        }
    }
    return overlap_zone.reduce((prev, row) => prev + row.reduce((prev, cnt) => cnt > 1 ? (prev + 1) : prev, 0), 0)
}
console.log(count_overlap(true))
console.log(count_overlap(false))

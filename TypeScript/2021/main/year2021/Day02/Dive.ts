import { readFileSync } from "node:fs"
import { EOL } from "node:os"
const data = readFileSync("input.txt").toString().trim().split(EOL)
let forward = 0, aim = 0, aimedDepth = 0
data.forEach(cmd => {
    const delta = +cmd.split(" ")[1]
    if (cmd.startsWith("forward")) {
        forward += delta
        aimedDepth += aim * delta
    } else {
        aim += cmd.startsWith("down") ? delta : -delta;
    }
})
console.log(`Part 1: ${forward * aim}`)
console.log(`Part 2: ${forward * aimedDepth}`)
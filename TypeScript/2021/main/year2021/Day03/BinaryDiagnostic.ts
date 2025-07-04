import { readFileSync } from "node:fs"
import { EOL } from "node:os"
const input = readFileSync("input.txt").toString().split(EOL)
const commonBit = (s: string) => [...s].filter(c => c === "1").length >= s.length / 2 ? "1" : "0"
const reverse = (s: "0" | "1") => s === "0" ? "1" : "0"
const commons = [...Array(input[0].length)].map((_, col) => input.map(s => s.charAt(col)).toSorted().join("")).map(s => commonBit(s))
const leasts = commons.map(reverse) //default separated with a comma
console.log(`Part 1: ${parseInt(commons.join(""), 2) * parseInt(leasts.join(""), 2)}`)
function filterBy(isCommon = false) {
    let q = input.map((_, sID) => ({ sID, bID: 0 }))
    while (q.length > 1) {
        const bitSet = q.map(e => input[e.sID][e.bID]).join("")
        const criteria = isCommon ? reverse(commonBit(bitSet)) : commonBit(bitSet)
        const nextLevel: typeof q = [];
        while (q.length > 0) {
            const res = q.shift()
            const { sID, bID } = res!
            if (input[sID][bID] === criteria) {
                nextLevel.push({ sID, bID: bID + 1 })
            }
        }
        q = nextLevel
    }
    return parseInt(input[q.shift()!.sID], 2)
}
console.log(`Part 2: ${filterBy() * filterBy(true)}`)
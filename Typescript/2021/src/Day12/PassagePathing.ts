import { readFile } from "fs/promises"
import { EOL } from "os"

enum CaveType {
    BigCave,
    SmallCave
}

class Cave {
    public constructor(caveName: string) {
        this.caveName = caveName
        this.caveType = [...caveName].reduce((p, c) => p && c.toUpperCase() === c, true) ? CaveType.BigCave : CaveType.SmallCave
    }
    public caveName: string
    public caveType: CaveType
}

const links = await readFile("Day12/input.txt")
    .then(data => data.toString().trimEnd().split(`${EOL}`))
    .then(lines =>
        lines.map(
            line => line.split("-").map(caveName => new Cave(caveName)) as [Cave, Cave]
        )
    )

const caveMap = new Map<string, Cave[]>()
for (const [from, to] of links) {
    caveMap.set(from.caveName, [...(caveMap.get(from.caveName) ?? []), to])
    caveMap.set(to.caveName, [...(caveMap.get(to.caveName) ?? []), from])
}

// Yet another expensive dfs.
function countWays(reachTimes = 1, currentCave: string = "start", visited = new Map<string, number>([["start", 1]]), hasTwiceInPath = false) {
    let wayCount = 0;
    if (currentCave === "end") {
        return 1;
    }
    for (const cave of caveMap.get(currentCave)!) {
        if (cave.caveType === CaveType.SmallCave) {
            const t = visited.get(cave.caveName) ?? 0
            if (t < reachTimes && cave.caveName !== "start") {
                let h = hasTwiceInPath
                if (t === 1) {
                    if (hasTwiceInPath) {
                        continue
                    } else {
                        h = true
                    }
                }
                visited.set(cave.caveName, t + 1)
                wayCount += countWays(reachTimes, cave.caveName, visited, h)
                visited.set(cave.caveName, t)
            }
        } else {
            wayCount += countWays(reachTimes, cave.caveName, visited, hasTwiceInPath)
        }
    }
    return wayCount
}

console.log(countWays())
console.log(countWays(2))

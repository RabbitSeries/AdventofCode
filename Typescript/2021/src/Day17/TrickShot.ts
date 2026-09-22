import { AocInput } from '@utils/iohelper'

function predictx(vx: number, t: number) {
    if (t <= vx) {
        return predicty(vx, t)
    }
    return predicty(vx, vx)
}

function predicty(vy: number, t: number) {
    return (vy + vy - t + 1) * t / 2
}

export async function main() {
    const data = new AocInput(17).toString().trimEnd()
    const re = /target area: x=(\d+)..(\d+), y=([-]*\d+)..([-]*\d+)/
    const result = re.exec(data)
    if (!result) {
        return
    }
    let max_h = -Infinity
    const [x1, x2, y1, y2] = result.slice(1).map(it => parseInt(it))
    const pv = []
    const visited = new Set<string>()
    // Uh, this is ugly
    // TODO
    // TODO
    // TODO
    // TODO
    for (let vx = 1; vx <= x2; vx++) {
        for (let t = 1; t <= 1000; t++) {
            const px = predictx(vx, t)
            if (px < x1) {
                continue
            }
            if (px > x2) {
                break
            }
            for (let vy = y1; vy <= 1000; vy++) {
                const py = predicty(vy, t)
                if (py < y1) {
                    continue
                }
                if (py > y2) {
                    continue
                }
                max_h = Math.max(max_h, predicty(vy, vy))
                if (visited.has(`${vx},${vy}`)) {
                    continue
                }
                visited.add(`${vx},${vy}`)
                pv.push([vx, vy])
            }
        }
    }
    console.log(max_h)
    console.log(pv.length)
}

main()

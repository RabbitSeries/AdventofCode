import { AocInput } from '@utils/iohelper'

function predict({ vx, vy }: { vx: number, vy: number }, t: number) {
    return {
        x: predictx(vx, t),
        y: predicty(vy, t),
    }
}

function predictx(vx: number, t: number) {
    if (t <= vx) {
        return predicty(vx, t)
    }
    return predicty(vx, vx)
}

function solve_t(vx: number, x: number) {
    if (x > predictx(vx, vx)) {
        return undefined
    }
    const b = 2 * vx + 1
    return Math.ceil((b - Math.sqrt(Math.pow(b, 2) - 8 * x)) / 2)
}

function predicty(vy: number, t: number) {
    return (vy + vy - t + 1) * t / 2
}

class Vector {
    public constructor(public x: number, public y: number) {}
    public plus(other: Vector) {
        return new Vector(this.x + other.x, this.y + other.y)
    }

    public times(num: number) {
        return new Vector(this.x * num, this.y * num)
    }

    public normalize() {
        return new Vector(Math.sign(this.x), Math.sign(this.y))
    }
}

// function max_height(x1: number, x2: number, y1: number, y2: number) {
//     // Choose one side
//     if (Math.sign(x1) * Math.sign(x2) > 0) { // Same sign
//         const vx_range = [0, Math.sign(x1)]
//     } else {

//     }
//     if (vx === 0) {
//         //
//     } else {
//         const l = Math.sign(x1) * Math.sign(x2) <= 0 ? 0 : Math.Math.min(x1, x2), r = Math.max(x1, x2)
//         while (l <= r) {
//             const mid = Math.floor((l + r) / 2)
//             if (predictx(vx, mid)) {}
//         }
//     }
// }

function min_vx(x: number) {
    let l = 1, r = x
    let best = r
    while (l < r) {
        const mid = Math.floor((l + r) / 2)
        const cmp = predictx(mid, mid) - x
        if (cmp < 0) {
            l = mid + 1
        } else {
            r = mid
            best = r
        }
    }
    return best
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
    for (let vx = min_vx(x1); vx <= x2; vx++) {
        const min_t = solve_t(vx, x1)
        const max_t = solve_t(vx, x2)
        for (let t = min_t!; max_t === undefined ? true : t <= max_t; t++) {
            const px = predictx(vx, t)
            if (px > x2) {
                break
            }
            const min_vy = Math.ceil(t / 2 + y1 / t - 1 / 2)
            const max_vy = Math.floor(t / 2 + y2 / t - 1 / 2)
            if (predicty(min_vy, t) > y2) {
                break
            }
            for (let vy = min_vy; vy <= max_vy; vy++) {
                const py = predicty(vy, t)
                if (py > y2) {
                    break
                }
                max_h = Math.max(max_h, predicty(vy, vy))
            }
        }
    }
    console.log(max_h)
}

main()

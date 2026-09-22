import { AocInput } from '@utils/iohelper'

function predict({ vx, vy }: { vx: number, vy: number }, t: number) {
    return {
        x: predictx(vx, t),
        y: predicty(vy, t),
    }
}

function predictx(vx: number, t: number) {
    if (t < vx) {
        return predicty(vx, t)
    }
    return predicty(vx, vx)
}

function predicty(vy: number, t: number) {
    return (vy + vy - t + 1) * t / 2
}

function solve_vy(y: number, t: number) {
    return Math.max(0, Math.sqrt(2 * y / t + t - 1))
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

export async function main() {
    const data = new AocInput(17).toString().trimEnd()
    const re = /target area: x=(\d+)..(\d+), y=([-]*\d+)..([-]*\d+)/
    const result = re.exec(data)
    if (!result) {
        return
    }
    let max_h = -Infinity
    const [x1, x2, y1, y2] = result.slice(1).map(it => parseInt(it))
    for (let vx = 1; vx <= x2; vx++) {
        let min_time = Infinity
        const max_time = Infinity
        const maxx = predictx(vx, vx)
        if (x1 > maxx) {
            min_time = Infinity
        }else{
            min_time = 
        }
        
        for (let t = min_time; t <= max_time; t++) {
            //  y1 <= vy * t - t^2 / 2 <= y2
            const [min_vy, max_vy] = [Math.ceil(solve_vy(y1, t)),
                Math.floor(solve_vy(y2, t))]
            for (let vy = min_vy; vy <= max_vy; vy++) {
                max_h = Math.max(max_h, predicty(vy, t))
                if (max_h === -5) {
                    console.log(max_h)
                }
            }
        }
    }
    console.log(max_h)
}

main()

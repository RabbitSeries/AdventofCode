import { aocInput } from '@utils/iohelper'
import { EOL } from 'node:os'

type Point3D = [number, number, number]

type Transform = [Point3D, Point3D, Point3D]

function transformed(p: Point3D, transform: Transform): Point3D {
    const result: Point3D = [0, 0, 0]
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            result[i] += p[j] * transform[j][i]
        }
    }
    return result
}

function minus(a: Point3D, b: Point3D) {
    return a.map((v, i) => v - b[i]) as Point3D
}

function plus(a: Point3D, b: Point3D) {
    return a.map((v, i) => v + b[i]) as Point3D
}

function toString(a: Point3D) {
    return a.join(',')
}

function overlap(scanner_a: Point3D[], scanner_b: Point3D[], a_b: Point3D) {
    const relative_a = scanner_b.map(v => plus(v, a_b))
    const hash_str: Set<string> = new Set()
    for (const p of scanner_a) {
        hash_str.add(toString(p))
    }
    let count = 0
    for (const p of relative_a) {
        if (hash_str.has(toString(p))) {
            count++
        }
    }
    return count >= 12 ? relative_a : undefined
}

const TRANSFORMS = (() => {
    const result: Transform[] = []
    for (let axis = 0; axis < 3; axis++) {
        for (const facing of [1, -1]) {
            for (let direction = 0; direction < 4; direction++) {
                const transform = Array.from({ length: 3 },
                    () => Array.from({ length: 3 }, () => 0)) as Transform
                transform[0][axis] = facing
                let conversion = true
                for (let i = 0; i < 3; i++) {
                    if (i === axis) {
                        continue
                    }
                    if (direction === 0) {
                        transform[1][i] = conversion ? 1 : 0
                        transform[2][i] = conversion ? 0 : 1
                    } else if (direction === 1) {
                        transform[1][i] = conversion ? 1 : 0
                        transform[2][i] = conversion ? 0 : -1
                    } else if (direction === 2) {
                        transform[1][i] = conversion ? -1 : 0
                        transform[2][i] = conversion ? 0 : 1
                    } else {
                        transform[1][i] = conversion ? -1 : 0
                        transform[2][i] = conversion ? 0 : -1
                    }
                    conversion = false
                }
                result.push(transform)
            }
        }
    }
    return result
})()

function consturct(scanner_a: Point3D[], scanner_b: Point3D[]) {
    const original = scanner_b
    for (const transform of TRANSFORMS) {
        scanner_b = original.map(v => transformed(v, transform))
        for (let a = 0; a < scanner_a.length; a++) {
            const beacon_a = scanner_a[a]
            for (let b = 0; b < scanner_b.length; b++) {
                const beacon_b = scanner_b[b]
                // beacon_a = a->P
                // transformed = b->p
                const a_b = minus(beacon_a, beacon_b)
                const result = overlap(scanner_a, scanner_b, a_b)
                if (result !== undefined) {
                    return result
                }
            }
        }
    }
    return undefined
}

export async function main() {
    const scanners = aocInput(19).splitblocks().map((block) => {
        const coordinates = block.split(EOL).slice(1)
        return coordinates.map((it) => {
            const m = it.matchAll(/[-]*\d+/g).toArray()
            return m.map(n => parseInt(n[0])) as Point3D
        })
    })
    const matched = new Set<number>()
    const q: number[] = []
    for (let i = 0; i < scanners.length - 1; i++) {
        for (let j = i + 1; j < scanners.length; j++) {
            console.log('Matching ', i, ' with ', j, ' ...')
            const match_result = consturct(scanners[i], scanners[j])
            if (match_result != undefined) {
                console.log('Matched', i, j)
                scanners[j] = match_result
                q.push(i, j)
                matched.add(i)
                matched.add(j)
                break
            }
        }
        if (q.length) {
            break
        }
    }
    while (q.length) {
        const i = q.shift()!
        for (let j = 0; j < scanners.length; j++) {
            if (matched.has(j)) {
                continue
            }
            console.log('Matching ', i, ' with ', j, ' ...')
            const match_result = consturct(scanners[i], scanners[j])
            if (match_result !== undefined) {
                console.log('Matched', i, j)
                scanners[j] = match_result
                q.push(j)
                matched.add(j)
            }
        }
    }
    const beacons = new Set<string>()
    for (const i of matched) {
        for (const beacon of scanners[i]) {
            beacons.add(beacon.join(','))
        }
    }
    console.log(beacons.size, ' in total')
}

await main()

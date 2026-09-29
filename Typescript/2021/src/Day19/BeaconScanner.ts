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
    return count >= 12
}

function cross_product(a: Point3D, b: Point3D) {
    const v = Array.from({ length: 3 }, () => 0)
    for (let i = 0; i < 3; i++) {
        const [j, k] = [(i + 1) % 3, (i + 2) % 3]
        v[i] = Math.pow(-1, i) * (a[j] * b[k] - a[k] * b[j])
    }
    return v as Point3D
}

function direction(axis: number, dir: number) {
    const v = Array.from({ length: 3 }, () => 0)
    v[axis] = dir / Math.abs(dir)
    return v as Point3D
}

const TRANSFORMS = (() => {
    const result: Transform[] = []
    for (let axis = 0; axis < 3; axis++) {
        for (const facing_v of [direction(axis, 1), direction(axis, -1)]) {
            // facing freedom 3 axes * 2 = 6
            for (let up_axis = 0; up_axis < 3; up_axis++) {
                if (up_axis === axis) { // up_axis freedom 2 * 2
                    continue
                }
                for (const left_hand of [
                    direction(up_axis, 1), direction(up_axis, -1),
                ]) {
                    const up_v = cross_product(facing_v, left_hand)
                    const transform = Array.from(
                        { length: 3 },
                        () => Array.from({ length: 3 }, () => 0) as Point3D,
                    ) as Transform
                    for (let i = 0; i < 3; i++) {
                        transform[i][0] = facing_v[i]
                        transform[i][1] = left_hand[i]
                        transform[i][2] = up_v[i]
                    }
                    result.push(transform)
                }
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
                // Assume beacon_a is beacon_b
                const beacon_b = scanner_b[b]
                const a_b = minus(beacon_a, beacon_b)
                if (overlap(scanner_a, scanner_b, a_b)) {
                    console.log(a_b)
                    return [transform, a_b] as [Transform, Point3D]
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
    const match_group = new Map<number, [number, Transform, Point3D][]>()
    const pair_q = [0]
    const pair_visited = new Set<number>([0])
    while (pair_q.length) {
        const i = pair_q.shift()!
        for (let j = 0; j < scanners.length; j++) {
            if (pair_visited.has(j)) {
                continue
            }
            console.log('Matching ', i, j)
            const match_result = consturct(scanners[i], scanners[j])
            if (match_result !== undefined) {
                console.log('Matched ', i, j)
                const [t, i_j] = match_result
                match_group.getOrInsert(i, []).push([j, t, i_j])
                pair_visited.add(j)
                pair_q.push(j)
            }
        }
    }
    const q: [number, [Transform, Point3D][]][] = [[0, []]]
    const paths = new Map<number, [Transform, Point3D][]>()
    const visited = new Set<number>([0])
    while (q.length) {
        const [i, path] = q.shift()!
        paths.set(i, path)
        for (const [child, t, i_j] of match_group.get(i) ?? []) {
            if (visited.has(child)) {
                continue
            }
            q.push([child, [[t, i_j], ...path]])
            visited.add(child)
        }
    }
    for (const [j, transforms] of paths) {
        let p: Point3D = [0, 0, 0]
        for (const [t, i_j] of transforms) {
            scanners[j] = scanners[j].map(v => plus(transformed(v, t), i_j))
            p = plus(p, i_j)
        }
        console.log(p)
    }
    const beacons = new Set<string>()
    for (const scanner of scanners) {
        for (const pos of scanner) {
            beacons.add(toString(pos))
        }
    }
    console.log('Found ', beacons.size, ' beacons')
}

await main()

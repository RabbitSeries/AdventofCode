import { aocInput } from '@utils/iohelper'
import { EOL } from 'node:os'

type Point3D = [number, number, number]

type Transform = [Point3D, Point3D, Point3D]

function transformed(p: Point3D, transform: Transform) {
    const result: Point3D = [0, 0, 0]
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            result[i] += p[j] * transform[j][i]
        }
    }
    return result
}

function transformedEuclidean(
    points: Point3D[],
    transform: Transform): Point3D[] {
    return points.map(p => transformed(p, transform))
}

function minus(a: Point3D, b: Point3D) {
    return a.map((v, i) => v - b[i]) as Point3D
}

function plus(a: Point3D, b: Point3D) {
    return a.map((v, i) => v + b[i]) as Point3D
}

function translationEuclidean(points: Point3D[], diff: Point3D) {
    return points.map(v => plus(v, diff))
}

function toString(a: Point3D) {
    return a.join(',')
}

function cross_product(a: Point3D, b: Point3D) {
    const v = Array.from({ length: 3 }, () => 0)
    for (let i = 0; i < 3; i++) {
        const [j, k] = [(i + 1) % 3, (i + 2) % 3]
        v[i] = (a[j] * b[k] - a[k] * b[j])
    }
    return v as Point3D
}

function unit_vec(axis: number, dir: number) {
    const v = Array.from({ length: 3 }, () => 0)
    v[axis] = dir / Math.abs(dir)
    return v as Point3D
}

// **Group** A^{-1} \in {A}, thus no need to perform an inverted match.
const TRANSFORMS = (() => {
    const result: Transform[] = []
    for (let facing_axis = 0; facing_axis < 3; facing_axis++) {
        for (const facing_v of [
            unit_vec(facing_axis, 1),
            unit_vec(facing_axis, -1),
        ]) {
            // facing freedom 3 axes * 2 = 6
            for (let left_axis = 0; left_axis < 3; left_axis++) {
                if (left_axis === facing_axis) { // left_axis freedom 2 * 2
                    continue
                }
                for (const left_v of [
                    unit_vec(left_axis, 1), unit_vec(left_axis, -1),
                ]) {
                    const up_v = cross_product(facing_v, left_v)
                    const transform = Array.from(
                        { length: 3 },
                        () => Array.from({ length: 3 }, () => 0) as Point3D,
                    ) as Transform
                    for (let i = 0; i < 3; i++) {
                        transform[i][0] = facing_v[i]
                        transform[i][1] = left_v[i]
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
    const hash_str: Set<string> = new Set()
    for (const p of scanner_a) {
        hash_str.add(toString(p))
    }

    for (const transform of TRANSFORMS) {
        const reoriented = transformedEuclidean(scanner_b, transform)
        for (const beacon_a of scanner_a) {
            for (const beacon_b of reoriented) {
                // Assume beacon_a and beacon_b signifies the same beacon.
                const a_b = minus(beacon_a, beacon_b)

                let count = 0
                for (const p of translationEuclidean(reoriented, a_b)) {
                    if (hash_str.has(toString(p))) {
                        count++
                        if (count >= 12) {
                            return [transform, a_b] as [Transform, Point3D]
                        }
                    }
                }
            }
        }
    }
    return undefined
}

function invert(m: Transform): Transform {
    const [[a, b, c], [d, e, f], [g, h, i]] = m

    const A = (e * i - f * h)
    const B = -(d * i - f * g)
    const C = (d * h - e * g)
    const det = a * A + b * B + c * C

    return [
        [A / det, -(b * i - c * h) / det, (b * f - c * e) / det],
        [B / det, (a * i - c * g) / det, -(a * f - c * d) / det],
        [C / det, -(a * h - b * g) / det, (a * e - b * d) / det],
    ]
}

export async function main() {
    const scanners = aocInput(19).splitblocks().map((block) => {
        const coordinates = block.split(EOL).slice(1)
        return coordinates.map((it) => {
            const m = it.matchAll(/[-]*\d+/g).toArray()
            return m.map(n => parseInt(n[0])) as Point3D
        })
    })
    const match_group = new Map<number, Map<number, [Transform, Point3D]>>()
    for (let i = 0; i < scanners.length - 1; i++) {
        for (let j = i + 1; j < scanners.length; j++) {
            const match_result = consturct(scanners[i], scanners[j])
            if (match_result !== undefined) {
                console.log('Matched', i, j)
                const [t, i_j] = match_result
                match_group.getOrInsert(i, new Map()).set(j, [t, i_j])
                const inverted = invert(t)
                match_group.getOrInsert(j, new Map()).set(i, [inverted,
                    minus([0, 0, 0], transformed(i_j, inverted))])
            }
        }
    }
    // Reorient scanners data to the first scanner's coordinates system.
    const q: [number, [Transform, Point3D][]][] = [[0, []]]
    const paths = new Map<number, [Transform, Point3D][]>()
    const visited = new Set<number>([0])
    while (q.length) {
        const [i, path] = q.shift()!
        paths.set(i, path)
        for (const [child, [t, i_j]] of match_group.get(i)!) {
            if (visited.has(child)) {
                continue
            }
            q.push([child, [[t, i_j], ...path]])
            visited.add(child)
        }
    }
    // Above bfs forms the transform and translation path
    // to rotate the scanner.
    const scanner_positions = Array.from(
        { length: scanners.length },
        () => [0, 0, 0] as Point3D)
    for (const [j, transforms] of paths) {
        let p: Point3D = [0, 0, 0]
        for (const [t, i_j] of transforms) {
            scanners[j] = translationEuclidean(
                transformedEuclidean(scanners[j], t),
                i_j)
            p = plus(transformed(p, t), i_j)
        }
        scanner_positions[j] = p
    }
    for (let i = 0; i < scanners.length; i++) {
        if (!match_group.has(i)) {
            console.log('Missing', i)
        }
    }
    if (match_group.size !== scanners.length) {
        throw Error('There are unmatched groups')
    }
    const beacons = new Map<string, Point3D>()
    for (const i of match_group.keys()) {
        for (const pos of scanners[i]) {
            beacons.set(toString(pos), pos)
        }
    }
    console.log('Found', beacons.size, 'beacons')
    let max_dis = 0
    for (let i = 0; i < scanners.length - 1; i++) {
        for (let j = i + 1; j < scanners.length; j++) {
            const s1 = scanner_positions[i]
            const s2 = scanner_positions[j]
            const v = minus(s1, s2)
            const dis = Math.abs(v[0]) + Math.abs(v[1]) + Math.abs(v[2])
            max_dis = Math.max(dis, max_dis)
        }
    }
    console.log('The largest distance is', max_dis)
}

await main()

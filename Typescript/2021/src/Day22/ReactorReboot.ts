import { aocInput } from '@utils/iohelper'
type Cuboid = {
    matrix: [number, number][]
    sign: number
}
function Volume(cuboid: Cuboid) {
    let v = 1
    for (const [low, high] of cuboid.matrix) {
        v = v * (high - low + 1)
    }
    return v * cuboid.sign
}
function* intersect(current: Cuboid[], b: Cuboid): Generator<Cuboid> {
    const intersect_impl = (a: Cuboid, b: Cuboid) => {
        const c: [number, number][] = []
        for (let i = 0; i < 3; i++) {
            const [l, r] = [a.matrix[i], b.matrix[i]]
            const [low, high] = [Math.max(l[0], r[0]), Math.min(l[1], r[1])]
            if (low > high) {
                return undefined
            }
            c.push([low, high])
        }
        return {
            matrix: c,
            sign: a.sign * b.sign,
        }
    }
    if (b.sign === 1) {
        yield b
        b = {
            matrix: b.matrix,
            sign: -1,
        }
    }
    for (const a of current) {
        const c = intersect_impl(a, b)
        if (c !== undefined) {
            yield c
        }
    }
}

function total_volume(cuboids: Cuboid[]) {
    const first = cuboids[0]
    const rest = cuboids.splice(1)
    const result = [first]
    for (const cuboid of rest) {
        for (const itc of [...intersect(result, cuboid)]) {
            result.push(itc)
        }
    }
    return result
}

export async function main() {
    const cuboids = aocInput(22).splitlines()
        .map((it) => {
            const ranges = it.matchAll(/[-]*\d+/g)
                .map(v => parseInt(v[0])).toArray()
            return {
                matrix: [
                    ranges.splice(0, 2) as [number, number],
                    ranges.splice(0, 2) as [number, number],
                    ranges as [number, number],
                ],
                sign: it.startsWith('on') ? 1 : -1,
            }
        })
    console.log(total_volume(cuboids.filter(it => it.matrix.flatMap(v => v)
        .reduce(
            (prev, curr) => prev && curr >= -50 && curr <= 50,
            true,
        ))).reduce((prev, curr) => prev + Volume(curr), 0))
    console.log(total_volume(cuboids)
        .reduce((prev, curr) => prev + Volume(curr), 0),
    )
}

await main()

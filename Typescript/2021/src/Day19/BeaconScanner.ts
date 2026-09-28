import { aocInput } from '@utils/iohelper'
import { EOL } from 'node:os'

type Point3D = [number, number, number]

type Transform = [Point3D, Point3D, Point3D]

function transform(p: Point3D, transform: Transform) {
    const result = [0, 0, 0]
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            result[i] += p[j] * transform[j][i]
        }
    }
    return result
}

export async function main() {
    const scanners = aocInput(19).splitblocks().map((block) => {
        const coordinates = block.split(EOL).slice(1)
        return coordinates.map((it) => {
            const m = it.matchAll(/[-]*\d+/g).toArray()
            return m.map(n => parseInt(n[0]))
        },
        )
    })
    for (let i = 0; i < scanners.length; i++) {
        for (let j = 0; j < scanners.length; j++) {
            // Huh?
        }
    }
}

import { aocInput, splitlines } from '@utils/iohelper'
// import { createWriteStream } from 'node:fs'
// import { EOL } from 'node:os'

type Point2D = {
    x: number
    y: number
}

function sampling(
    image: string[],
    enhance_map: string,
    fill: string,
    p: Point2D,
) {
    const rows = image.length
    const cols = image[0].length
    let bits = ''
    for (let i = -1; i <= 1; i++) {
        for (let j = -1; j <= 1; j++) {
            const x = p.x + i
            const y = p.y + j
            if (x < 0 || x >= rows || y < 0 || y >= cols) {
                bits += fill === '#' ? '1' : '0'
                continue
            }
            bits += image[x][y] === '.' ? '0' : '1'
        }
    }
    return enhance_map[parseInt(bits, 2)]
}

// function print_to_file(image: string[]) {
//     const ws = createWriteStream(
//         '/home/rabbit/AdventofCode/Typescript/2021/src/Day20/output.txt', {
//             autoClose: true,
//         })
//     ws.write(image.join(EOL))
//     return new Promise<void>(resolve => ws.end(() => resolve()))
// }

function enhance(image: string[], enhance_map: string, fill: string) {
    const rows = image.length
    const cols = image[0].length
    return Array.from(
        { length: rows + 2 },
        (_, x) => Array.from(
            { length: cols + 2 },
            (_, y) => {
                return sampling(
                    image,
                    enhance_map,
                    fill,
                    { x: x - 1, y: y - 1 })
            }).join(''),
    )
}

function count_lit(s: string) {
    let n = 0
    for (const c of s) {
        if (c === '#') {
            n++
        }
    }
    return n
}

export async function main() {
    const [enhance_map, image_raw] = aocInput(20).splitblocks()
    const image = splitlines(image_raw)
    let fill = '.'
    let result = image
    // await print_to_file(result)
    for (let i = 0; i < 2; i++) {
        result = enhance(result, enhance_map, fill)
        fill = fill === '#' ? enhance_map[511] : enhance_map[0]
        // await print_to_file(result)
    }
    console.log('Pixels lit:', result.reduce(
        (prev, curr) =>
            prev + count_lit(curr), 0),
    )
    for (let i = 0; i < 48; i++) {
        result = enhance(result, enhance_map, fill)
        fill = fill === '#' ? enhance_map[511] : enhance_map[0]
        // await print_to_file(result)
    }
    console.log('Pixels lit:', result.reduce(
        (prev, curr) =>
            prev + count_lit(curr), 0),
    )
}

await main()

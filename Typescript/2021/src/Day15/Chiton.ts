import Heap from '@utils/Heap'
import { readFile } from 'node:fs/promises'
import { EOL } from 'node:os'

const data = await readFile('Day15/input.txt')
    .then(it => it.toString().trim().split(EOL).map(row => [...row].map(s =>
        s.charCodeAt(0) - '0'.charCodeAt(0),
    )))

const dx = [0, 0, -1, 1]
const dy = [1, -1, 0, 0]

type Status = {
    x: number
    y: number
    risk_level: number
    manhattan: number
}

function expand_col(data: number[][]) {
    for (let r = 0; r < data.length; r++) {
        let copy = data[r]
        for (let i = 0; i < 4; i++) {
            copy = copy.map((i) => {
                const add_one = i % 9 + 1
                data[r].push(add_one)
                return add_one
            })
        }
    }
}

function expand_row(data: number[][]) {
    const row = data.length
    for (let i = 0; i < 4; i++) {
        for (let r = i * row; r < (i + 1) * row; r++) {
            const new_row = []
            for (let j = 0; j < data[r].length; j++) {
                new_row.push(data[r][j] % 9 + 1)
            }
            data.push(new_row)
        }
    }
}

function manhattan(data: number[][], x: number, y: number) {
    const [row, col] = [data.length, data[0].length]
    return row - x + col - y
}

function main() {
    const min_risk_level = Array.from({ length: data.length },
        () => Array.from({ length: data[0].length })
            .fill(Infinity) as number[])
    min_risk_level[0][0] = 0
    const [row, col] = [data.length, data[0].length]
    const h = new Heap<Status>((i, j) => {
        if (j.risk_level !== i.risk_level) {
            return i.risk_level - j.risk_level
        } else {
            return i.manhattan - j.manhattan
        }
    })

    h.push({
        x: 0,
        y: 0,
        risk_level: 0,
        manhattan: manhattan(data, 0, 0),
    })

    while (h.length) {
        const s = h.pop()!
        if (s.x === row - 1 && s.y === col - 1) {
            console.log(s.risk_level)
            return
        }
        for (let i = 0; i < 4; i++) {
            const nx = s.x + dx[i]
            const ny = s.y + dy[i]
            if (nx >= 0 && nx < row && ny >= 0 && ny < col
              && data[nx][ny] + s.risk_level < min_risk_level[nx][ny]) {
                const r = data[nx][ny] + s.risk_level
                min_risk_level[nx][ny] = r
                h.push({
                    x: nx,
                    y: ny,
                    risk_level: r,
                    manhattan: manhattan(min_risk_level, nx, ny),
                })
            }
        }
    }
    console.log('Not found')
}
main()
expand_row(data)
expand_col(data)
main()

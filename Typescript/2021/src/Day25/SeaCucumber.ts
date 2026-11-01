import { aocInput } from '@utils/iohelper'

function move_sc(s: string[][], plan: [number, number][], isRight: boolean) {
    const [row, col] = [s.length, s[0].length]
    while (plan.length) {
        const [i, j] = plan.shift()!
        if (isRight) {
            const next_col = j + 1 < col ? j + 1 : 0;
            [s[i][j], s[i][next_col]] = [s[i][next_col], s[i][j]]
        } else {
            const next_row = i + 1 < row ? i + 1 : 0;
            [s[i][j], s[next_row][j]] = [s[next_row][j], s[i][j]]
        }
    }
}

function move(s: string[][]) {
    const [row, col] = [s.length, s[0].length]
    const choices: [number, number][] = []
    for (let i = 0; i < row; i++) {
        for (let j = 0; j < col; j++) {
            if (s[i][j] == '>') {
                const next_col = (j + 1) % col
                if (s[i][next_col] == '.') {
                    choices.push([i, j])
                }
            }
        }
    }
    let moved = choices.length > 0
    move_sc(s, choices, true)
    for (let i = 0; i < row; i++) {
        for (let j = 0; j < col; j++) {
            if (s[i][j] == 'v') {
                const next_row = (i + 1) % row
                if (s[next_row][j] == '.') {
                    choices.push([i, j])
                }
            }
        }
    }
    moved = moved || choices.length > 0
    move_sc(s, choices, false)
    return moved
}

export async function main() {
    const situation = aocInput(25).splitlines().map(it => [...it])
    let step = 1
    while (move(situation)) {
        step++
    }
    console.log(step)
}

await main()

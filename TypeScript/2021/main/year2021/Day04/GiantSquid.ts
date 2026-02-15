import { readFile } from "fs/promises";
import { EOL } from "os";

const input_blocks = await readFile("Day04/input.txt").then(buf => buf.toString().trim().split(`${EOL}${EOL}`))
const bingo_nums = input_blocks[0].split(",").map(s => parseInt(s))
const parse_boards = (board_blocks: string[]) =>
    board_blocks.map(board => {
        const indexHash: Map<number, { i: number, j: number }> = new Map()
        const boardCells: number[][] = Array.from({ length: 5 }, () => Array(5))
        board.split(EOL).map((line, i) => {
            line.trim().split(/\s+/).map((cell, j) => {
                const num = parseInt(cell)
                boardCells[i][j] = num
                indexHash.set(num, { i, j })
            })
        })
        return {
            indexHash,// Each board has unique numbers in this tests example
            boardCells,
            rowRemain: Array.from({ length: 5 }, () => 5),
            colRemain: Array.from({ length: 5 }, () => 5)
        }
    })

function run_bingo(first_win: boolean = true) {
    let bingo_boards = parse_boards(input_blocks.slice(1))
    for (const number of bingo_nums) {
        const nextRound = []
        for (const board of bingo_boards) {
            if (!board.indexHash.has(number)) {
                nextRound.push(board)
                continue
            }
            const { i, j } = board.indexHash.get(number)!
            board.indexHash.delete(number)
            if (--board.rowRemain[i] && --board.colRemain[j]) {
                nextRound.push(board)
                continue
            }
            if (first_win) {
                const sum = board.indexHash.keys().reduce((a, b) => a + b, 0)
                return sum * number
            } else {
                if (bingo_boards.length === 1) {
                    const sum = board.indexHash.keys().reduce((a, b) => a + b, 0)
                    return sum * number
                }
            }
        }
        bingo_boards = nextRound
    }
}

console.log(run_bingo())

console.log(run_bingo(false))

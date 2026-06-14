import { readFile } from "fs/promises";
import { splitblocks, splitlines } from "@utils/iohelper"

enum FoldDirection {
    Vertical,
    Horizontal
}

class TransparentOrigami {
    private paper: boolean[][] = []
    private positions: [number, number][] = []
    private instructions: { direction: FoldDirection, position: number }[] = []
    public async part1() {
        const parsed = await readFile("Day13/input.txt").then(data => splitblocks(data)).then(blocks => {
            const positions = splitlines(blocks[0]).map(line => line.split(",").map(p => parseInt(p)) as [number, number])
            const instructions = splitlines(blocks[1]).map(line => {
                const result = line.match(/fold along (\w)=(\d+)/)!
                return {
                    direction: result[1] === "x" ? FoldDirection.Horizontal : FoldDirection.Vertical,
                    position: parseInt(result[2])
                }
            })
            return { positions, instructions }
        })
        this.positions = parsed.positions
        this.instructions = parsed.instructions
        const [w, h] = this.positions.reduce(([w, h], [x, y]) => [Math.max(w, x + 1), Math.max(h, y + 1)], [0, 0])
        this.paper = Array.from({ length: h }, () => Array.from({ length: w }, () => false))
        for (const [x, y] of this.positions) {
            this.paper[y][x] = true
        }
        this.fold(this.instructions[0])
        return this.paper.reduce((prev, row) => prev + row.reduce((prev, curr) => prev + +curr, 0), 0)
    }

    public async part2() {
        for (const option of this.instructions.slice(1)) {
            this.fold(option)
        }
        return this.paper.map(row => row.map(bl => bl ? "#" : " ").join("")).join("\n")
    }

    private fold(option: { direction: FoldDirection, position: number }) {
        if (option.direction === FoldDirection.Horizontal) {
            for (let y = 0; y < this.paper.length; y++) {
                for (let dis = 1; option.position - dis >= 0 && option.position + dis < this.paper[y].length; dis++) {
                    const [x, mirrorX] = [option.position - dis, option.position + dis]
                    this.paper[y][x] = this.paper[y][x] || this.paper[y][mirrorX]
                }
                this.paper[y] = this.paper[y].slice(0, option.position)
            }
        } else {
            for (let x = 0; x < this.paper[0].length; x++) {
                for (let dis = 1; option.position - dis >= 0 && option.position + dis < this.paper.length; dis++) {
                    const [y, mirrorY] = [option.position - dis, option.position + dis]
                    this.paper[y][x] = this.paper[y][x] || this.paper[mirrorY][x]
                }
            }
            this.paper = this.paper.slice(0, option.position)
        }
    }
}

const solution = new TransparentOrigami()
console.log(await solution.part1())
console.log(await solution.part2())

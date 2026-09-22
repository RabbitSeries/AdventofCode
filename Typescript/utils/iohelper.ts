// This file must be included in tsconfig.ts to use
//  @types/node intellisense in certain way.
// @types/node doesn't require import.
// import type Buffer from "node:buffer"
import { readFileSync } from 'node:fs'
import { EOL } from 'node:os'
export function splitlines(buffer: Buffer<ArrayBuffer> | string) {
    const str = typeof buffer === 'string'
        ? buffer
        : buffer.toString().trimEnd()
    return str.split(`${EOL}`)
}
export function splitblocks(buffer: Buffer<ArrayBuffer>) {
    return buffer.toString().trimEnd().split(`${EOL}${EOL}`)
}
export class AocInput {
    public constructor(private day: number) {
        this.input_path = `Day${this.day < 10 ? '0' : ''}${this.day}/input.txt`
    }

    private fetch_data() {
        if (this.data === undefined) {
            this.data = readFileSync(this.input_path)
        }
        return this.data!
    }

    public splitlines() {
        return splitlines(this.fetch_data())
    }

    public splitblocks() {
        return splitblocks(this.fetch_data())
    }

    public toString() {
        return this.fetch_data().toString()
    }

    private input_path: string
    private data: Buffer<ArrayBuffer> | undefined
}

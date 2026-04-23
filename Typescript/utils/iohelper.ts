// This file must be included in tsconfig.ts to use @types/node intellisense in certain way.
// @types/node doesn't require import.
// import type Buffer from "node:buffer"
import { EOL } from "node:os"
export function splitlines(buffer: Buffer<ArrayBuffer> | string) {
    const str = typeof buffer === "string" ? buffer : buffer.toString().trimEnd()
    return str.split(`${EOL}`)
}
export function splitblocks(buffer: Buffer<ArrayBuffer>) {
    return buffer.toString().trimEnd().split(`${EOL}${EOL}`)
}
import * as esbuild from "esbuild"
import fg from "fast-glob"
import fs from "node:fs"
const src = fg.globSync(["./**/*.ts", "!node_modules"])
const outDirectory = "dist";
if (fs.existsSync(outDirectory)) {
    fs.rmSync(outDirectory, { force: true, recursive: true })
}
const ctx = await esbuild.context({
    target: "esnext",
    entryPoints: src,
    platform: "node",
    format: "esm",
    bundle: true,
    outbase: ".",
    outdir: outDirectory,
    sourcemap: "inline",
    logLevel: "info",
})
await ctx.watch()
console.log('Watching...')
await ctx.rebuild()
console.log('Initial build finished.')

import * as esbuild from "esbuild"
import fg from "fast-glob"
import * as fs from "node:fs"
const src = fg.globSync(["./2021/**/*.ts", "!**/*.md"])
console.log(`Found files: ${src}`)
const outDirectory = "dist";
if (fs.existsSync(outDirectory)) {
    fs.rmSync(outDirectory, { force: true, recursive: true })
}
await esbuild.build({
    entryPoints: src,
    platform: "node",
    format: "esm",
    bundle: true,
    outdir: outDirectory,
    sourcemap: "inline"
})
console.log(`bundle finished`)
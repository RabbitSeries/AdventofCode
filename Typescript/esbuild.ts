import * as esbuild from "esbuild"
import fg from "fast-glob"
import fs from "node:fs"
// import path from "node:path";
const src = fg.globSync(["./**/*.ts", "!node_modules"])
const outDirectory = "dist";
if (fs.existsSync(outDirectory)) {
    fs.rmSync(outDirectory, { force: true, recursive: true })
}

// const utilsAliasResolve: esbuild.Plugin = {
//     name: "OK, I have to give you a name, jesus.",
//     setup(build) {
//         build.onResolve({ filter: /^@utils\// }, args => {
//             return {
//                 path: path.resolve(path.format({
//                     dir: "./utils",
//                     name: args.path.substring("utils/".length),
//                     ext: "ts"
//                 }))
//             }
//         })
//     }
// }

for (const year of [2021]) {
    const ctx = await esbuild.context({
        target: "esnext",
        entryPoints: src,
        platform: "node",
        format: "esm",
        tsconfig: `./${year}/tsconfig.json`,
        bundle: true,
        outbase: ".",
        outdir: outDirectory,
        sourcemap: "inline",
        logLevel: "info",
        // plugins: [utilsAliasResolve]
    })
    await ctx.watch()
    console.log(`Watching year-${year}...`)
    await ctx.rebuild()
    console.log(`Initial build year-${year} finished.`)
}

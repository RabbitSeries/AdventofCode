import { glob } from 'node:fs/promises'
import { pathToFileURL } from 'node:url'

const data: { main: () => void }[] = []
for await (const d of glob('./**/*.js', { exclude: ['**/main.js'] })) {
    const url = new URL(pathToFileURL(d).href).toString()
    console.log(url)
    try {
        data.push(await import(url) as { main: () => void })
        console.log(data[0].main)
    } catch {
        //
    }
}
console.log(data)

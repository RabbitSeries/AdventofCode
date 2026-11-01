import HashMap from '@utils/HashMap'
import { aocInput } from '@utils/iohelper'

type Rule = {
    a: number
    b: number
    c: number
}

function analyse_data(data: string[]) {
    const rules: string[][] = []
    let buf: string [] = []
    const itr = Iterator.from(data)
    let val = itr.next()
    while (!val.done) {
        while (!val.done && !val.value.startsWith('inp')) {
            buf.push(val.value)
            val = itr.next()
        }
        if (buf.length) {
            rules.push(buf)
            buf = []
        } else if (!val.done) {
            buf.push(val.value)
            val = itr.next()
        }
    }
    console.log(
        rules.slice()
            .map(it => it.map(line => line.split(' ').slice(-1)[0]).join('\t'))
            .join('\n'),
    )
    return rules
}

function runMONAD(n: number[], rules: Rule[]) {
    let z = 0
    for (let i = 0; i < rules.length; i++) {
        const { a, b, c } = rules[i]
        const w = n[i]
        const x = +((z % 26 + a) == w)
        z = z / b
        z = Math.sign(z) * Math.floor(Math.abs(z))
        let y = 25 * x + 1
        z = z * y
        y = (w + c) * x
        z = z + y
    }
    console.log(z)
}

type State = {
    step: number
    z: number
}

function solveMONAD(rules: Rule[], maximum: boolean) {
    const q: [State, number[]][] = []
    const visited = new HashMap<State, number>(s => `${s.step},${s.z}`)
    let monad = maximum ? -Infinity : Infinity
    const toInt = (digits: number[]) => parseInt(digits.join(''))
    const check_and_push = (s: State, n: number[], w: number) => {
        if (!(w > 0 && w < 10)) {
            return
        }
        const result = [w, ...n]
        const curr = visited.get(s)
        const num = toInt(result)
        if (curr !== undefined && (maximum ? (num >= curr) : (num <= curr))) {
            return
        }
        visited.upsert(s, () => num)
        q.push([s, result])
    }
    q.push(
        [{ step: rules.length, z: 0 }, []],
    )
    visited.upsert({ step: rules.length, z: 0 }, () => 0)
    let digit = 0
    while (q.length) {
        const [s, n] = q.shift()!
        const num = toInt(n)
        if (maximum ? (visited.get(s)! > num) : (visited.get(s)! < num)) {
            continue
        }
        if (n.length > digit) {
            console.log(n.length)
            digit = n.length
        }
        const { z, step } = s
        if (step == 0) {
            // Ouput
            if (z !== 0) {
                console.log(
                    `Failed ${z}`,
                    num.toLocaleString(undefined, { useGrouping: false }),
                )
            } else {
                if (maximum) {
                    monad = Math.max(num, monad)
                } else {
                    monad = Math.min(num, monad)
                }
                console.log(
                    num.toLocaleString(undefined,
                        { useGrouping: false }))
            }
            continue
        }
        const { a, b, c } = rules[step - 1]
        if (b == 1) {
            // case 1
            {
                const oldZ = Math.floor(z / 26)
                const w = z % 26 - c
                check_and_push(
                    { step: step - 1, z: oldZ },
                    n,
                    w,
                )
            }
            // case 3
            check_and_push(
                {
                    step: step - 1,
                    z: z,
                },
                n,
                z % 26 + a,
            )
        } else {
            // case 2
            {
                const w = z % 26 - c
                for (let i = 0; i < 26; i++) {
                    if (i == w - a) {
                        continue
                    }
                    const oldZ = Math.floor(z / 26) * 26 + i
                    check_and_push(
                        {
                            step: step - 1,
                            z: oldZ,
                        },
                        n, w,
                    )
                }
            }
            // case 4
            {
                for (let i = 0; i < 26; i++) {
                    const oldZ = z * 26 + i
                    const w = oldZ % 26 + a
                    check_and_push(
                        {
                            step: step - 1,
                            z: oldZ,
                        },
                        n, w,
                    )
                }
            }
        }
    }
    return monad
}

export async function main() {
    const data = aocInput(24).splitlines()
    const rules: Rule[] = analyse_data(data).map((it) => {
        return {
            a: parseInt(it[5].split(' ').slice(-1)[0]),
            b: parseInt(it[4].split(' ').slice(-1)[0]),
            c: parseInt(it[15].split(' ').slice(-1)[0]),
        }
    })
    const part1 = solveMONAD(rules, true)
        .toLocaleString(undefined, { useGrouping: false })
    console.log(part1)
    runMONAD([...part1].map(it => parseInt(it)), rules)

    // TODO, too slow
    const part2 = solveMONAD(rules, false)
        .toLocaleString(undefined, { useGrouping: false })
    console.log(part2)
    runMONAD([...part2].map(it => parseInt(it)), rules)
}

await main()

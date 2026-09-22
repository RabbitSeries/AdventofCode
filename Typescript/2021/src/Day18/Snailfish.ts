import { aocInput } from '@utils/iohelper'

type SnailFish = [number | SnailFish, number | SnailFish]

class Parser {
    public constructor(private line: string) {}

    private feed(pos: number) {
        this.line = this.line.slice(pos)
    }

    // E->[V,V]
    // V->\d+|[V,V]
    private E(): SnailFish {
        this.feed(1)
        const v1 = this.V()
        this.feed(1)
        const v2 = this.V()
        this.feed(1)
        return [v1, v2]
    }

    private V(): number | SnailFish {
        const m = /^\d+/.exec(this.line)
        if (m) {
            this.feed(m[0].length)
            return parseInt(m[0])
        }
        this.feed(1)
        const v1 = this.V()
        this.feed(1)
        const v2 = this.V()
        this.feed(1)
        return [v1, v2]
    }

    static parse(line: string) {
        const p = new Parser(line)
        return p.E()
    }
}

function is_arr(p: number | SnailFish): p is SnailFish {
    return p instanceof Array
}
function is_number(p: number | SnailFish): p is number {
    return !is_arr(p)
}

function deepcopy(num: SnailFish): SnailFish {
    let l = num[0]
    if (is_arr(l)) {
        l = deepcopy(l)
    }
    let r = num[1]
    if (is_arr(r)) {
        r = deepcopy(r)
    }
    return [l, r]
}
function add_into(p: SnailFish, n: number, left: boolean) {
    if (left) {
        let leftmost = p
        while (is_arr(leftmost[0])) {
            leftmost = leftmost[0]
        }
        leftmost[0] += n
    } else {
        let rightmost = p
        while (is_arr(rightmost[1])) {
            rightmost = rightmost[1]
        }
        rightmost[1] += n
    }
}

function explode(
    num: SnailFish,
    depth = 0,
): [number | undefined, number | undefined] | undefined {
    const [l, r] = num
    if (is_number(l) && is_number(r)) {
        if (depth >= 4) {
            return num as [number, number]
        } else {
            return undefined
        }
    }
    if (is_arr(l)) {
        const explosion = explode(l, depth + 1)
        if (explosion) {
            if (explosion === l) {
                num[0] = 0
            }
            if (explosion[1] !== undefined) {
                const rshard = explosion[1]
                if (is_arr(r)) {
                    add_into(r, rshard, true)
                } else {
                    num[1] = r + rshard
                }
            }
            // composite a new one so that parent can recognize the child
            return [explosion[0], undefined]
        }
    }
    if (is_arr(r)) {
        const explosion = explode(r, depth + 1)
        if (explosion) {
            if (explosion === r) {
                num[1] = 0
            }
            if (explosion[0] !== undefined) {
                const lshard = explosion[0]
                if (is_arr(l)) {
                    add_into(l, lshard, false)
                } else {
                    num[0] = l + lshard
                }
                explosion[0] = undefined
            }
            return [undefined, explosion[1]]
        }
    }
}

function split(num: SnailFish): boolean {
    const [l, r] = num
    if (is_number(l)) {
        if (l >= 10) {
            num[0] = [Math.floor(l / 2), Math.ceil(l / 2)]
            return true
        }
    } else {
        const splited = split(l)
        if (splited) {
            return splited
        }
    }
    if (is_number(r)) {
        if (r >= 10) {
            num[1] = [Math.floor(r / 2), Math.ceil(r / 2)]
            return true
        }
    } else {
        const splited = split(r)
        if (splited) {
            return splited
        }
    }
    return false
}

// type strArr = string | [strArr, strArr]

// function printSnailFish(pair: SnailFish): strArr {
//     const l = pair[0] instanceof Array
//         ? printSnailFish(pair[0])
//         : `${pair[0]}`
//     const r = pair[1] instanceof Array
//         ? printSnailFish(pair[1])
//         : `${pair[1]}`
//     return `[${l},${r}]`
// }

function magnitude(num: SnailFish): number {
    let result = 0
    if (is_arr(num[0])) {
        result += magnitude(num[0]) * 3
    } else {
        result += num[0] * 3
    }
    if (is_arr(num[1])) {
        result += magnitude(num[1]) * 2
    } else {
        result += num[1] * 2
    }
    return result
}

function add(a: SnailFish, b: SnailFish) {
    const combination: SnailFish = [deepcopy(a), deepcopy(b)]
    while (true) {
        const explosion = explode(combination)
        if (explosion !== undefined) {
            continue
        }
        const splited = split(combination)
        if (splited) {
            continue
        }
        break
    }
    return combination
}

async function main() {
    const pairs = aocInput(18).splitlines().map(it => Parser.parse(it))
    const addition = pairs.slice(1).reduce((a, b) => add(a, b), pairs[0])
    console.log(magnitude(addition))

    let max_magnitude = -Infinity
    for (let i = 0; i < pairs.length; i++) {
        for (let j = 0; j < pairs.length; j++) {
            if (i === j) {
                continue
            }
            max_magnitude = Math.max(
                max_magnitude, magnitude(add(pairs[i], pairs[j])))
        }
    }
    console.log(max_magnitude)
}

await main()

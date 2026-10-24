import HashMap from '@utils/HashMap'
import HashSet from '@utils/HashSet'
import { aocInput } from '@utils/iohelper'

type Point2D = number[]

const COST = [1, 10, 100, 1000] as const

class Situation {
    public constructor(
        public map: string[][],
        public amphipods: HashSet<Point2D>,
    ) {
        // this.records = [this.toString()]
    }

    // public records: string[]

    // private toString() {
    //     return this.map.map(it => it.join('')).join('\n')
    // }

    private copy() {
        const m = this.map.map(it => [...it])
        const a = new HashSet(this.amphipods.hasher)
        for (const amphipod of this.amphipods) {
            a.insert(amphipod)
        }
        return new Situation(m, a)
    }

    public move(f: Point2D, t: Point2D) {
        const c = this.copy()
        const original = c.get(f)
        c.set(f, '.')
        c.set(t, original)
        c.amphipods.remove(f)
        c.amphipods.insert(t)
        // c.records = [...this.records, c.toString()]
        return c
    }

    public isEmpty(p: Point2D) {
        return this.get(p) === '.'
    }

    public get(p: Point2D) {
        return this.map[p[0]][p[1]]
    }

    public set(p: Point2D, c: string) {
        this.map[p[0]][p[1]] = c
    }

    public finished() {
        for (let j = 0; j < 4; j++) {
            for (let i = 2; i < this.map.length - 1; i++) {
                if (this.get([i, 3 + j * 2])
                  !== String.fromCharCode('A'.charCodeAt(0) + j)) {
                    return false
                }
            }
        }
        return true
    }

    public sameAmphipods(p: Point2D, column: number, start_row: number = 1) {
        let is_same = true
        const tp = this.get(p)
        for (let i = start_row; is_same && (i < this.map.length - 1); i++) {
            if (this.get([i, column]) != tp) {
                is_same = false
            }
        }
        return is_same
    }
}

function targetCol(p: Point2D, s: Situation) {
    const amphipod_type = s.get(p)
    return 3
      + (amphipod_type.charCodeAt(0) - 'A'.charCodeAt(0)) * 2
}

function* movements(p: Point2D, s: Situation) {
    function* move_up(p: Point2D, s: Situation) {
        // In hallway, can't move up
        if (p[0] === 1) {
            return
        }
        // If in target column and the amphipods of the same type below, skip
        const tc = targetCol(p, s)
        if (tc === p[1]) {
            if (s.sameAmphipods(p, p[1], p[0] + 1)) {
                return
            }
        }
        let x = p[0]
        const y = p[1]
        // Can go up
        while (x !== 1) {
            if (s.get([--x, y]) !== '.') {
                return
            }
        }
        // Walk in the hallway
        for (const dy of [-1, 1]) {
            let ny = y + dy
            while (s.get([1, ny]) === '.') {
                const np = [1, ny]
                if (ny % 2 === 0 || ny === 1 || ny === 11) {
                    // Compose a new one
                    yield np
                }
                ny += dy
            }
        }
    }

    function* move_down(p: Point2D, s: Situation) {
        // Not in hallway, skip
        if (p[0] !== 1) {
            return
        }
        const tc = targetCol(p, s)
        const dy = tc > p[1] ? 1 : -1
        const np = [1, p[1] + dy] as Point2D
        while (s.isEmpty(np)) {
            if (np[1] === tc) {
                // In target column, below must be amphipods of the same type
                let last_empty = 1
                while (s.isEmpty([last_empty + 1, tc])) {
                    last_empty++
                    continue
                }
                // Fulled
                if (last_empty === 1) {
                    return
                }
                if (!s.sameAmphipods(p, tc, last_empty + 1)) {
                    return
                }
                yield [last_empty, tc]
            }
            np[1] += dy
        }
    }
    for (const np of move_up(p, s)) {
        yield np
    }
    for (const np of move_down(p, s)) {
        yield np
    }
}

function amphipods(row: number) {
    const result = new HashSet<Point2D>(p => `${p[0]},${p[1]}`)
    for (const j of [3, 5, 7, 9]) {
        for (let i = 0; i < row; i++) {
            result.insert([i + 2, j])
        }
    }
    return result
}

function organize(start: Situation) {
    const visited = new HashMap<Situation, number>(
        (s: Situation) => {
            return s.map.map(it => it.join('')).join('')
        })

    const q: [Situation, number][] = [[start, 0]]
    visited.set(start, 0)
    let min_cost = Infinity
    while (q.length) {
        const [s, cost] = q.shift()!
        // console.log(s.map.map(it => it.join('')).join('\n'))
        // console.log()
        if (s.finished()) {
            min_cost = Math.min(min_cost, cost)
            // for (const r of s.records) {
            //     console.log(r)
            //     console.log()
            // }
            continue
        }
        for (const p of s.amphipods) {
            for (const np of movements(p, s)) {
                const ns = s.move(p, np)
                const nc = cost
                  + COST[s.get(p).charCodeAt(0) - 'A'.charCodeAt(0)]
                  * (Math.abs(p[0] - np[0]) + Math.abs(p[1] - np[1]))
                if (!visited.has(ns) || nc < visited.get(ns)!) {
                    visited.set(ns, nc)
                    q.push([ns, nc])
                }
            }
        }
    }
    return min_cost
}

export async function main() {
    const data = aocInput(23).splitlines().map(it => [...it])

    const blueprint = new Situation(
        data,
        amphipods(2),
    )

    console.log(organize(blueprint))

    const unfolded = new Situation(
        [
            ...data.splice(0, 3),
            [...'  #D#C#B#A#'],
            [...'  #D#B#A#C#'],
            ...data,
        ],
        amphipods(4),
    )

    console.log(organize(unfolded))
}

await main()

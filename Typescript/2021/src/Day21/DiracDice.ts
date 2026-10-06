import { aocInput } from '@utils/iohelper'

type Player = {
    position: number
    score: number
}
type State = {
    p1: Player
    p2: Player
    turn: boolean
}

function copy_state(s: State): State {
    return {
        p1: copy_player(s.p1),
        p2: copy_player(s.p2),
        turn: s.turn,
    }
}

function copy_player(p: Player): Player {
    return {
        position: p.position,
        score: p.score,
    }
}

type HashMap = Map<string, { s: State, count: number }>

function hash(s: State) {
    return [
        s.p1.position,
        s.p1.score,
        s.p2.position,
        s.p2.score,
        s.turn,
    ].join(',')
}

function part1(p1: Player, p2: Player) {
    let dice = 1
    let turn = true
    let rolled = 0
    while (p1.score < 1000 && p2.score < 1000) {
        const p = turn ? p1 : p2
        for (let i = 0; i < 3; i++) {
            p.position = (p.position - 1 + dice) % 10 + 1
            dice = dice % 100 + 1
            rolled++
        }
        p.score += p.position
        turn = !turn
    }
    if (p1.score >= 1000) {
        console.log(p2.score * rolled)
    } else {
        console.log(p1.score * rolled)
    }
}

// 3*3*3
function* spawn(step: number = 0, depth: number = 1): Generator<number> {
    if (depth > 3) {
        yield step
        return
    }
    for (let i = 1; i <= 3; i++) {
        for (const s of spawn(step + i, depth + 1)) {
            yield s
        }
    }
}

export async function main() {
    const [p1, p2] = aocInput(21).splitlines()
        .map((it) => {
            const m = it.match(/position: (\d+)/)!
            return parseInt(m[1])
        })
    part1(
        { position: p1, score: 0 },
        { position: p2, score: 0 },
    )
    const states: HashMap = new Map()
    const initState: State = {
        p1: {
            position: p1,
            score: 0,
        },
        p2: {
            position: p2,
            score: 0,
        },
        turn: true,
    }
    const init_key = hash(initState)
    states.set(init_key, { s: initState, count: 1 })
    const wins: [number, number] = [0, 0]
    const spawns = [...spawn()]
    while (states.size) {
        const keys = states.keys().toArray()
        for (const key of keys) {
            const { s, count } = states.get(key)!
            states.delete(key)
            for (let roll = 0; roll < 27; roll++) {
                const ns = copy_state(s)
                const p = ns.turn ? ns.p1 : ns.p2
                p.position = (p.position - 1 + spawns[roll]) % 10 + 1
                p.score += p.position
                if (p.score >= 21) {
                    wins[ns.turn ? 0 : 1] += count
                    continue
                }
                ns.turn = !ns.turn
                const ns_key = hash(ns)
                states.getOrInsert(ns_key, { s: ns, count: 0 }).count += count
            }
        }
    }
    console.log(Math.max(...wins))
}

await main()

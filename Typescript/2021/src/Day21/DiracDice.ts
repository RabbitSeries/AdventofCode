import { HashMap } from '@utils/HashMap'
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
            return {
                position: parseInt(m[1]),
                score: 0,
            } as Player
        })
    part1(copy_player(p1), copy_player(p2))
    let states_set = new HashMap<State, number>(
        s => [
            s.p1.position,
            s.p1.score,
            s.p2.position,
            s.p2.score,
            s.turn,
        ].join(','))
    states_set.set({ p1: copy_player(p1), p2: copy_player(p2), turn: true }, 1)
    const wins: [number, number] = [0, 0]
    const spawns = [...spawn()]
    while (states_set.size) {
        const transferred = new HashMap<State, number>(states_set.hasher)
        for (const [s, count] of states_set) {
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
                transferred.upsert(ns, (_, c) => (c ?? 0) + count)
            }
        }
        states_set = transferred
    }
    console.log(Math.max(...wins))
}

await main()

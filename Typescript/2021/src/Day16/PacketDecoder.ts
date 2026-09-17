import { readFile } from 'node:fs/promises'

type Packet = {
    version: number
    type_id: number
    data: number | Packet[]
}

type Bits = (0 | 1)[]
function consume_and_decode(bits: Bits): Packet {
    const assert_lenth = (len: number) => {
        if (bits.length < len) {
            throw Error('Invalid data length')
        }
    }
    assert_lenth(6)
    const version = parseInt(bits.splice(0, 3).join(''), 2)
    const type_id = parseInt(bits.splice(0, 3).join(''), 2)
    if (type_id === 4) {
        let data: number = 0
        while (bits.length) {
            assert_lenth(5)
            const bin_digit = bits.splice(0, 5)
            data = data * 16 + parseInt(bin_digit.splice(1).join(''), 2)
            if (bin_digit[0] === 0) {
                break
            }
        }
        return {
            version,
            type_id,
            data,
        }
    } else {
        assert_lenth(1)
        const length_type_id = bits.splice(0, 1)[0]
        if (length_type_id === 0) {
            assert_lenth(15)
            const length = parseInt(bits.splice(0, 15).join(''), 2)
            assert_lenth(length)
            const sub_packets_bin = bits.splice(0, length)
            const sub_packets: Packet[] = []
            const op_packet = {
                version,
                type_id,
                data: sub_packets,
            }
            while (sub_packets_bin.length) {
                sub_packets.push(consume_and_decode(sub_packets_bin))
            }
            return op_packet
        } else {
            assert_lenth(11)
            const size = parseInt(bits.splice(0, 11).join(''), 2)
            const sub_packets: Packet[] = []
            for (let i = 0; i < size; i++) {
                sub_packets.push(consume_and_decode(bits))
            }
            return {
                version,
                type_id,
                data: sub_packets,
            }
        }
    }
}
enum TypeID {
    Sum = 0,
    Product = 1,
    Minimum = 2,
    Maximum = 3,
    Literal = 4,
    Gth = 5,
    Lth = 6,
    Eq = 7,
}

function part1(root: Packet) {
    const q: Packet[] = [root]
    let sum_version = 0
    while (q.length) {
        const p = q.shift()!
        sum_version += p.version
        if (p.type_id == TypeID.Literal) {
            continue
        }
        for (const np of p.data as Packet[]) {
            q.push(np)
        }
    }
    return sum_version
}

function part2(root: Packet): number {
    if (root.type_id == TypeID.Literal) {
        return root.data as number
    }
    const sub_packets = root.data as Packet[]
    const sub_packet_values = sub_packets.map(it => part2(it))
    if (root.type_id === TypeID.Sum) {
        return sub_packet_values.reduce((a, b) => a + b, 0)
    }
    if (root.type_id === TypeID.Product) {
        return sub_packet_values.reduce((a, b) => a * b, 1)
    }
    if (root.type_id === TypeID.Minimum) {
        return Math.min(...sub_packet_values)
    }
    if (root.type_id === TypeID.Maximum) {
        return Math.max(...sub_packet_values)
    }
    if (root.type_id === TypeID.Gth) {
        return +(sub_packet_values[0] > sub_packet_values[1])
    }
    if (root.type_id === TypeID.Lth) {
        return +(sub_packet_values[0] < sub_packet_values[1])
    }
    return +(sub_packet_values[0] === sub_packet_values[1])
}

async function main() {
    const data = await readFile('Day16/input.txt')
        .then(it => [...it.toString().trim()]
            .flatMap((it) => {
                const bits = [...parseInt(it, 16).toString(2)]
                    .map(bit => bit.charCodeAt(0) - '0'.charCodeAt(0))
                return [...Array.from({ length: 4 - bits.length }).fill(0),
                    ...bits] as (0 | 1)[]
            }))
    const packet = consume_and_decode(data)
    console.log(part1(packet))
    console.log(part2(packet))
}

main()

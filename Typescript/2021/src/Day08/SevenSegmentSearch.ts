import { readFile } from "fs/promises";
import { EOL } from "os";

const data = await readFile("Day08/input.txt")
    .then(data => data.toString().trim().split(`${EOL}`))
    .then(lines => lines.map(line => {
        const [signals, outputs] = line.split("|")
        return {
            singals: signals.trim().split(" "),
            outputs: outputs.trim().split(" ")
        }
    }))

type Data = typeof data

function countUnique(data: Data) {
    return data.map(v => v.outputs.reduce((prev, o) => (o.length === 2 || o.length === 3 || o.length === 4 || o.length === 7) ? prev + 1 : prev, 0)).reduce((a, b) => a + b)
}

console.log(countUnique(data))

function solveInput(signals: Set<string>[], outputs: Set<string>[]) {
    // The wire-to-segment connections for the digits 1, 4, 7, and 8,
    // use 2, 3, 4, and 7 segments respectively.
    // and assuming segments are indexed starting from a.
    const digits = Array.from({ length: 10 }, () => new Set<string>())
    for (const signal of signals) {
        if (signal.size === 2) {
            digits[1] = signal
        } else if (signal.size === 3) {
            digits[7] = signal
        } else if (signal.size === 4) {
            digits[4] = signal
        } else if (signal.size === 7) {
            digits[8] = signal
        }
    }
    const segments = Array.from({ length: 7 }, () => "")

    // Each digit uses the following number of wires: 6, 2, 5, 5, 4, 5, 6, 3, 7, 6.
    // The intersections of wires determine which wire lights up which segment.
    const segmentIdx = (ch: string) => ch.charCodeAt(0) - "a".charCodeAt(0)
    // Segment a: The digits 1 (2 segments) and 7 (3 segments) help identify the wire for segment a.
    segments[segmentIdx("a")] = digits[7].difference(digits[1]).keys().toArray()[0]

    // Digit 9:
    // Segment g: Three digits use 6 wires: digits 0, 6, and 9.
    // Digit 9 can be distinguished among them by checking whose wires include the 4 segments of digit 4.
    // Once that’s done, digit 9’s segments and the 6_th segment can be identified by
    // excluding segment a and the segments from digit 4, leaving the last segment (segment g).
    const digit06 = []
    for (const sixWiresSignal of signals.filter(signal => signal.size === 6)) {
        if (sixWiresSignal.isSupersetOf(digits[4])) {
            digits[9] = sixWiresSignal
            segments[segmentIdx("g")] = sixWiresSignal.difference(new Set([segments[segmentIdx("a")], ...digits[4]]))
                .keys().take(1).toArray()[0]
        } else {
            digit06.push(sixWiresSignal)
        }
    }

    // Digits 0 and 6:
    // Segment e: Digits 0 and 6 can be distinguished by whether their segments include those of digit 1.
    // This segment can be identified by excluding segment a, segment g,
    // and the segments from digit 4 from the segments of digit 4.
    for (const digit of digit06) {
        if (digit.intersection(digits[1]).size == 1) {
            digits[6] = digit
        } else {
            digits[0] = digit
        }
    }
    segments[segmentIdx("e")] = digits[6].difference(new Set([segments[segmentIdx("a")], segments[segmentIdx("g")], ...digits[4]]))
        .keys().take(1).toArray()[0]

    // Segments c and f: These can be found by excluding segment a, g and e, 
    // then removing the remaining segments from digit 4’s segments.
    segments[segmentIdx("c")] = digits[4].difference(
        digits[6].difference(
            new Set(
                [segments[segmentIdx("a")],
                segments[segmentIdx("g")],
                segments[segmentIdx("e")]]
            )
        )
    ).keys().toArray()[0]
    segments[segmentIdx("f")] = digits[1].difference(new Set([segments[segmentIdx("c")]])).keys().toArray()[0]

    // Segments b and d: After excluding the known segments from digit 6, segments b and d remain.
    // They can be distinguished because digit 0 only contains segment b.
    segments[segmentIdx("d")] = digits[6].difference(digits[0]).keys().toArray()[0]
    segments[segmentIdx("b")] = digits[4].difference(new Set([segments[segmentIdx("d")], ...digits[1]]))
        .keys().toArray()[0]

    // Then the remaining 5 wires signals are 2, 3, 5.
    // Only digit 2 doesn't contains segment f.
    // Then digit 3 contains segment c while digit 5 doesn't. 
    for (const digit of signals.filter(signal => signal.size === 5)) {
        if (!digit.has(segments[segmentIdx("f")])) {
            digits[2] = digit
        } else if (digit.has(segments[segmentIdx("c")])) {
            digits[3] = digit
        } else {
            digits[5] = digit
        }
    }
    const parsed = []
    for (const output of outputs) {
        for (let i = 0; i < 10; i++) {
            if (output.symmetricDifference(digits[i]).size === 0) {
                parsed.push(i)
            }
        }
    }
    return parseInt(parsed.join(""))
}

console.log(data.map(
    v => solveInput(
        v.singals.map(signal => new Set([...signal])),
        v.outputs.map(output => new Set([...output]))
    )
).reduce((a, b) => a + b, 0))

/* eslint-disable @stylistic/max-len */
import { aocInput } from '@utils/iohelper'

/**
 * | inp w | mul x | add x z | mod x | div z | add x | eql x w | eql x | mul y | add y | mul y x | add y | mul z y | mul y | add y w | add y | mul y x | add z y |
 * | :---- | :---- | :------ | :---- | :---- | :---- | :------ | :---- | :---- | :---- | :------ | :---- | :------ | :---- | :------ | :---- | :------ | :------ |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 * | w     | 0     | z       | 26    | b     | a     | w       | 0     | 0     | 25    | x       | 1     | y       | 0     | w       | c     | x       | y       |
 *
 * step 1: x = 0 if z % 26 + a == w else 1
 * step 2: z = z / b, b \in {1, 26}
 * step 3: y = 25 * x + 1 = 26 if x == 1 else 0
 * step 4: z = z * (25 * x + 1) = z * 26 if x == 1 else z
 * step 5: y = (w + c) * x == w + c if x == 1 else 0
 * step 6: z = z + y = z + w + c if x == 1 else z
 * That is:
 * If x == 1, b == 1   : z = z * 26 + w + c
 * If x == 1, b == 26  : z = z / 26 * 26 + w + c
 * If x == 0, b == 1   : z = z
 * If x == 0, b == 26  : z = z / 26 * 26
*/

// For the final z to be 0, the algorithm can be reverted.

const data = aocInput(24).splitlines()
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

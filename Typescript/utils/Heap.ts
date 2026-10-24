export class Heap<T> {
    // cmp(child, parent) > 0
    constructor(private cmp: (i: T, j: T) => number) { }

    public get length(): number {
        return this.element.length
    }

    private element: T[] = []

    public static from<T>(iterable: Iterable<T>, cmp: (i: T, j: T) => number) {
        const h = new Heap<T>(cmp)
        for (const e of iterable) {
            h.push(e)
        }
        return h
    }

    public push(e: T) {
        this.element.push(e)
        this.sift_up(this.element.length - 1)
    }

    public pop(): T | undefined {
        if (this.element.length) {
            if (this.element.length > 2) {
                const top = this.element[0]
                this.element[0] = this.element.pop()!
                this.sift_down()
                return top
            } else {
                return this.element.shift()
            }
        } else {
            return undefined
        }
    }

    private swap(i: number, j: number) {
        if (i == j) {
            return
        }
        [this.element[i], this.element[j]] = [this.element[j], this.element[i]]
    }

    private sift_up(i: number) {
        while (i > 0) {
            const p = Math.floor((i + 1) / 2 - 1)
            if (this.cmp(this.element[i], this.element[p]) > 0) {
                break
            } else {
                this.swap(i, p)
                i = p
            }
        }
    }

    private sift_down(i: number = 0) {
        while (i < this.element.length) {
            const [l, r] = [2 * i + 1, 2 * i + 2]
            let min_id = i
            if (l < this.element.length
              && this.cmp(this.element[l], this.element[min_id]) <= 0) {
                min_id = l
            }
            if (r < this.element.length
              && this.cmp(this.element[r], this.element[min_id]) <= 0) {
                min_id = r
            }
            if (i !== min_id) {
                this.swap(i, min_id)
                i = min_id
            } else {
                break
            }
        }
    }
}

export default Heap

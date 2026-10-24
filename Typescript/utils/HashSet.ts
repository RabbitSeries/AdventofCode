export class HashSet<K> {
    private m = new Map<string, K>()

    public get(key: K): K | undefined {
        return this.m.get(this._hasher(key))
    }

    public has(t: K) {
        return this.m.has(this._hasher(t))
    }

    public insert(k: K) {
        return this.m.set(this._hasher(k), k)
    }

    public remove(k: K) {
        return this.m.delete(this._hasher(k))
    }

    public get size(): number {
        return this.m.size
    }

    [Symbol.iterator]() {
        return this.m.values()
    }

    private _hasher: (k: K) => string

    public get hasher() {
        return this._hasher
    }

    public constructor(hasher: (k: K) => string) {
        this._hasher = hasher
    }
}

export default HashSet

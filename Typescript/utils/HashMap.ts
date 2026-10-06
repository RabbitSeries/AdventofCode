export class HashMap<K, V> {
    private m = new Map<string, { k: K, v: V }>()

    public get(key: K): V | undefined {
        return this.m.get(this.hasher(key))?.v
    }

    public has(t: K) {
        return this.m.has(this.hasher(t))
    }

    public set(k: K, v: V) {
        return this.m.set(this.hasher(k), { k, v })
    }

    public remove(k: K) {
        return this.m.delete(this.hasher(k))
    }

    public upsert(
        k: K,
        value: (k: K, v: V | undefined) => V | undefined) {
        const _v = this.m.get(this.hasher(k))
        const v = value(k, _v?.v)
        if (v !== undefined) {
            this.m.set(this.hasher(k), { k, v })
        }
    }

    public get size(): number {
        return this.m.size
    }

    [Symbol.iterator]() {
        return this.m.values().map(it => [it.k, it.v] as [K, V])
    }

    public constructor(public hasher: (k: K) => string) { }
}

export default HashMap

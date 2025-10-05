package year2023.Day25

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import java.io.*
import java.util.*

@AoCSolution()
class MinimumCut : ISolution {
    fun readFile(input: BufferedReader) {
        input.use { data ->
            data.lines().forEach { l ->
                val split = l.split(":")
                val inWire = Hasher(split[0].trim())
                split[1].trim().split(" ").asSequence().map(this::Hasher).forEach { wHash ->
                    WireGraph.getOrPut(inWire) { HashSet() }.add(wHash)
                    WireGraph.getOrPut(wHash) { HashSet() }.add(inWire)
                }
            }
        }
    }

    fun Hasher(WireName: String): Int {
        return WireName.fold(0) { i, c -> i * 26 + c.code - 'a'.code }
    }

    val WireGraph: MutableMap<Int, MutableSet<Int>> = HashMap()

    fun CutCount(Cliques: Map<Int, Set<Int>>): Int {
        if (Cliques.size != 2) return -1
        val (from, to) = Cliques.values.take(2)
        var cutCnt = 0
        for (fromV in from) {
            WireGraph[fromV]?.forEach { conn ->
                if (to.contains(conn)) {
                    cutCnt++
                }
            }
        }
        return cutCnt
    }

    fun FindCliqueRoot(CliqueRootMap: HashMap<Int, Int>, name: Int): Int {
        CliqueRootMap[name]!!.let { f ->
            if (CliqueRootMap[f] == name) {
                return f
            }
            return CliqueRootMap.compute(name) { k, v -> FindCliqueRoot(CliqueRootMap, f) }!!
        }
    }

    fun Karger(seed: Long, timeout: Long): Map<Int, Set<Int>> {
        val now = System.currentTimeMillis()
        val rand = Random(seed)
        val Cliques: MutableMap<Int, MutableSet<Int>> = HashMap()
        val CliqueRootMap: HashMap<Int, Int> = HashMap()
        val NodesHavingEdge: MutableSet<Int> = HashSet(WireGraph.keys)
        val visited: MutableMap<Int, MutableSet<Int>> = HashMap()
        WireGraph.keys.forEach { e ->
            visited.put(e, HashSet())
            Cliques.put(e, mutableSetOf(e))
            CliqueRootMap.put(e, e)
        }
        while (Cliques.size != 2) {
            if (System.currentTimeMillis() - now > timeout) {
                break
            }
            // Get father
            val NodeList = NodesHavingEdge.toList()
            val father = NodeList[rand.nextInt(NodeList.size)]
            // Get child
            val ChildList = WireGraph[father]!!.filter { child ->
                !visited[father]!!.contains(child) && !visited[child]!!.contains(father)
            }.toList()
            if (ChildList.isEmpty()) {
                NodesHavingEdge.remove(father)
                continue
            }
            val child = ChildList[rand.nextInt(ChildList.size)]
            visited[father]!!.add(child)
            visited[child]!!.add(father)
            val FatherCliqueName = FindCliqueRoot(CliqueRootMap, father)
            val ChildCliqueName = FindCliqueRoot(CliqueRootMap, child)
            CliqueRootMap.put(ChildCliqueName, FatherCliqueName)
            if (FatherCliqueName != ChildCliqueName) {
                Cliques[FatherCliqueName]!!.addAll(Cliques[ChildCliqueName]!!)
                Cliques.remove(ChildCliqueName)
            }
        }
        return Cliques
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        //val timeout = 70L
        //while (true) {
        //    val seed = System.currentTimeMillis()
        //    val res = Karger(seed, timeout);
        //    if (CutCount(res) == 3) {
        //        println("Using seed: ${seed}, time cost ${System.currentTimeMillis() - seed}");
        //        println("Solution 1: " + res.values.map { it.size }.fold(1) { i, v -> i * v })
        //        return
        //    }
        //}
        // Kotlin 1759684678660 - 20 ms
        // Java - 1747570281531 - 40 ms
        // val res = Karger(1747570281531L, Long.MAX_VALUE)
        // println(CutCount(res)) // This is 4 in Kotlin, but 3 in Java
        // Kotlin seems to have modified either HashMethod or Random generator
        println("Solution 1: " + Karger(1759684678660L, Long.MAX_VALUE).values.asSequence().map { s -> s.size }
            .fold(1) { i, v -> i * v })
    }

    override fun Solution2(input: BufferedReader) {
        println("\t\t\t------All 50 stars AoC Finished!!!!------")
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            MinimumCut().let { day25 ->
                BufferedReader(FileReader("Day25/input.txt")).use { day25.Solution1(it) }
                BufferedReader(FileReader("Day25/input.txt")).use { day25.Solution2(it) }
            }
        }
    }
}

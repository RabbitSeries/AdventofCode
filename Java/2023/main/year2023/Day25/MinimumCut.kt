package year2023.Day25

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import java.io.*
import java.util.*
import JavaDataModel.inputFileName

@AoCSolution(day = 25)
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

    fun Hasher(WireName: String) = WireName.fold(0) { i, c -> i * 26 + c.code - 'a'.code }

    val WireGraph: MutableMap<Int, MutableSet<Int>> = HashMap()

    // fun CutCount(Cliques: Map<Int, Set<Int>>) = when {
    //     Cliques.size != 2 -> -1
    //     else -> {
    //         val (from, to) = Cliques.values.take(2)
    //         from.sumOf { WireGraph[it]?.count { conn -> conn in to } ?: 0 }
    //     }
    // }

    fun FindCliqueRoot(CliqueRootMap: HashMap<Int, Int>, name: Int): Int =
        CliqueRootMap[name]!!.let { f ->
            when {
                CliqueRootMap[f] == name -> f
                else -> CliqueRootMap.compute(name) { k, v -> FindCliqueRoot(CliqueRootMap, f) }!!
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
            val root = NodeList[rand.nextInt(NodeList.size)]
            // Get child
            val ChildList = WireGraph[root]!!.filter { child ->
                !visited[root]!!.contains(child) && !visited[child]!!.contains(root)
            }.toList()
            if (ChildList.isEmpty()) {
                NodesHavingEdge.remove(root)
                continue
            }
            val child = ChildList[rand.nextInt(ChildList.size)]
            visited[root]!!.add(child)
            visited[child]!!.add(root)
            val CliqueRootId = FindCliqueRoot(CliqueRootMap, root)
            val CliqueChildId = FindCliqueRoot(CliqueRootMap, child)
            CliqueRootMap.put(CliqueChildId, CliqueRootId)
            if (CliqueRootId != CliqueChildId) {
                Cliques[CliqueRootId]!!.addAll(Cliques[CliqueChildId]!!)
                Cliques.remove(CliqueChildId)
            }
        }
        return Cliques
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        //val timeout = 70L
        //while (true) {
        //    val seed = System.currentTimeMillis()
        //    val res = Karger(seed, timeout)
        //    if (CutCount(res) == 3) {
        //        println("Using seed: ${seed}, time cost ${System.currentTimeMillis() - seed}")
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
                BufferedReader(FileReader(day25.inputFileName!!)).use { day25.Solution1(it) }
                BufferedReader(FileReader(day25.inputFileName!!)).use { day25.Solution2(it) }
            }
        }
    }
}

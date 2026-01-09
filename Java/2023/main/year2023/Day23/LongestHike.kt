package year2023.Day23

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import java.io.BufferedReader
import java.io.File
import java.util.LinkedList
import java.util.Queue
import kotlin.math.max

private typealias Point2D = Pair<Int, Int>

@AoCSolution(day = 23)
class LongestHike : ISolution {

    enum class Cell(val value: Int) {
        PATH(-2),
        FOREST(-1),
        UP(0),
        RIGHT(1),
        DOWN(2),
        LEFT(3)
    }

    lateinit var Start: Point2D
    lateinit var Dest: Point2D


    val dx = arrayOf(-1, 0, 1, 0)

    val dy = arrayOf(0, 1, 0, -1)

    // lateinit cannot decorate primitive types
    var rows: Int = 0
    var cols: Int = 0

    lateinit var IceIsland: List<List<Cell>>

    fun readFile(input: BufferedReader) {
        IceIsland = input.lineSequence().map { line ->
            line.trim().asSequence().map {
                when (it) {
                    '#' -> Cell.FOREST
                    '^' -> Cell.UP
                    '>' -> Cell.RIGHT
                    'v' -> Cell.DOWN
                    '<' -> Cell.LEFT
                    else -> Cell.PATH
                }
            }.toList()
        }.toList()
        Start = 0 to 1
        Dest = (IceIsland.size - 1) to (IceIsland[0].size - 2)
        rows = IceIsland.size
        cols = IceIsland[0].size
    }

    private fun Point2D.isValid() = first >= 0 && second >= 0 && first < rows && second < cols

    private fun Point2D.getNext(direction: Int) = first + dx[direction] to second + dy[direction]

    private fun Point2D.block() = IceIsland[first][second]

    data class Path(val start: Point2D, val end: Point2D, val len: Int)

    val OutPath = mutableMapOf<Point2D, MutableSet<Path>>()
    val UndirectedNetwork = mutableMapOf<Point2D, MutableSet<Path>>()
    val InDegree = mutableMapOf<Point2D, Int>()

    fun BuildDirectedNetwork() {
        val q = ArrayDeque(listOf(Path(Start, Start, 0)))
        val visited: MutableSet<Point2D> = mutableSetOf(Start)
        while (q.isNotEmpty()) {
            val (curStart, curEnd, curLen) = q.removeFirst()
            for (i in 0..3) {
                val nextPos = curEnd.getNext(i)
                // Leave GetBlock(nextPos) == FOREST || != i. Leave != i because it is another path's endian, don't mark them as visited
                if (nextPos.isValid() && (nextPos.block() == Cell.PATH || nextPos.block().value == i) && nextPos !in visited) {
                    // This can also be processed before for-loop search, as there is no two paths intersected before the joint.
                    visited += nextPos
                    if (nextPos.block() == Cell.PATH || curLen == 0) {// Next block is PATH or a joint exit
                        if (nextPos == Dest) {
                            // Add curPath ->StartPos - HashSet()
                            OutPath.getOrPut(curStart) { mutableSetOf() } += Path(curStart, nextPos, curLen + 1)
                            InDegree.merge(nextPos, 1, Int::plus)
                        } else {
                            q += Path(curStart, nextPos, curLen + 1)
                        }
                    } else if (nextPos.block().value == i && curLen > 0) {// Next block is the entrance of a joint
                        val joint = nextPos.getNext(i)
                        visited += joint// Mark the generated position as visited
                        OutPath.getOrPut(curStart) { mutableSetOf() } += Path(curStart, joint, curLen + 2)
                        InDegree.merge(joint, 1, Int::plus)
                        q += Path(joint, joint, 0)
                    }
                }
            }
        }
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        BuildDirectedNetwork()
        val Dist: MutableMap<Point2D, Int> = mutableMapOf(Start to 0)
        val topoQ: Queue<Point2D> = LinkedList(listOf(Start))
        while (topoQ.isNotEmpty()) {
            val curPos = topoQ.poll()
            OutPath[curPos]?.forEach { p ->
                UndirectedNetwork.getOrPut(curPos, ::mutableSetOf) += p
                UndirectedNetwork.getOrPut(p.end, ::mutableSetOf) += Path(p.end, curPos, p.len)
                Dist.merge(p.end, Dist.getValue(p.start) + p.len, ::max)
                if (InDegree.merge(p.end, -1, Int::plus) == 0) {
                    topoQ += p.end
                }
            }

        }
        println("Solution 1: ${Dist[Dest]}")
    }

    fun DFS(curPos: Point2D, visited: MutableSet<Point2D>, curPathLen: Int): Int {
        var res = curPathLen
        for (p in UndirectedNetwork[curPos]!!) {
            val nextEnd = p.end
            if (nextEnd !in visited) {
                visited += nextEnd
                res = max(res, DFS(nextEnd, visited, curPathLen + p.len))
                visited.remove(nextEnd)
            }
        }
        return res
    }

    override fun Solution2(input: BufferedReader) {
        println("Solution 2: ${DFS(Start, mutableSetOf(Start), 0)}")
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            LongestHike().apply {
                File("Day23/input.txt").let {
                    Solution1(it.bufferedReader())
                    Solution2(it.bufferedReader())
                }
            }
        }
    }
}

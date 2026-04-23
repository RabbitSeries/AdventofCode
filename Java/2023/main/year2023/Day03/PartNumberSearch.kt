package year2023.Day03

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import java.io.*

typealias Point2D = Pair<Int, Int>

@AoCSolution(day = 3)
class PartNumberSearch : ISolution {
    lateinit var schematic: List<String>

    val GearPositions: MutableList<Point2D> = ArrayList()
    val NumPositions: HashMap<Point2D, Int> = HashMap()

    var rows = 0
    var cols = 0

    fun readFile(input: BufferedReader) {
        schematic = input.use { it.readLines() }
        rows = schematic.size
        cols = schematic[0].length
        schematic.indices.forEach { i ->
            var num: Point2D? = null
            schematic[i].indices.forEach { j ->
                val ch = schematic[i][j]
                if (ch.isDigit()) {
                    if (num == null) {
                        num = Point2D(i, j)
                    }
                    NumPositions.compute(num) { _, v -> v?.let { "${it}${ch}".toInt() } ?: ch.digitToInt() }
                } else {
                    if (num != null) {
                        num = null
                    }
                    if (ch == '*') {
                        GearPositions.add(Point2D(i, j))
                    }
                }

            }

        }
    }

    // up down left right lu ld ru rd
    val dx: IntArray = intArrayOf(-1, 1, 0, 0, -1, 1, -1, 1)
    val dy: IntArray = intArrayOf(0, 0, -1, 1, -1, -1, 1, 1)

    fun isValid(coordinate: Point2D): Boolean {
        return coordinate.first in 0 until rows && coordinate.second in 0 until cols
    }


    override fun Solution1(input: BufferedReader) {
        readFile(input)
        println("Solution 1: " + NumPositions.asSequence().filter { (pos, num) ->
            num.toString().indices.any { i ->
                val curPos = Point2D(pos.first, pos.second + i)
                IntRange(0, 7).any { direction ->
                    Point2D(
                        curPos.first + dx[direction],
                        curPos.second + dy[direction]
                    ).let { nextPos ->
                        isValid(
                            nextPos
                        ) && schematic[nextPos.first][nextPos.second].let { !it.isDigit() && it != '.' }
                    }
                }
            }
        }.sumOf { it.value })
    }

    fun searchNumber(cols: Int, pos: Point2D, visited: MutableSet<Point2D>): Int {
        var startIndex = pos.second
        var endIndex = pos.second
        val x = pos.first
        var nextPos = Point2D(x, startIndex - 1)
        // Search before
        while (nextPos.second >= 0 && schematic[x][nextPos.second].isDigit()) {
            startIndex--
            visited.add(nextPos)
            nextPos = Point2D(x, startIndex - 1)
        }
        // Search forward
        nextPos = Point2D(x, endIndex + 1)
        while (nextPos.second < cols && schematic[x][nextPos.second].isDigit()) {
            endIndex++
            visited.add(nextPos)
            nextPos = Point2D(x, endIndex + 1)
        }
        return schematic[x].substring(startIndex, endIndex + 1).toInt()
    }

    override fun Solution2(input: BufferedReader) {
        println("Solution 2: " + GearPositions.asSequence().map { pos ->
            val nums: MutableList<Int> = ArrayList()
            val visited: MutableSet<Point2D> = HashSet()
            for (i in 0 until 8) {
                val nextPos = Point2D(pos.first + dx[i], pos.second + dy[i])
                if (isValid(nextPos) && schematic[nextPos.first][nextPos.second].isDigit()) {
                    if (!visited.contains(nextPos)) {
                        visited.add(nextPos)
                        nums.add(searchNumber(cols, nextPos, visited))
                    }
                }
            }
            nums
        }.filter { it.size == 2 }.sumOf { it[0] * it[1] })
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            val solution = PartNumberSearch()
            BufferedReader(FileReader("Day03/input.txt")).use { solution.Solution1(it) }
            BufferedReader(FileReader("Day03/input.txt")).use { solution.Solution2(it) }
        }
    }
}

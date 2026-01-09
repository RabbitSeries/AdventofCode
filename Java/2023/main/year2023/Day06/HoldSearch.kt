package year2023.Day06

import java.io.*

import JavaDataModel.ISolution
import JavaDataModel.AoCSolution

@AoCSolution(day = 6)
class HoldSearch : ISolution {
    lateinit var recordList: List<Pair<Long, Long>>

    fun readFile(input: BufferedReader) {
        val lines = input.lineSequence().toList()
        val TimeList = lines[0].substringAfter(":").trim().splitToSequence(Regex("\\s+")).map { it.toLong() }.iterator()
        val DistanceList = lines[1].substringAfter(":").trim().split(Regex("\\s+")).map { it.toLong() }.iterator()
        // recordList = generateSequence {
        //     when {
        //         TimeList.hasNext() && DistanceList.hasNext() -> Pair(TimeList.next(), DistanceList.next())
        //         else -> null // null auto terminate the sequence
        //     }
        // }.toList()
        recordList = sequence {
            while (TimeList.hasNext() && DistanceList.hasNext()) {
                yield(Pair(TimeList.next(), DistanceList.next()))
            }
        }.toList()
    }

    fun binarySearch(totalTime: Long, distance: Long): Long {
        var s = 1L
        var t = totalTime / 2
        while (s != t) {
            val mid = (s + t) / 2
            val value = (totalTime - mid) * mid
            if (value >= distance) {
                t = mid
            } else {
                s = mid + 1
            }
        }
        return (totalTime / 2 - s + 1) * 2 - (if (totalTime % 2 == 0L) 1L else 0L)
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        println("Solution 1: " + recordList.asSequence().map { (k, v) ->
            binarySearch(k, v)
        }.fold(1L) { a, b -> a * b })
    }

    override fun Solution2(input: BufferedReader) {
        val totalTime = recordList.asSequence().joinToString("") { it.first.toString() }.toLong()
        val distance = recordList.asSequence().joinToString("") { it.second.toString() }.toLong()
        println("Solution 2: " + binarySearch(totalTime, distance))
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            HoldSearch().apply {
                BufferedReader(FileReader("Day06/input.txt")).use(::Solution1)
                BufferedReader(FileReader("Day06/input.txt")).use(::Solution2)
            }
        }
    }
}

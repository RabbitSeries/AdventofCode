package year2023.Day05

import java.io.*

import JavaDataModel.ISolution
import JavaDataModel.AoCSolution

typealias Interval = Pair<Long, Long>

@AoCSolution()
class LocationCascadingMapper : ISolution {
    lateinit var Seeds: List<Long>

    val MapBlocks: MutableList<List<Triple<Long, Long, Long>>> = mutableListOf()

    // there will be only one level stack for this search, no need further process of returned value, apply tail recursion optimize
    tailrec fun SearchMap(source: Long, level: Int): Long {
        if (level >= MapBlocks.size) {
            return source
        }
        val nextSourceOrCurrent = MapBlocks[level].firstNotNullOfOrNull { (from, to, range) ->
            if (source in from.until(from + range)) source - from + to else null
        } ?: source
        return SearchMap(nextSourceOrCurrent, level + 1)
    }

    fun readFile(input: BufferedReader) {
        input.lineSequence().joinToString("\n").splitToSequence("\n\n").forEach { block ->
            when {
                block.startsWith("seeds:") -> Seeds = block.substringAfter(":").trim()
                    .splitToSequence(Regex("\\s+")).map { it.toLong() }.toList()

                else ->
                    MapBlocks += block.lineSequence().filterNot { it.endsWith("map:") }
                        .map {
                            it.splitToSequence(Regex("\\s+")).map(String::toLong).toList()
                                .let { (to, from, range) -> Triple(from, to, range) }
                        }.toList()
            }
        }
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        // minBy returns T, minOf returns R
        println("Solution 1: ${Seeds.minOf { token -> SearchMap(token, 0) }}")
    }

    tailrec fun SearchInterval(level: Int, itvList: List<Pair<Long, Long>>): List<Pair<Long, Long>> {
        if (level >= MapBlocks.size) {
            return itvList
        }
        val nextItvList: List<Pair<Long, Long>> = itvList.asSequence().flatMap { itv ->
            val matchedList: MutableList<Pair<Long, Long>> = ArrayList()
            var unmatchedList: MutableList<Pair<Long, Long>> = mutableListOf(itv)
            for ((from, to, range) in MapBlocks[level]) {
                if (unmatchedList.isEmpty()) {
                    break
                }
                val fromEnd = from + range
                unmatchedList.fold(mutableListOf<Interval>() to mutableListOf<Interval>()) { (mAcc, unMAcc), (itvStart, itvEnd) ->
                    // coerceAtLeast set a lower bound for both, is Max. Ehh hard to understand
                    // val mapped = Pair(itvStart.coerceAtLeast(from), itvEnd.coerceAtMost(fromEnd))
                    val mapped = maxOf(itvStart, from) to minOf(itvEnd, fromEnd)
                    if (mapped.first >= mapped.second) {
                        unMAcc += itvStart to itvEnd
                    } else {
                        mAcc += Pair(mapped.first - from + to, mapped.second - from + to)
                        // *----|------------|----*
                        // *----|------*-----|----
                        // -----|------*-----|----*
                        if (itvStart < from) {
                            unMAcc += itvStart to from
                        }
                        if (itvEnd > fromEnd) {
                            unMAcc += fromEnd to itvEnd
                        }
                    }
                    mAcc to unMAcc
                }.apply {
                    unmatchedList = second
                    matchedList += first
                }
            }
            (matchedList + unmatchedList).asSequence()
        }.toList()
        return SearchInterval(level + 1, nextItvList)
    }

    override fun Solution2(input: BufferedReader) {
        // val f: (Long, Long) -> Long = Long::coerceAtMost // Eh, IDEA doesn't provide auto-completion on this
        println("Solution 2: " + Seeds.asSequence().chunked(2).flatMap { (begin, range) ->
            SearchInterval(0, listOf(begin to begin + range)).asSequence().map { it.first }
        }.min())
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            LocationCascadingMapper().apply {
                BufferedReader(FileReader("Day05/input.txt")).use(::Solution1)
                BufferedReader(FileReader("Day05/input.txt")).use(::Solution2)
            }
        }
    }
}

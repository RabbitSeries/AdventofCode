package year2023.Day09;

import JavaDataModel.*;
import java.io.BufferedReader

@AoCSolution(day = 9)
class HistoryExtrapolate : ISolution {
    lateinit var HistoryLists: List<List<Int>>

    fun predict(historyList: List<Int>, isForward: Boolean): Int {
        if (historyList.stream().allMatch { l -> l == 0 }) {
            return 0
        }
        val nextLevelList = (1 until historyList.size).map { i -> historyList[i] - historyList[i - 1] }
        return (if (isForward) historyList.last() else historyList.first()) + (if (isForward) 1 else -1) * predict(
            nextLevelList,
            isForward
        )
    }

    override fun Solution1(input: BufferedReader) {
        HistoryLists = input.lineSequence().map { line ->
            line.splitToSequence("""\s+""".toRegex()).map { it.toInt() }.toList()
        }.toList()
        println("Solution 1: ${HistoryLists.asSequence().sumOf { l -> predict(l, true) }}")
    }

    override fun Solution2(input: BufferedReader) {
        println("Solution 2: ${HistoryLists.asSequence().sumOf { l -> predict(l, false) }}")
    }
}

package year2023.Day04;

import java.io.*;

import JavaDataModel.ISolution;
import JavaDataModel.AoCSolution;
import kotlin.math.pow

@AoCSolution()
class CardPoints : ISolution {
    fun readFile(input: BufferedReader) {
        CardsList = input.lineSequence().map { line ->
            val cards = line.substringAfter(':').split("|")
            cards[0].trim().split("\\s+".toRegex()).map { it.toInt() }.toSet() to cards[1].trim()
                .split("\\s+".toRegex()).map { it.toInt() }.toList()
        }.toList()
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        WinResultList = CardsList.map { (k, v) -> v.asSequence().filter { k.contains(it) }.count() }
        println("Solution 1: " + WinResultList.asSequence().filter { it > 0 }.sumOf { 2.0.pow(it - 1).toInt() });
    }

    override fun Solution2(input: BufferedReader) {
        val dp = WinResultList.asSequence().map { 1 }.toMutableList()
        dp.indices.forEach { i ->
            val bonus = WinResultList[i]
            for (j in IntRange(i + 1, (i + bonus).coerceAtMost(dp.size - 1))) {
                dp[j] = dp[j] + dp[i]
            }
        }
        println("Solution 2: " + dp.sum());
    }

    lateinit var CardsList: List<Pair<Set<Int>, List<Int>>>

    lateinit var WinResultList: List<Int>

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            CardPoints().apply {
                BufferedReader(FileReader("Day04/input.txt")).use { Solution1(it) }
                BufferedReader(FileReader("Day04/input.txt")).use { Solution2(it) }
            }
        }
    }
}

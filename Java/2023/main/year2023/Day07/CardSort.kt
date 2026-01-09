package year2023.Day07

import java.io.*

import JavaDataModel.ISolution
import JavaDataModel.AoCSolution

import java.util.Comparator.comparing

@AoCSolution(day = 7)
class CardSort : ISolution {
    lateinit var cardBits: MutableList<Pair<String, Int>>

    companion object {
        // Compile time const, C++ concept?
        const val Rule1Cards = "AKQJT98765432"
        const val Rule2Cards = "AKQT98765432J"

        @JvmStatic
        fun main(args: Array<String>) {
            CardSort().apply {
                BufferedReader(FileReader("Day07/input.txt")).use(::Solution1)
                BufferedReader(FileReader("Day07/input.txt")).use(::Solution2)
            }
        }
    }


    enum class Priority {
        HighCar, OnePair, TwoPair, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind
    }

    fun cardHoldCompare(hold: String, rule: String): Int {
        return hold.asSequence().fold(0) { a, b -> a * rule.length + rule.indexOf(b) }
    }

    fun rule1Priority(str: String): Priority {
        val maxSameChCnt = str.asSequence().groupBy { it }.values.map { it.size }.fold(0) { acc, i -> maxOf(acc, i) }
        return when (maxSameChCnt) {
            1 -> Priority.HighCar
            2 -> if (str.asSequence().groupBy { it }.values.count { it.size == 2 } == 2)
                Priority.TwoPair else Priority.OnePair

            3 -> if (str.asSequence().groupBy { it }.values.any { it.size == 2 })
                Priority.FullHouse else Priority.ThreeOfAKind

            else -> Priority.entries[maxSameChCnt + 1]
        }
    }

    fun wildCards(str: String): Priority {
        val wildCardCnt = str.count { it == 'J' }
        if (wildCardCnt == 0) {
            return rule1Priority(str)
        }
        val sames = str.asSequence().filter { it != 'J' }.groupBy { it }.values
        val maxSameChCnt = sames.maxOfOrNull { it.size } ?: 0
        val totalSameChCnt = wildCardCnt + maxSameChCnt
        return when (totalSameChCnt) {
            2 -> Priority.OnePair
            3 -> when (maxSameChCnt) {
                1 -> Priority.ThreeOfAKind
                2 -> when (sames.count { it.size == 2 }) {
                    1 -> Priority.ThreeOfAKind
                    2 -> Priority.FullHouse
                    else -> throw ExceptionInInitializerError("Unexpected Total SameCh Cnt")
                }

                else -> throw ExceptionInInitializerError("Unexpected Total SameCh Cnt")
            }
            // totalSameChCnt == 4 , 1+3 (1) , 2+2 (1), 3+1(1)
            4 -> Priority.FourOfAKind
            5 -> Priority.FiveOfAKind
            else -> throw ExceptionInInitializerError("Unexpected Total SameCh Cnt")
        }
    }

    fun readFile(input: BufferedReader) {
        cardBits = input.lineSequence().map { it.trim().split("\\s+".toRegex()) }
            .map { it[0] to it[1].toInt() }.toMutableList()
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        cardBits.sortWith(comparing<Pair<String, Int>, Priority> { rule1Priority(it.first) }
            .thenComparing(comparing<Pair<String, Int>, Int> { cardHoldCompare(it.first, Rule1Cards) }.reversed()))
        println("Solution 1: " + cardBits.mapIndexed { i, it -> it.second * (i + 1) }.sum())
    }

    override fun Solution2(input: BufferedReader) {
        cardBits.sortWith(comparing<Pair<String, Int>, Priority> { wildCards(it.first) }
            .thenComparing(comparing<Pair<String, Int>, Int> { cardHoldCompare(it.first, Rule2Cards) }.reversed()))
        println("Solution 2: " + cardBits.mapIndexed { i, it -> it.second * (i + 1) }.sum())
    }
}

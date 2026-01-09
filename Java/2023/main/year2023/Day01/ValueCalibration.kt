package year2023.Day01

import JavaDataModel.*
import java.io.BufferedReader
import java.io.FileReader

@AoCSolution(day = 1)
class ValueCalibration : ISolution {
    private lateinit var mapList: List<String>

    private fun findDigit(line: String, index: Int, reverse: Boolean, additional: Map<String, Int>?): Int? {
        // Check additional word mappings
        additional?.forEach { (digit, value) ->
            val matches = if (!reverse) {
                index + 1 >= digit.length && line.substring(index + 1 - digit.length, index + 1) == digit
            } else {
                index + digit.length <= line.length && line.substring(index, index + digit.length) == digit
            }
            if (matches) return value
        }

        // Check numeric digit
        return if (line[index].isDigit()) {
            line[index].digitToInt()
        } else {
            null
        }
    }

    private fun dualSearch(additional: Map<String, Int>? = null): Int {
        return mapList.sumOf { line ->
            var firstDigit: Int? = null
            var lastDigit: Int? = null

            // Find first digit
            for (i in line.indices) {
                firstDigit = findDigit(line, i, false, additional)
                if (firstDigit != null) break
            }

            // Find last digit
            for (i in line.length - 1 downTo 0) {
                lastDigit = findDigit(line, i, true, additional)
                if (lastDigit != null) break
            }
            (firstDigit ?: 0) * 10 + (lastDigit ?: 0)
        }
    }

    override fun Solution1(input: BufferedReader) {
        mapList = input.use { it.lines().filter { line -> line.length > 1 }.toList() }
        println("Solution 1: ${dualSearch()}")
    }

    override fun Solution2(input: BufferedReader) {
        println(
            "Solution 2: ${
                dualSearch(
                    mapOf(
                        "zero" to 0, "one" to 1, "two" to 2, "three" to 3, "four" to 4,
                        "five" to 5, "six" to 6, "seven" to 7, "eight" to 8, "nine" to 9
                    )
                )
            }"
        )
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            val solution = ValueCalibration()
            solution.Solution1(FileReader("Day01/input.txt").buffered())
            solution.Solution2(FileReader("Day01/input.txt").buffered())
        }
    }
}
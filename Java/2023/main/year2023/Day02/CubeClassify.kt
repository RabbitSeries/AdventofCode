package year2023.Day02;

import java.io.*;
import java.util.*;
import java.util.stream.IntStream;

import JavaDataModel.ISolution;
import JavaDataModel.AoCSolution

@AoCSolution()
class CubeClassify : ISolution {
    private lateinit var GameList: List<List<Map<String, Int>>>
    fun readFile(input: BufferedReader) {
        GameList = input.use {
            it.lines().map { line ->
                line.split(":")[1].split(";").map { grab ->
                    grab.split(",").associate { cubeInfo ->
                        val arr = cubeInfo.trim().split(Regex("\\s"))
                        arr[1] to arr[0].toInt()
                    }
                }
            }.toList()
        }
    }

    override fun Solution1(input: BufferedReader) {
        readFile(input)
        val available = mapOf("red" to 12, "green" to 13, "blue" to 14)
        println("Solution 1: " + GameList.indices.asSequence().filter { gameId ->
            GameList[gameId].all { it.all { (k, v) -> available[k]?.let { limit -> v <= limit } ?: false } }
        }.sumOf { it + 1 })
    }

    override fun Solution2(input: BufferedReader) {
        println("Solution 2: " + GameList.sumOf { game ->
            game.asSequence().flatMap { it.asSequence() }
                .groupBy({ it.key }, { it.value })
                .asSequence().map { it.value }.map { it.max() }
                .fold(1) { a, b -> a * b }
        })
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            val solution = CubeClassify()
            solution.Solution1(BufferedReader(FileReader("Day02/input.txt")))
            solution.Solution2(BufferedReader(FileReader("Day02/input.txt")))
        }
    }
}

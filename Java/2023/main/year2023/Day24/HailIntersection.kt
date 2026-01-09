package year2023.Day24

import java.io.BufferedReader

import org.hipparchus.linear.ArrayRealVector
import org.hipparchus.optim.InitialGuess
import org.hipparchus.optim.nonlinear.vector.leastsquares.LeastSquaresBuilder
import org.hipparchus.optim.nonlinear.vector.leastsquares.LevenbergMarquardtOptimizer
import org.hipparchus.optim.nonlinear.vector.leastsquares.MultivariateJacobianFunction

import JavaDataModel.AoCSolution
import JavaDataModel.ISolution
import JavaDataModel.inputFileName
import org.hipparchus.linear.Array2DRowRealMatrix
import java.io.File
import kotlin.math.ceil
import kotlin.math.exp

@AoCSolution(day = 24)
class HailIntersection : ISolution {

    private fun String.toPoint2D() = """[+-]*\d+""".toRegex().findAll(this).map { it.value.toLong() }.toList()

    fun ParseHail(s: String): Hail {
        val InputSplit = s.split("@")
        return Hail(InputSplit[0].toPoint2D(), InputSplit[1].toPoint2D())
    }

    lateinit var HailList: List<Hail>

    fun readFile(input: BufferedReader) {
        HailList = input.lineSequence().map(this::ParseHail).toList()
    }

    val L = 200000000000000L
    val R = 400000000000000L

    fun inBoundary(candidate: Pair<Double, Double>?) = candidate?.let { p ->
        p.first >= L && p.first <= R && p.second >= L && p.second <= R
    } ?: false


    override fun Solution1(input: BufferedReader) {
        readFile(input)
        println(
            "Solution 1: ${
                HailList.indices.sumOf { i ->
                    ((i + 1)..(HailList.size - 1)).map { j -> HailList[i].Intersection(HailList[j]) }
                        .count(this::inBoundary)
                }
            }"
        )
    }

    override fun Solution2(input: BufferedReader) {
        val ri = arrayOf(
            HailList[0].pos.asSequence().map { it.toDouble() }.toList(),
            HailList[1].pos.asSequence().map { it.toDouble() }.toList(),
            HailList[2].pos.asSequence().map { it.toDouble() }.toList()
        )
        val vi = arrayOf(
            HailList[0].delta.asSequence().map { it.toDouble() }.toList(),
            HailList[1].delta.asSequence().map { it.toDouble() }.toList(),
            HailList[2].delta.asSequence().map { it.toDouble() }.toList(),
        )
        val gradientJacobian = MultivariateJacobianFunction { point ->
            val params = point.toArray()
            val r = arrayOf(params[0], params[1], params[2])
            val v = arrayOf(params[3], params[4], params[5])
            val t = arrayOf(exp(params[6]), exp(params[7]), exp(params[8]))
            val residuals = DoubleArray(9)
            for (i in 0..2) {//t_1..3 \times R-V x y z
                for (j in 0..2) {
                    residuals[i * 3 + j] = r[j] - ri[i][j] + (v[j] - vi[i][j]) * t[i]
                }
            }
            // \partial{residuals}/\partial{r_x,r_y,r_z,v_x,v_y,v_z,t_1,t_2,t_3}
            val jacobian = Array(9) { DoubleArray(9) }
            for (i in 0..2) {//t_1..3
                for (j in 0..2) {//R_V x,y,z
                    jacobian[i * 3 + j][j] = 1.0//0-2 partial r_x,r_y,r_z
                    jacobian[i * 3 + j][3 + j] = t[i]//3-5 partial v_x,v_y,v_z
                    jacobian[i * 3 + j][6 + i] = (v[j] - vi[i][j]) * t[i]//partial u_i
                }
            }
            org.hipparchus.util.Pair(ArrayRealVector(residuals), Array2DRowRealMatrix(jacobian))
        }
        val initialGuess = InitialGuess(DoubleArray(9)).initialGuess
        val problem = LeastSquaresBuilder()
            .start(initialGuess)
            .model(gradientJacobian)
            .target(ArrayRealVector(9))
            .lazyEvaluation(false)
            .maxEvaluations(Integer.MAX_VALUE)
            .maxIterations(Integer.MAX_VALUE)
            .build()
        val optimizer = LevenbergMarquardtOptimizer().withCostRelativeTolerance(1e-16)
        val optimum = optimizer.optimize(problem)
        val r_v_t_vec = optimum.point.toArray()
        println("Solution 2: ${ceil(r_v_t_vec[0] + r_v_t_vec[1] + r_v_t_vec[2]).toULong()}") // Ugly.............
        // for (int i = 0 i < 9 i++) {
        //     System.out.printf("%.5f,", i > 6 ? Math.exp(r_v_t_vec[i]) : r_v_t_vec[i])
        // }
        // System.out.printf("\n%.5f", optimum.getCost())
    }

    companion object {
        @JvmStatic
        fun main(args: Array<String>) {
            HailIntersection().apply {
                File(inputFileName!!).let {
                    Solution1(it.bufferedReader())
                    Solution2(it.bufferedReader())
                }
            }
        }
    }
}

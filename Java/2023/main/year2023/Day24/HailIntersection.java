package year2023.Day24;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.IntStream;

import org.hipparchus.linear.Array2DRowRealMatrix;
import org.hipparchus.linear.ArrayRealVector;
import org.hipparchus.optim.InitialGuess;
import org.hipparchus.optim.nonlinear.vector.leastsquares.LeastSquaresBuilder;
import org.hipparchus.optim.nonlinear.vector.leastsquares.LeastSquaresOptimizer.Optimum;
import org.hipparchus.optim.nonlinear.vector.leastsquares.LeastSquaresProblem;
import org.hipparchus.optim.nonlinear.vector.leastsquares.LevenbergMarquardtOptimizer;
import org.hipparchus.optim.nonlinear.vector.leastsquares.MultivariateJacobianFunction;
import org.hipparchus.util.Pair;

import JavaDataModel.AoCSolution;
import JavaDataModel.ISolution;

@AoCSolution()
public class HailIntersection implements ISolution {

    Hail ParseHail(String s) {
        var InputSplit = s.split("@");
        return new Hail(
                Arrays.stream(InputSplit[0].trim().split("\\s*,\\s*")).map(Long::parseLong).toList(),
                Arrays.stream(InputSplit[1].trim().split("\\s*,\\s*")).map(Long::parseLong).toList());
    }

    List<Hail> HailList = null;

    void readFile(BufferedReader input) throws Exception {
        HailList = input.lines().map(this::ParseHail).toList();
    }

    final long L = 200000000000000L, R = 400000000000000L;

    boolean inBoundary(Optional<Pair<Double, Double>> candidate) {
        if (candidate.isPresent()) {
            var p = candidate.get();
            return p.getFirst() >= L && p.getFirst() <= R && p.getSecond() >= L && p.getSecond() <= R;
        }
        return false;
    }

    public void Solution1(BufferedReader input) throws Exception {
        readFile(input);
        System.out.println("Solution 1: " + IntStream.range(0, HailList.size()).mapToLong(i -> IntStream.range(i + 1, HailList.size())
                .mapToObj(j -> HailList.get(i).Intersection(HailList.get(j)))
                .filter(this::inBoundary)
                .count()).sum());
    }

    /**
     * Currently using hipparchus MultivariateJacobianFunction as a solution
     * @see HailIntersection_LinearAlgebra#Solution2(List) year2023.Legacy.Day24.HailIntersection_LinearAlgbra
     */
    public void Solution2(BufferedReader input) throws Exception {
        // HailIntersection_LinearAlgebra legacy = new HailIntersection_LinearAlgebra();
        // legacy.Solution2(HailList);
        double[][] ri = {
                HailList.get(0).pos.stream().mapToDouble(Double::valueOf).toArray(),
                HailList.get(1).pos.stream().mapToDouble(Double::valueOf).toArray(),
                HailList.get(2).pos.stream().mapToDouble(Double::valueOf).toArray(),
        };
        double[][] vi = {
                HailList.get(0).delta.stream().mapToDouble(Double::valueOf).toArray(),
                HailList.get(1).delta.stream().mapToDouble(Double::valueOf).toArray(),
                HailList.get(2).delta.stream().mapToDouble(Double::valueOf).toArray(),
        };
        MultivariateJacobianFunction gradientJacobian = point -> {
            var params = point.toArray();
            double[] r = {
                    params[0], params[1], params[2]
            };
            double[] v = {
                    params[3], params[4], params[5]
            };
            double[] t = {
                    Math.exp(params[6]),
                    Math.exp(params[7]),
                    Math.exp(params[8])
            };
            double[] residuals = new double[9];
            for (int i = 0; i < 3; i++) {//t_1..3 \times R-V x y z
                for (int j = 0; j < 3; j++) {
                    residuals[i * 3 + j] = r[j] - ri[i][j] + (v[j] - vi[i][j]) * t[i];
                }
            }
            double[][] jacobian = new double[9][9]; // \partial{residuals}/\partial{r_x,r_y,r_z,v_x,v_y,v_z,t_1,t_2,t_3} 
            for (int i = 0; i < 3; i++) {//t_1..3
                for (int j = 0; j < 3; j++) {//R_V x,y,z
                    jacobian[i * 3 + j][j] = 1;//0-2 partial r_x,r_y,r_z
                    jacobian[i * 3 + j][3 + j] = t[i];//3-5 partial v_x,v_y,v_z
                    jacobian[i * 3 + j][6 + i] = (v[j] - vi[i][j]) * t[i];//partial u_i
                }
            }
            return new Pair<>(new ArrayRealVector(residuals), new Array2DRowRealMatrix(jacobian));
        };
        double[] initialGuess = new InitialGuess(new double[9]).getInitialGuess();
        LeastSquaresProblem problem = new LeastSquaresBuilder()
                .start(initialGuess)
                .model(gradientJacobian)
                .target(new ArrayRealVector(9))
                .lazyEvaluation(false)
                .maxEvaluations(Integer.MAX_VALUE)
                .maxIterations(Integer.MAX_VALUE)
                .build();
        LevenbergMarquardtOptimizer optimizer = new LevenbergMarquardtOptimizer()
                .withCostRelativeTolerance(1e-16);
        Optimum optimum = optimizer.optimize(problem);
        var r_v_t_vec = optimum.getPoint().toArray();
        System.out.println("Solution 2: " + Math.round(r_v_t_vec[0] + r_v_t_vec[1] + r_v_t_vec[2])); // Ugly.............
        // for (int i = 0; i < 9; i++) {
        //     System.out.printf("%.5f,", i > 6 ? Math.exp(r_v_t_vec[i]) : r_v_t_vec[i]);
        // }
        // System.out.printf("\n%.5f", optimum.getCost());
    }

    public static void main(String[] args) throws Exception {
        HailIntersection Day24 = new HailIntersection();
        Day24.Solution1(new BufferedReader(new FileReader("Day24/input.txt")));
        Day24.Solution2(null);
    }
}

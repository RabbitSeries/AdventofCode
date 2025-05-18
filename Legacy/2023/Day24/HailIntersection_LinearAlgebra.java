package Day24;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.List;
import java.util.stream.IntStream;

import org.hipparchus.linear.*;
import org.hipparchus.optim.nonlinear.vector.leastsquares.*;
import org.hipparchus.optim.nonlinear.vector.leastsquares.LeastSquaresOptimizer.*;
import org.hipparchus.util.*;

public class HailIntersection_LinearAlgebra {
    class Hail {
        public Hail(List<Long> p, List<Long> d) {
            pos = p;
            delta = d;
        }

        List<Long> pos, delta;
    }

    Hail ParseHail(String s) {
        var InputSplit = s.split("@");
        return new Hail(
                Arrays.stream(InputSplit[0].trim().split("\\s*,\\s*")).mapToLong(Long::parseLong).boxed().toList(),
                Arrays.stream(InputSplit[1].trim().split("\\s*,\\s*")).mapToLong(Long::parseLong).boxed().toList());
    }

    List<Hail> HailList = null;

    void readFile() throws Exception {
        BufferedReader input = new BufferedReader(new FileReader("input.txt"));
        HailList = input.lines().map(this::ParseHail).toList();
        input.close();
    }

    void Solution2() throws Exception {
        readFile();
        Double[][] ri = {
                HailList.get(0).pos.stream().map(Double::valueOf).toArray(Double[]::new),
                HailList.get(1).pos.stream().map(Double::valueOf).toArray(Double[]::new),
                HailList.get(2).pos.stream().map(Double::valueOf).toArray(Double[]::new),
        };
        Double[][] vi = {
                HailList.get(0).delta.stream().map(Double::valueOf).toArray(Double[]::new),
                HailList.get(1).delta.stream().map(Double::valueOf).toArray(Double[]::new),
                HailList.get(2).delta.stream().map(Double::valueOf).toArray(Double[]::new),
        };
        double[] initialGuess = new double[9];
        LeastSquaresProblem problem = new LeastSquaresBuilder()
                .start(initialGuess)
                .model(modelFunction(ri, vi))
                .target(new ArrayRealVector(9))
                .lazyEvaluation(false)
                .maxEvaluations(Integer.MAX_VALUE)
                .maxIterations(Integer.MAX_VALUE)
                .build();

        LevenbergMarquardtOptimizer optimizer = new LevenbergMarquardtOptimizer()
                // This method relies on a language's double precision implementation (IEEE 754 cpp, Java, Python), which makes this method very fragile.
                // But Python seems to be able to use arbitrary Integer precision in lib sympy solution.
                .withCostRelativeTolerance(1e-16);
        Optimum optimum = optimizer.optimize(problem);
        var r_v_t_vec = optimum.getPoint().toArray();
        var r_vec = IntStream.range(0, 3).mapToLong(i -> (long) r_v_t_vec[i]).toArray();
        System.out.println("Solution 2: " + (r_vec[0] + r_vec[1] + r_vec[2]));
    }

    private MultivariateJacobianFunction modelFunction(Double[][] ri, Double[][] vi) {
        return point -> {
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
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    residuals[i * 3 + j] = r[j] + v[j] * t[i] - ri[i][j] - vi[i][j] * t[i];
                }
            }
            double[][] jacobian = new double[9][9]; // Eh, AI says the jacobian looks like this. The derivation of e^t (T) is still e^t (T),
                                                    // so jacobian stays the same, but T is always positive
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    jacobian[i * 3 + j][j] = 1;
                    jacobian[i * 3 + j][3 + j] = t[i];
                    for (int k = 0; k < 3; k++) {
                        if (i == k) {
                            jacobian[i * 3 + j][6 + k] = (v[j] - vi[i][j]) * t[i];
                        } else {
                            jacobian[i * 3 + j][6 + k] = 0;
                        }
                    }
                }
            }
            return new Pair<>(new ArrayRealVector(residuals), new Array2DRowRealMatrix(jacobian));
        };
    }

    public static void main(String[] args) throws Exception {
        HailIntersection_LinearAlgebra Day24 = new HailIntersection_LinearAlgebra();
        Day24.Solution2();
    }
}

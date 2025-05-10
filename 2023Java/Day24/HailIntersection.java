package Day24;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.IntStream;

import org.apache.commons.math4.legacy.fitting.leastsquares.LeastSquaresBuilder;
import org.apache.commons.math4.legacy.fitting.leastsquares.LeastSquaresOptimizer.Optimum;
import org.apache.commons.math4.legacy.fitting.leastsquares.LeastSquaresProblem;
import org.apache.commons.math4.legacy.fitting.leastsquares.LevenbergMarquardtOptimizer;
import org.apache.commons.math4.legacy.fitting.leastsquares.MultivariateJacobianFunction;
import org.apache.commons.math4.legacy.linear.Array2DRowRealMatrix;
import org.apache.commons.math4.legacy.linear.ArrayRealVector;
import org.apache.commons.math4.legacy.core.Pair;

public class HailIntersection {

    class Hail {
        public Hail(List<Long> p, List<Long> d) {
            pos = p;
            delta = d;
        }

        public Optional<Pair<Double, Double>> Intersection(Hail rhs) {
            long coeff = delta.get(1) * rhs.delta.get(0) - delta.get(0) * rhs.delta.get(1);
            if (coeff != 0) {
                double thisGap = (1.0d * (rhs.delta.get(0) * (rhs.pos.get(1) - pos.get(1)) - rhs.delta.get(1) * (rhs.pos.get(0) - pos.get(0))) / coeff);
                double rhsGap = (pos.get(0) + delta.get(0) * thisGap - rhs.pos.get(0)) / rhs.delta.get(0);
                if (thisGap >= 0 && rhsGap > 0) {
                    return Optional.of(new Pair<Double, Double>(
                            pos.get(0) + delta.get(0) * thisGap,
                            pos.get(1) + delta.get(1) * thisGap));
                }
            }
            return Optional.empty();
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

    final long L = 200000000000000L, R = 400000000000000L;

    boolean inBoundary(Optional<Pair<Double, Double>> candidate) {
        if (candidate.isPresent()) {
            var p = candidate.get();
            return p.getFirst() >= L && p.getFirst() <= R && p.getSecond() >= L && p.getSecond() <= R;
        }
        return false;
    }

    void Solution1() throws Exception {
        readFile();
        System.out.println("Solution 1: " + IntStream.range(0, HailList.size()).mapToLong(i -> IntStream.range(i + 1, HailList.size())
                .mapToObj(j -> HailList.get(i).Intersection(HailList.get(j)))
                .filter(this::inBoundary)
                .count()).sum());
    }

    void Solution2() throws Exception {
        // Learning Python
        // Ehhhhhhhhhhhhhhhhhhhhh??????????????
        double scale = 1e13 + 1e14;
        double[][] ri = {
                {
                        233210433951170.0 / scale, 272655040388795.0 / scale, 179982504986147.0 / scale
                },
                {
                        385274025881243.0 / scale, 351578921558552.0 / scale, 375160114124378.0 / scale
                },
                {
                        298962016918939.0 / scale, 322446494312107.0 / scale, 293073189215975.0 / scale
                }
        };

        // Given velocity vectors (vi) - scaled accordingly
        double[][] vi = {
                {
                        39.0 / scale, -98.0 / scale, 166.0 / scale
                },
                {
                        -71.0 / scale, -36.0 / scale, -9.0 / scale
                },
                {
                        36.0 / scale, 8.0 / scale, 96.0 / scale
                }
        };

        // Initial guess: [r1, r2, r3, v1, v2, v3, ln(t1), ln(t2), ln(t3)]
        // Using log-transform for positive time constraints
        double[] initialGuess = new double[9];

        // Build the least squares problem
        LeastSquaresProblem problem = new LeastSquaresBuilder()
                .start(initialGuess)
                .model(modelFunction(ri, vi))
                .target(new ArrayRealVector(9)) // Target is zero residuals
                .lazyEvaluation(false)
                .maxEvaluations(1000000000)
                .maxIterations(1000000000)
                .build();

        // Solve the problem
        LevenbergMarquardtOptimizer optimizer = new LevenbergMarquardtOptimizer()
                .withCostRelativeTolerance(1e-15)
                .withParameterRelativeTolerance(1e-15);
        Optimum optimum = optimizer.optimize(problem);

        // Extract and process solution
        double[] solution = optimum.getPoint().toArray();
        System.out.println("Solution 2: " + (long) (solution[0] * scale + solution[1] * scale + solution[2] * scale));
    }

    private MultivariateJacobianFunction modelFunction(double[][] ri, double[][] vi) {
        return point -> {
            var params = point.toArray();
            double[] r = {
                    params[0], params[1], params[2]
            };
            double[] v = {
                    params[3], params[4], params[5]
            };
            double[] t = {
                    Math.exp(params[6]), // t1 = exp(ln(t1))
                    Math.exp(params[7]), // t2
                    Math.exp(params[8]) // t3
            };
            double[] residuals = new double[9];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    residuals[i * 3 + j] = r[j] + v[j] * t[i] - ri[i][j] - vi[i][j] * t[i];
                }
            }
            double[][] jacobian = new double[9][9];
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
        HailIntersection Day24 = new HailIntersection();
        Day24.Solution1();
        Day24.Solution2();
    }
}

package Day24;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.IntStream;

import org.hipparchus.util.*;

import JavaDataModel.SolutionBase2023;
import Legacy.Day24.HailIntersection_LinearAlgebra;

public class HailIntersection implements SolutionBase2023 {

    Hail ParseHail(String s) {
        var InputSplit = s.split("@");
        return new Hail(
                Arrays.stream(InputSplit[0].trim().split("\\s*,\\s*")).mapToLong(Long::parseLong).boxed().toList(),
                Arrays.stream(InputSplit[1].trim().split("\\s*,\\s*")).mapToLong(Long::parseLong).boxed().toList());
    }

    List<Hail> HailList = null;

    void readFile() throws Exception {
        BufferedReader input = new BufferedReader(new FileReader("Day24/input.txt"));
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

    public void Solution1() throws Exception {
        readFile();
        System.out.println("Solution 1: " + IntStream.range(0, HailList.size()).mapToLong(i -> IntStream.range(i + 1, HailList.size())
                .mapToObj(j -> HailList.get(i).Intersection(HailList.get(j)))
                .filter(this::inBoundary)
                .count()).sum());
    }

    /**
     * Currently using hipparchus MultivariateJacobianFunction as a solution
     * @see HailIntersection_LinearAlgebra#Solution2() Legacy.Day24.HailIntersection_LinearAlgbra
     */
    public void Solution2() throws Exception {
        HailIntersection_LinearAlgebra legacy = new HailIntersection_LinearAlgebra();
        legacy.Solution2(HailList);
    }

    public static void main(String[] args) throws Exception {
        HailIntersection Day24 = new HailIntersection();
        Day24.Solution1();
        Day24.Solution2();
    }
}

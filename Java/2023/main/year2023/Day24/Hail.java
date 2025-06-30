package year2023.Day24;

import java.util.List;
import java.util.Optional;

import org.hipparchus.util.Pair;

public class Hail {
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

    public List<Long> pos, delta;
}
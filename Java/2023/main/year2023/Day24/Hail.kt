package year2023.Day24

class Hail(val pos: List<Long>, val delta: List<Long>) {
    fun Intersection(rhs: Hail): Pair<Double, Double>? {
        val coeff = delta[1] * rhs.delta[0] - delta[0] * rhs.delta[1]
        if (coeff != 0L) {
            val thisGap = 1.0 * (rhs.delta[0] * (rhs.pos[1] - pos[1]) - rhs.delta[1] * (rhs.pos[0] - pos[0])) / coeff
            val rhsGap = (pos[0] + delta[0] * thisGap - rhs.pos[0]) / rhs.delta[0]
            if (thisGap >= 0 && rhsGap > 0) {
                return pos[0] + delta[0] * thisGap to pos[1] + delta[1] * thisGap
            }
        }
        return null
    }
}
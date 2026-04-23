package JavaDataModel;

import java.util.Comparator;

public class Point2D extends Pair<Integer, Integer> implements Comparator<Point2D>, Comparable<Point2D> {

    public Point2D extractPosPoint2D() {
        // Drop pathList
        return new Point2D(getKey(), getValue());
    }

    public Point2D(Integer _x, Integer _y) {
        super(_x, _y);
    }

    public static boolean isValid(int rows, int cols, Point2D coordinate) {
        return coordinate.getKey() >= 0 && coordinate.getKey() < rows && coordinate.getValue() >= 0
                && coordinate.getValue() < cols;
    }

    public static Point2D getNextPosition(Point2D curPos, int index) {
        return new Point2D(curPos.getKey() + dx[index], curPos.getValue() + dy[index]);
    }

    // up down left right lu ld ru rd
    public static int[] dy = new int[] {
            0, 0, -1, 1, -1, -1, 1, 1
    };

    public static int[] dx = new int[] {
            -1, 1, 0, 0, -1, 1, -1, 1
    };

    @Override
    public int compareTo(Point2D rhs) {
        if (this == rhs)
            return 0;
        return this.first.equals(rhs.first) ? Integer.compare(this.second, rhs.second) : Integer.compare(this.first, rhs.first);
    }

    @Override
    public int compare(Point2D a, Point2D b) {
        return a.compareTo(b);
    }

}

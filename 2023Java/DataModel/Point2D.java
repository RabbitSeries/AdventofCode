package DataModel;

import java.util.List;

public class Point2D extends Pair<Integer, Integer> {
    public List<Pair<Integer, Integer>> pathList;

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
    public static int[] dy = new int[] {0, 0, -1, 1, -1, -1, 1, 1};
    public static int[] dx = new int[] {-1, 1, 0, 0, -1, 1, -1, 1};
}

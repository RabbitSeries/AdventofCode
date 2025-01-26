package DataModel;

import java.util.Objects;

public class Point2D {
    Integer x;
    Integer y;

    public Point2D(int _x, int _y) {
        x = _x;
        y = _y;
    }

    @Override
    public boolean equals(Object object) {
        if (this == object)
            return true;
        if (object == null || getClass() != object.getClass())
            return false;
        Point2D p = (Point2D) object;
        return p.x.equals(x) && p.y.equals(y);
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y);
    }

    public Integer getX() {
        return this.x;
    }

    public void setX(Integer x) {
        this.x = x;
    }

    public Integer getY() {
        return this.y;
    }

    public void setY(Integer y) {
        this.y = y;
    }

    public static boolean isValid(int rows, int cols, Point2D coordinate) {
        return coordinate.getX() >= 0 && coordinate.getX() < rows && coordinate.getY() >= 0 && coordinate.getY() < cols;
    }

    public static int[] dy = new int[] { 0, 0, -1, 1, -1, -1, 1, 1 };
    public static int[] dx = new int[] { -1, 1, 0, 0, -1, 1, -1, 1 };
}

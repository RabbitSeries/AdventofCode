package DataModel;

import java.util.*;

public class Pair<K, V> {

    K first;
    V second;

    public Pair(K _x, V _y) {
        first = _x;
        second = _y;
    }

    public K getKey() {
        return first;
    }

    public V getValue() {
        return second;
    }

    @Override
    public boolean equals(Object object) {
        if (this == object)
            return true;
        if (object == null || getClass() != object.getClass())
            return false;
        Pair<?, ?> p = (Pair<?, ?>) object;
        return Objects.equals(first, p.first) && Objects.equals(second, p.second);
    }

    @Override
    public int hashCode() {
        return Objects.hash(getKey(), getValue());
    }

}

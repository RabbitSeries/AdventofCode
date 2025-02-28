package JavaDataModel;

import java.util.Objects;

public class TenaryTuple<E1, E2, E3> {
    E1 e1;
    E2 e2;
    E3 e3;

    public TenaryTuple(E1 e1, E2 e2, E3 e3) {
        this.e1 = e1;
        this.e2 = e2;
        this.e3 = e3;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        TenaryTuple<?, ?, ?> tuple = (TenaryTuple<?, ?, ?>) obj;
        return Objects.equals(e1, tuple.e1) && Objects.equals(e2, tuple.e2)
                && Objects.equals(e3, tuple.e3);
    }

    @Override
    public int hashCode() {
        return Objects.hash(e1, e2, e2);
    }
}

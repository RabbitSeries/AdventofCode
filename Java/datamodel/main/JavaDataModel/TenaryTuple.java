package JavaDataModel;

import java.util.Objects;

public class TenaryTuple<E1, E2, E3> {
    E1 e1;

    E2 e2;

    E3 e3;

    public E1 getE1() {
        return this.e1;
    }

    public void setE1(E1 e1) {
        this.e1 = e1;
    }

    public E2 getE2() {
        return this.e2;
    }

    public void setE2(E2 e2) {
        this.e2 = e2;
    }

    public E3 getE3() {
        return this.e3;
    }

    public void setE3(E3 e3) {
        this.e3 = e3;
    }

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

    @Override
    public String toString() {
        return "(" + getE1().toString() + "," + getE2().toString() + "," + getE3().toString() + ")";
    }
}

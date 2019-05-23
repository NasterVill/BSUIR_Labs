package com.coursework.g75.lightningtuner.src.auxiliary_types;

import java.util.Objects;

public final class Pair<A, B> {
    private final A first;
    private final B second;

    public Pair(A a, B second) {
        this.first = a;
        this.second = second;
    }

    public static <C, D> Pair<C, D> valueOf(C c, D d) {
        return new Pair<C, D>(c, d);
    }

    public A getFirst() {
        return first;
    }

    public B getSecond() {
        return second;
    }

    @Override
    public String toString() {
        return "[" + Objects.toString(first) + ", " + Objects.toString(second) + "]";
    }

    @Override
    public boolean equals(Object x) {
        if (!(x instanceof Pair))
            return false;
        else {
            Pair<?,?> that = (Pair<?,?>) x;
            return
                    Objects.equals(this.first, that.first) &&
                            Objects.equals(this.second, that.second);
        }
    }

    @Override
    public int hashCode() {
        return Objects.hash(first, second);
    }
}

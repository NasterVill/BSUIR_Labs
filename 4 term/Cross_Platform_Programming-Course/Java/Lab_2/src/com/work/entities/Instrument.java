package com.work.entities;

/**
 * <h1>Instrument</h1>
 *
 * Instrument class serves as a
 * container class for all other
 * instruments, which are defined
 * in this program.
 */

public class Instrument {
    private int mass;

    public Instrument(int mass)
        throws IllegalArgumentException{
        if (mass <= 0) {
            throw new IllegalArgumentException("The value of instrument's param mass can't be zero or negative!");
        }
        this.mass = mass;
    }

    public int getMass() {
        return this.mass;
    }

}

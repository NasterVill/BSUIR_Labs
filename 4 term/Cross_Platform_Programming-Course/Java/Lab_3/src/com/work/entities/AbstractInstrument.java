package com.work.entities;

import com.work.product.Blank;
import com.work.exceptions.InvalidShapeParamException;

/**
 * <h1>AbstractInstrument</h1>
 *
 * AbstractInstrument class serves as a
 * container class for all other
 * instruments, which are defined
 * in this program.
 */

public abstract class AbstractInstrument {
    private int mass;

    public AbstractInstrument(int mass)
        throws IllegalArgumentException{
        if (mass <= 0) {
            throw new IllegalArgumentException("The value of instrument's param mass can't be zero or negative!");
        }
        this.mass = mass;
    }

    public int getMass() {
        return this.mass;
    }

    public abstract void performAction(Blank item) throws InvalidShapeParamException;
}

package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;

/**
 * <h1>Saw</h1>
 *
 * Saw class is used to reduce
 * blank's(Blank object) length thus changing it's
 * shape.
 */
public class Saw extends Instrument {
    public static final int CUT_LENGTH = 9;

    private int length;

    public Saw(int length, int mass)
        throws IllegalArgumentException{
        super(mass);
        if(length <= 0) {
            throw new IllegalArgumentException("The value of saw's param length can't be zero or negative!");
        }
        this.length = length;
    }

    public void cut(Blank item)
            throws InvalidShapeParamException {
        item.shape.changeLength(item.shape.getLength() - CUT_LENGTH);
    }

}

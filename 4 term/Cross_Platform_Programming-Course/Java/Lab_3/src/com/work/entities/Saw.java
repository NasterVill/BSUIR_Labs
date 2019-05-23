package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;
import com.work.product.Blank;

/**
 * <h1>Saw</h1>
 *
 * Saw class is used to reduce
 * blank's(Blank object) length thus changing it's
 * shape.
 */
public class Saw extends AbstractInstrument {
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

    @Override
    public void performAction(Blank item)
            throws InvalidShapeParamException {
        item.getShape().changeLength(item.getShape().getLength() - CUT_LENGTH);
        item.getShape().changeWidth(item.getShape().getWidth() - CUT_LENGTH);
    }

}

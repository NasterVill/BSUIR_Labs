package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;
import com.work.product.Blank;

/**
 * <h1>JackPlane</h1>
 *
 * JackPlane class is used to reduce
 * blank's(Blank object) width thus changing it's
 * shape.
 */
public class JackPlane extends AbstractInstrument {
    public static final int SMOOTH_CUT = 1;

    public JackPlane (int mass)
        throws IllegalArgumentException{
        super(mass);
    }

    @Override
    public void performAction(Blank item)
            throws InvalidShapeParamException {
        item.getShape().changeLength(item.getShape().getLength() - SMOOTH_CUT);
        item.getShape().changeWidth(item.getShape().getWidth() - SMOOTH_CUT);
    }
}

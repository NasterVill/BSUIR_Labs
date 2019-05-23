package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;

/**
 * <h1>JackPlane</h1>
 *
 * JackPlane class is used to reduce
 * blank's(Blank object) width thus changing it's
 * shape.
 */
public class JackPlane extends Instrument {
    public static final int SMOOTH_CUT = 1;

    public JackPlane (int mass)
        throws IllegalArgumentException{
        super(mass);
    }

    public void smoothOut(Blank item)
        throws InvalidShapeParamException {
        item.shape.changeWidth(item.shape.getWidth() - SMOOTH_CUT);
    }
}

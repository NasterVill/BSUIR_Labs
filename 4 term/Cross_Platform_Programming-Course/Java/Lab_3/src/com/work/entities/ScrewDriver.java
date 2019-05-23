package com.work.entities;

import com.work.product.Blank;

/**
 * <h1>JackPlane</h1>
 *
 * JackPlane is used to change blank's(Blank object)
 * screwed field thus "screwing" or unscrewing it.
 */
public class ScrewDriver extends AbstractInstrument {
    public ScrewDriver(int mass) {
        super(mass);
    }

    @Override
    public void performAction(Blank item) {
        item.getProductInfo().changeState();
    }
}

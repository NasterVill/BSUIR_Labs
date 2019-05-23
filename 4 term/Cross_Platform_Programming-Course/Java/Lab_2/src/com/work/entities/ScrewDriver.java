package com.work.entities;

/**
 * <h1>JackPlane</h1>
 *
 * JackPlane is used to change blank's(Blank object)
 * screwed field thus "screwing" or unscrewing it.
 */
public class ScrewDriver extends Instrument {
    public ScrewDriver(int mass) {
        super(mass);
    }

    public void screw(Blank item) {
        item.screwed = true;
    }

    public void unScrew(Blank item) {
        item.screwed = false;
    }
}

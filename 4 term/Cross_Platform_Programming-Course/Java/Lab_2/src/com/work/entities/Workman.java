package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;

import java.util.ArrayList;
import java.util.List;

/**
 * <h1>Workman</h1>
 *
 * Workman class represents the logic of the program,
 * thus it serves as a controller, which connects all
 * other classes's functionality.
 * It represents the workman.
 */
public class Workman {
    private String name;
    private List<Instrument> instruments;

    public Workman (String name ,Saw saw, JackPlane jackplane, ScrewDriver screwdriver)
            throws NullPointerException {
        if (saw == null) {
            throw new NullPointerException("The value of saw param can not be null!");
        }
        if (jackplane == null) {
            throw new NullPointerException("The value of jack-plane param can not be null!");
        }
        if (screwdriver == null) {
            throw new NullPointerException("The value of screwdriver param can not be null!");
        }
        if (name.isEmpty() || name == null) {
            throw new NullPointerException("The value of name param can not be null or empty!");
        }

        this.name = name;

        this.instruments = new ArrayList<Instrument>();
        instruments.add(saw);
        instruments.add(jackplane);
        instruments.add(screwdriver);
    }

    public String getName() {
        return this.name;
    }

    /**
     * This method is used to configure the pane
     * for the program to be able to interact with user
     * @param item This is the Blank object, which can be modified
     *             in this method (depending on the values of other params).
     * @param allowCut allows/restricts cutting(reducing length) the Blank object
     * @param allowSmoothing allows/restricts smoothing(reducing width) the Blank object
     * @param allowScrewing allows/restricts screwing the Blank object
     * @return void.
     * @throws InvalidShapeParamException
     * @see Blank
     */
    public void work(Blank item, boolean allowCut, boolean allowSmoothing, boolean allowScrewing)
        throws InvalidShapeParamException {
        if (allowScrewing && item.isScrewed()) {
            ((ScrewDriver) instruments.get(2)).unScrew(item);
        } else if (allowScrewing && !item.isScrewed()) {
            ((ScrewDriver) instruments.get(2)).screw(item);
        }

        if(allowCut) {
            try {
                ((Saw) instruments.get(0)).cut(item);
            } catch (InvalidShapeParamException exception) {
                if (item.shape.getWidth() > Saw.CUT_LENGTH && allowSmoothing) {
                    ((JackPlane) instruments.get(1)).smoothOut(item);
                }
                throw exception;
            }
        }

        if(allowSmoothing) {
                ((JackPlane) instruments.get(1)).smoothOut(item);
        }
    }
}

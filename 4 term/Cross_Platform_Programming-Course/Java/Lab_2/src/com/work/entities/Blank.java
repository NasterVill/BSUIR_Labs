package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;

/**
 * <h1>Blank</h1>
 *
 * Blank class represents the product,
 * that the workman is handling.
 */
public class Blank {
    protected Shape shape;
    protected boolean screwed;

    public Blank() {
        this.shape = new Shape();
        this.screwed = false;
    }

    public Blank(Shape shape, boolean isScrewed)
    throws NullPointerException {
        if(shape == null) {
            throw new NullPointerException("The value of param shape can't be null!");
        }
        this.shape = shape;
        this.screwed = isScrewed;
    }

    public Blank(int width, int height, boolean isScrewed)
        throws InvalidShapeParamException {
        this.shape = new Shape(width, height);
        this.screwed = isScrewed;
    }

    public boolean isScrewed() {
        return this.screwed;
    }

    public Shape getShape() {
        return this.shape;
    }
}

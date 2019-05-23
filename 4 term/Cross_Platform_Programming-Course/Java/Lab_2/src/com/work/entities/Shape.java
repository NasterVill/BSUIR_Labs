package com.work.entities;

import com.work.exceptions.InvalidShapeParamException;

/**
 * <h1>Shape</h1>
 *
 * Shape class is used to store information
 * about blank(Blank object), like it's length and width.
 * It is the attribute of the Blank class.
 */
public class Shape {
    private int width;
    private int length;

    public Shape() {
        this.length = 100;
        this.width = 100;
    }

    public Shape(int width, int length)
        throws InvalidShapeParamException {
        if (width <= 0) {
            throw new InvalidShapeParamException("Blank's shape can't have zero or negative width!");
        }
        if (length <= 0) {
            throw new InvalidShapeParamException("Blank's shape can't have zero or negative height!");
        }

        this.width = width;
        this.length = length;
    }

    public int getWidth() {
        return this.width;
    }

    public int getLength() {
        return this.length;
    }

    public void changeWidth(int width)
        throws InvalidShapeParamException {
        if (width <= 0) {
            throw new InvalidShapeParamException("Blank's shape can't have zero or negative width!");
        }
        this.width = width;
    }

    public void changeLength(int length)
            throws InvalidShapeParamException {
        if (length <= 0) {
            throw new InvalidShapeParamException("Blank's shape can't have zero or negative height!");
        }
        this.length = length;
    }
}

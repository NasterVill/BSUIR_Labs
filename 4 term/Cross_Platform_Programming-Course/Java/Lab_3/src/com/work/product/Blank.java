package com.work.product;

import com.work.exceptions.InvalidShapeParamException;

/**
 * <h1>Blank</h1>
 *
 * Blank class represents the product,
 * that the workman is handling.
 */
public class Blank {
    private Shape shape;
    private ProductInfo productInfo;

    public Blank() {
        this.shape = new Shape();
        this.productInfo = new ProductInfo();
    }

    public Blank(Shape shape)
    throws NullPointerException {
        if(shape == null) {
            throw new NullPointerException("The value of param shape can't be null!");
        }
        this.shape = shape;
        this.productInfo = new ProductInfo();
    }

    public Blank(int width, int height)
        throws InvalidShapeParamException {
        this.shape = new Shape(width, height);
        this.productInfo = new ProductInfo();
    }


    public Shape getShape() {
        return this.shape;
    }

    public ProductInfo getProductInfo() {
        return this.productInfo;
    }
}

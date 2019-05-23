package com.work.product;

public class ProductInfo {
    private boolean state;

    public ProductInfo() {
        state = false;
    }

    public boolean isReady() {
        return this.state;
    }

    public void changeState() {
        state = !state;
    }
}

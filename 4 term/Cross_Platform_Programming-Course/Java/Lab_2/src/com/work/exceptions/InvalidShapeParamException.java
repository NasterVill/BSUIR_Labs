package com.work.exceptions;

public class InvalidShapeParamException extends Exception {
    private static final long serialVersionUID = 1L;

    public InvalidShapeParamException(String message) {
        super(message);
    }

}
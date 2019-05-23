package exceptions;

public class InvalidWaiterParamException extends Exception {
    private static final long serialVersionUID = 1L;

    public InvalidWaiterParamException(String message) {
        super(message);
    }
}

package exceptions;

public class InvalidPhilosopherParamException extends Exception {
    private static final long serialVersionUID = 1L;

    public InvalidPhilosopherParamException(String message) {
        super(message);
    }
}

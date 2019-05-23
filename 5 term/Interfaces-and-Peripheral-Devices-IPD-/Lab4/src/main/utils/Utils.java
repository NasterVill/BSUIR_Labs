package utils;

import javax.swing.*;

public final class Utils {
    private Utils() {};

    public static void showExceptionDialog(Exception exception) {
        JOptionPane.showMessageDialog(
            null,
            "Description: " + exception.getMessage(),
            "Exception caught!",
            JOptionPane.WARNING_MESSAGE
        );
    }
}
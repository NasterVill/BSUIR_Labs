package util;

import javafx.application.Platform;
import javafx.scene.control.Alert;

import java.lang.reflect.Array;
import java.util.Arrays;

public final class Utils {
    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();

    public static int byteToUnsignedInt(byte b) {
        return 0x00 << 24 | b & 0xff;
    }

    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }

        return Utils.insertPeriodically(new String(hexChars), " ", 2);
    }

    public static String insertPeriodically(
            String text, String insert, int period) {
        StringBuilder builder = new StringBuilder(
                text.length() + insert.length() * (text.length()/period)+1);

        int index = 0;
        String prefix = "";
        while (index < text.length())
        {
            // Don't put the insert in the very first iteration.
            // This is easier than appending it *after* each substring
            builder.append(prefix);
            prefix = insert;
            builder.append(text.substring(index,
                    Math.min(index + period, text.length())));
            index += period;
        }
        return builder.toString();
    }

    public static void showExceptionDialog(Exception exception) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error dialog");
        alert.setHeaderText("Exception caught!");
        alert.setContentText("Description: " + exception.getMessage());

       exception.printStackTrace();

        alert.showAndWait();
    }

    public static void runOnUIThread(Runnable task) {
        if(task == null) throw new NullPointerException("Param task can not be null");

        if(Platform.isFxApplicationThread()) task.run();
        else Platform.runLater(task);
    }

    private Utils() {};
}

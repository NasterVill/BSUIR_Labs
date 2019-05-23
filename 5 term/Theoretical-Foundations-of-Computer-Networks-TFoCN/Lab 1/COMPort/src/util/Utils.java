package util;

import javafx.application.Platform;
import javafx.scene.control.Alert;

public final class Utils {
    private Utils() {};

    public static void showExceptionDialog(Exception exception) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error dialog");
        alert.setHeaderText("Exception caught!");
        alert.setContentText("Description: " + exception.getMessage());

        alert.showAndWait();
    }

    public static void runOnUIThread(Runnable task) {
        if(task == null) throw new NullPointerException("Param task can not be null");

        if(Platform.isFxApplicationThread()) task.run();
        else Platform.runLater(task);
    }
}

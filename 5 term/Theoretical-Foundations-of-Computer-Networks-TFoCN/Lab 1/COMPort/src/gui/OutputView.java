package gui;

import controller.SerialPortController;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.layout.VBox;
import util.Utils;

public class OutputView {
    private static final double PADDING = 5;

    private SerialPortController serialPortController;

    private VBox outputLayout;
    private TextArea outputArea;

    private double width;
    private double height;

    public OutputView(SerialPortController serialPortController,
                      TextArea outputArea,
                      double width,
                      double height) {
        this.width = (width > 0) ? width : MainView.MIN_WIDTH;
        this.height = (height > 0) ? height : MainView.MIN_HEIGHT;
        this.outputArea = outputArea;
        this.serialPortController = serialPortController;

        this.initLayout();
    }

    public VBox getLayout() {
        return this.outputLayout;
    }

    private void initLayout() {
        if(outputArea == null) {
            throw new NullPointerException("The value of param 'outputArea' must not be null");
        }

        if(serialPortController == null) {
            throw new NullPointerException("The value of param 'serialPortController' must not be null");
        }

        outputLayout = new VBox();
        outputLayout.setPadding(new Insets(PADDING));
        outputLayout.setSpacing(PADDING);
        outputLayout.setMinSize(this.width, this.height);
        outputLayout.setFillWidth(true);

        Label outputLabel = new Label("Output Area: ");
        outputLayout.getChildren().add(outputLabel);

        outputArea.setEditable(false);
        outputArea.setWrapText(true);
        outputLayout.getChildren().add(outputArea);

        Button clearButton = new Button("Clear");
        clearButton.setPrefWidth(this.width);
        clearButton.setOnAction(event -> onClear());
        outputLayout.getChildren().add(clearButton);
    }

    private void onClear() {
        this.outputArea.clear();

        if(this.serialPortController.portIsOpened()) {
            try {
                this.serialPortController.clearBuffer();
            } catch (Exception exception) {
                Utils.showExceptionDialog(exception);
            }
        }
    }
}

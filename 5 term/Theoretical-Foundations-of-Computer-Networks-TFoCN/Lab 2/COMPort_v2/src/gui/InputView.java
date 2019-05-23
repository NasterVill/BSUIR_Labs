package gui;

import controller.SerialPortController;
import javafx.geometry.Insets;

import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import util.Utils;

import java.util.ArrayList;

public class InputView {
    private static final double PADDING = 5;

    private SerialPortController serialPortController;

    private VBox inputLayout;
    private TextArea inputArea;
    private Text outputBytesFiled;

    private TextArea packageArea;
    private ComboBox<String> destinationList;

    private double width;
    private double height;

    public InputView(SerialPortController serialPortController,
                     TextArea inputArea,
                     Text outputBytesFiled,
                     TextArea packageArea,
                     ComboBox<String> destinationList,
                     double width,
                     double height) {
        this.width = (width > 0) ? width : MainView.MIN_WIDTH;
        this.height = (height > 0) ? height : MainView.MIN_HEIGHT;
        this.inputArea = inputArea;
        this.packageArea = packageArea;
        this.destinationList = destinationList;
        this.outputBytesFiled = outputBytesFiled;
        this.serialPortController = serialPortController;
        this.initLayout();
    }

    public void setOutputBytesField(Text outputBytesFiled) {
        this.outputBytesFiled = outputBytesFiled;
    }

    public void setInputArea(TextArea inputArea) {
        this.inputArea = inputArea;
    }

    public VBox getLayout() {
        return this.inputLayout;
    }

    private void initLayout() {
        if(inputArea == null) {
            throw new NullPointerException("The value of param 'inputArea' must not be null");
        }

        if(outputBytesFiled == null) {
            throw new NullPointerException("The value of param 'outputBytesFiled' must not be null");
        }

        if(serialPortController == null) {
            throw new NullPointerException("The value of param 'serialPortController' must not be null");
        }

        inputLayout = new VBox();
        inputLayout.setPadding(new Insets(PADDING));
        inputLayout.setSpacing(PADDING);
        inputLayout.setMinSize(this.width, this.height);
        inputLayout.setFillWidth(true);

        Label inputLabel = new Label("Input Area: ");
        inputLayout.getChildren().add(inputLabel);

        inputArea.setWrapText(true);
        inputLayout.getChildren().add(inputArea);

        Button sendButton = new Button("Send");
        sendButton.setPrefWidth(this.width);
        sendButton.setOnAction(event -> onSend());
        inputLayout.getChildren().add(sendButton);
    }

    private void onSend() {
        try {
            if(this.serialPortController.getXOffState() && this.serialPortController.getXonXoffFlowControlMode()) {
                return;
            }

            ArrayList<byte[]> messages = serialPortController.composePackages(
                    inputArea.getText().getBytes(),
                    (byte)Integer.parseInt(
                            destinationList
                            .getSelectionModel()
                            .selectedItemProperty()
                            .get()
                    )
            );

            this.packageArea.clear();

            for(byte[] message : messages) {
                this.packageArea.appendText(Utils.bytesToHex(message));
                this.packageArea.appendText("\n\n");
            }

            this.serialPortController.write(messages);

            outputBytesFiled.setText(Integer.toString(this.serialPortController.getSentBytes()));

            inputArea.clear();
        }
        catch (Exception exception) {
            Utils.runOnUIThread(() -> Utils.showExceptionDialog(exception));
        }
    }
}

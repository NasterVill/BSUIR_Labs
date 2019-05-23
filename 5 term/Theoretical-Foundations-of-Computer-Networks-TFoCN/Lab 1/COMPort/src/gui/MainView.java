package gui;

import controller.SerialPortController;
import javafx.geometry.Insets;
import javafx.geometry.VPos;
import javafx.scene.Scene;
import javafx.scene.control.Separator;
import javafx.scene.control.TextArea;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;

public class MainView {
    public static final double MIN_WIDTH = 360;
    public static final double MIN_HEIGHT = 240;
    public static final String XON_IS_ON_MESSAGE = "XON was received. Data sending is allowed";
    public static final String XOFF_IS_ON_MESSAGE = "XOFF was received. Data sending is restricted";
    public static final String XON_XOFF_IS_NOT_SUPPORTED = "XON/XOFF flow control is not supported";

    private Scene scene;

    private TextArea inputArea;
    private TextArea outputArea;
    private Text textXonXoff;
    private Text bytesReceived;
    private Text bytesSent;

    private SerialPortController serialPortController;

    public MainView(SerialPortController serialPortController) {
        inputArea = new TextArea();
        outputArea = new TextArea();;
        bytesReceived = new Text();
        bytesSent = new Text();
        textXonXoff = new Text();

        final Separator separatorOne = new Separator();
        separatorOne.setPrefWidth(MIN_WIDTH);
        separatorOne.setValignment(VPos.CENTER);

        final Separator separatorTwo = new Separator();
        separatorTwo.setPrefWidth(MIN_WIDTH);
        separatorTwo.setValignment(VPos.CENTER);

        InputView inputView = new InputView(serialPortController, inputArea, bytesSent, MIN_WIDTH, MIN_HEIGHT);
        OutputView outputView = new OutputView(serialPortController, outputArea, MIN_WIDTH, MIN_HEIGHT);
        DebugView debugView = new DebugView(serialPortController, textXonXoff, bytesReceived, bytesSent, MIN_WIDTH, MIN_HEIGHT);

        // Overall component's size + padding
        final double height = MIN_HEIGHT * 3 + 40;
        final double width = MIN_WIDTH + 10;

        VBox mainLayout = new VBox();
        mainLayout.setPadding(new Insets(5));
        mainLayout.setSpacing(5);

        mainLayout.getChildren().add(inputView.getLayout());
        mainLayout.getChildren().add(separatorOne);
        mainLayout.getChildren().add(debugView.getLayout());
        mainLayout.getChildren().add(separatorTwo);
        mainLayout.getChildren().add(outputView.getLayout());

        scene = new Scene(mainLayout, width,  height);
    }

    public TextArea getInputArea() {
        return inputArea;
    }

    public TextArea getOutputArea() {
        return this.outputArea;
    }

    public Text getBytesReceivedField() {
        return this.bytesReceived;
    }

    public Text getBytesSentFiled() {
        return this.bytesSent;
    }

    public Text getTextXonXoff() {
        return this.textXonXoff;
    }

    public Scene getScene() {
        return this.scene;
    }
}

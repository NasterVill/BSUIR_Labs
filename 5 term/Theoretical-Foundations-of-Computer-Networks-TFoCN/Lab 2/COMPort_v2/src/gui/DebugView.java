package gui;

import controller.SerialPortController;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import jssc.SerialPort;
import util.Utils;

import java.util.ArrayList;

public class DebugView {
    private static final double GAP = 5;
    private static final double PADDING = 5;

    private SerialPortController serialPortController;

    private VBox debugLayout;

    private Text textXonXoff;
    private Text bytesReceived;
    private Text bytesSent;
    private TextArea packageArea;

    private ComboBox<String> portList;
    private ComboBox<String> destinationList;
    private ComboBox<String> sourceAddressList;
    private CheckBox flowControlXonXoffCheck;

    private Button openPortButton;
    private Button closePortButton;
    private Button sendXonButton;
    private Button sendXoffButton;

    private String portName = null;
    private boolean flowControlXonXoffEnabled = false;
    private boolean portIsOpened = false;

    private byte destination = -1;
    private byte address = -1;

    private double width;
    private double height;

    public DebugView(SerialPortController serialPortController,
                     Text textXonXoff,
                     Text bytesReceived,
                     Text bytesSent,
                     TextArea packageArea,
                     ComboBox<String> destinationList,
                     double width,
                     double height) {
        this.serialPortController = serialPortController;
        this.bytesReceived = bytesReceived;
        this.destinationList = destinationList;
        this.packageArea = packageArea;
        this.textXonXoff = textXonXoff;
        this.bytesSent = bytesSent;
        this.height = height;
        this.width = width;
        this.initLayout();
    }

    public Text getBytesReceivedField() {
        return this.bytesReceived;
    }

    public Text getBytesSentFiled() {
        return this.bytesSent;
    }

    public VBox getLayout() {
        return this.debugLayout;
    }

    private void initLayout() {
        if (bytesReceived == null) {
            throw new NullPointerException("The value of param 'bytesReceived' must not be null");
        }

        if (bytesSent == null) {
            throw new NullPointerException("The value of param 'bytesSent' must not be null");
        }

        debugLayout = new VBox();
        debugLayout.setPadding(new Insets(PADDING));
        debugLayout.setSpacing(PADDING);
        debugLayout.setFillWidth(true);
        debugLayout.setMinSize(this.width, this.height);

        sourceAddressList = new ComboBox<>();

        ScrollPane scrollPane = new ScrollPane();
        VBox.setVgrow(scrollPane, Priority.ALWAYS);

        GridPane debugGridPane = new GridPane();
        debugGridPane.setPadding(new Insets(0));
        debugGridPane.setHgap(GAP);
        debugGridPane.setVgap(GAP);


        Label portListLabel = new Label("Current Port: ");
        debugGridPane.add(portListLabel, 0, 0);
        ObservableList<String> items = FXCollections.observableArrayList(
                SerialPortController.getPortNames());
        portList = new ComboBox<>(items);
        portList.setPromptText("Ports");
        portList.valueProperty().
                addListener((observableValue, previous, current) ->
                        portName = current);
        debugGridPane.add(portList, 1, 0);


        Label flowControlXonXoffLabel = new Label("Enable XON/XOFF flow control ");
        debugGridPane.add(flowControlXonXoffLabel, 0, 1);
        flowControlXonXoffCheck = new CheckBox();
        flowControlXonXoffCheck.setSelected(false);
        flowControlXonXoffCheck.selectedProperty().
                addListener((observableValue, previous, current) ->
                        flowControlXonXoffEnabled = current);
        debugGridPane.add(flowControlXonXoffCheck, 1, 1);


        Label bytesReceivedLabel = new Label("Received (bytes): ");
        debugGridPane.add(bytesReceivedLabel, 0, 2);
        bytesReceived.setText("0");
        debugGridPane.add(bytesReceived, 1, 2);


        Label bytesSentLabel = new Label("Sent (bytes): ");
        debugGridPane.add(bytesSentLabel, 0, 3);
        bytesSent.setText("0");
        debugGridPane.add(bytesSent, 1, 3);

        openPortButton = new Button("Open port");
        openPortButton.setPrefWidth((this.width - PADDING) / 2);
        openPortButton.setOnAction(event -> onOpenPort());
        debugGridPane.add(openPortButton, 0, 4);


        closePortButton = new Button("Close port");
        closePortButton.setPrefWidth((this.width - PADDING) / 2);
        closePortButton.setOnAction(event -> onClosePort());
        closePortButton.setDisable(!this.portIsOpened);
        debugGridPane.add(closePortButton, 1, 4);

        sendXoffButton = new Button("Send XOFF");
        sendXoffButton.setPrefWidth((this.width - PADDING) / 2);
        sendXoffButton.setOnAction(event -> sendXoff());
        sendXoffButton.setDisable(true);
        debugGridPane.add(sendXoffButton, 0, 5);

        sendXonButton = new Button("Send XON");
        sendXonButton.setPrefWidth((this.width - PADDING) / 2);
        sendXonButton.setOnAction(event -> sendXon());
        sendXonButton.setDisable(true);
        debugGridPane.add(sendXonButton, 1, 5);


        ArrayList<String> addresses = new ArrayList<>();
        for (int i = 0; i <= 0xFF; i++) {
            addresses.add(Integer.toString(i));
        }

        Label sourceAddressLabel = new Label("This address: ");
        debugGridPane.add(sourceAddressLabel, 0, 6);

        ObservableList<String> sourceAddresses = FXCollections.observableArrayList(
                addresses);
        sourceAddressList.setItems(sourceAddresses);
        sourceAddressList.setPromptText("Addresses");
        sourceAddressList.valueProperty().
            addListener((observableValue, previous, current) ->
                    address = (byte)Integer.parseInt(current));
        debugGridPane.add(sourceAddressList, 1, 6);

        Label destinationLabel = new Label("Destination address: ");
        debugGridPane.add(destinationLabel, 0, 7);

        ObservableList<String> destinations = FXCollections.observableArrayList(
                addresses);
        destinationList.setItems(destinations);
        destinationList.setPromptText("Addresses");
        destinationList.valueProperty().
                addListener((observableValue, previous, current) ->
                        destination = (byte)Integer.parseInt(current));
        debugGridPane.add(destinationList, 1, 7);

        debugLayout.getChildren().add(debugGridPane);
        debugLayout.getChildren().add(textXonXoff);

        Label packageAreaLabel = new Label("Composed package(s): ");
        debugLayout.getChildren().add(packageAreaLabel);

        packageArea.setEditable(false);
        packageArea.setWrapText(true);
        debugLayout.getChildren().add(packageArea);
    }

    private void sendXoff() {
        try {
            this.serialPortController.sendXoff(destination);
        } catch(Exception exception) {
            Utils.runOnUIThread(() -> Utils.showExceptionDialog(exception));
        }
    }

    private void sendXon() {
        try {
            this.serialPortController.sendXon(destination);
        } catch(Exception exception) {
            Utils.runOnUIThread(() -> Utils.showExceptionDialog(exception));
        }
    }

    private void onOpenPort() {
        try {
            if(address == -1 || destination == -1) {
                throw new IllegalArgumentException("You must set source/destination address.");
            }

            if(address == destination) {
                throw new IllegalArgumentException("Source and destination address can't be equal!");
            }

            SerialPort serialPort = new SerialPort(portName);
            this.serialPortController.setSerialPort(serialPort, (byte)address, flowControlXonXoffEnabled);

            this.portIsOpened = !this.portIsOpened;

            sourceAddressList.setDisable(this.portIsOpened);
            openPortButton.setDisable(this.portIsOpened);
            closePortButton.setDisable(!this.portIsOpened);
            sendXonButton.setDisable(!this.flowControlXonXoffEnabled);
            sendXoffButton.setDisable(!this.flowControlXonXoffEnabled);
            portList.setDisable(this.portIsOpened);
            flowControlXonXoffCheck.setDisable(this.portIsOpened);

            if(this.flowControlXonXoffEnabled) {
                textXonXoff.setText(MainView.XOFF_IS_ON_MESSAGE);
                textXonXoff.setFill(Color.RED);
            } else {
                textXonXoff.setText(MainView.XON_XOFF_IS_NOT_SUPPORTED);
                textXonXoff.setFill(Color.GREY);
            }
        }
        catch (Exception exception) {
            Utils.runOnUIThread(() -> Utils.showExceptionDialog(exception));
        }
    }

    private void onClosePort() {
        try {
            this.serialPortController.closeSerialPort();
            this.portIsOpened = !this.portIsOpened;

            bytesReceived.setText("0");
            bytesSent.setText("0");
            textXonXoff.setText("");
            packageArea.clear();

            sourceAddressList.setDisable(this.portIsOpened);
            openPortButton.setDisable(this.portIsOpened);
            closePortButton.setDisable(!this.portIsOpened);
            sendXoffButton.setDisable(!this.portIsOpened);
            sendXonButton.setDisable(!this.portIsOpened);
            portList.setDisable(this.portIsOpened);
            flowControlXonXoffCheck.setDisable(this.portIsOpened);
        }
        catch (Exception exception) {
            Utils.runOnUIThread(() -> Utils.showExceptionDialog(exception));
        }
    }
}

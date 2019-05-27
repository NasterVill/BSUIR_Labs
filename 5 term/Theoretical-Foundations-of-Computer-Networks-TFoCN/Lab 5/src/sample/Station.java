package sample;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.VPos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class Station{
    private byte monitor;
    private byte sourceAddress;
    private byte destinationAddress;

    private String[] destinations = { "1", "10", "100" };
    private String destination = "";

    private TextArea inputArea;
    private TextArea outputArea;
    private Text tokenText;
    private Text sourceText;
    private Button send;
    private Button start;

    Station(byte source, byte dest,byte monitor){
        this.sourceAddress = source;
        this.destinationAddress = dest;
        this.monitor = monitor;

        sourceText =new Text();

        if(source == 1)
            sourceText.setText("1");
        else if(source == 10)
            sourceText.setText("10");
        else if (source == 100)
            sourceText.setText("100");
        this.monitor = monitor;
    }

    public byte getSourceAddress(){
        return sourceAddress;
    }

    public byte getDestinationAddress(){
        return destinationAddress;
    }

    public byte getMonitor(){
        return monitor;
    }

    public Button getSendButton(){
        return send;
    }

    public Button getStartButton(){
        return start;
    }

    public TextArea getOutputArea(){
        return outputArea;
    }

    public Text getTokenText(){
        return tokenText;
    }
    public TextArea getInputArea(){
        return inputArea;
    }

    public String getDestination() {
        return destination;
    }

    public void setDestinationAddress(byte dest){
        destinationAddress = dest;
    }

    public void start(Stage stage) {
        try {
            VBox rootLayout = new VBox();
            rootLayout.setPadding(new Insets(5));
            rootLayout.setSpacing(5);
            rootLayout.setFillWidth(true);

            Label labelInput = new Label("Input");
            rootLayout.getChildren().add(labelInput);

            inputArea = new TextArea();
            inputArea.setPrefWidth(250);
            inputArea.setMaxHeight(30);
            rootLayout.getChildren().add(inputArea);

            Label labelDestAddress = new Label("Destination: ");
            rootLayout.getChildren().add(labelDestAddress);
            ObservableList<String> items = FXCollections.observableArrayList(destinations);
            ComboBox<String> destinationsList = new ComboBox<>(items);
            destinationsList.setPromptText("Destination addresses");
            destinationsList.valueProperty().
                    addListener((observableValue, previous, current) ->
                            destination = current);
            destinationsList.setMinHeight(30);
            destinationsList.setPrefWidth(300);
            rootLayout.getChildren().add(destinationsList);

            Label labelOutput = new Label("Output");
            rootLayout.getChildren().add(labelOutput);

            outputArea = new TextArea();
            outputArea.setEditable(false);
            outputArea.setPrefHeight(100);
            outputArea.setWrapText(true);
            rootLayout.getChildren().add(outputArea);

            Label labelDebug = new Label("Debug: ");
            rootLayout.getChildren().add(labelDebug);

            final Separator separatorOne = new Separator();
            separatorOne.setPrefWidth(300);
            separatorOne.setValignment(VPos.CENTER);
            rootLayout.getChildren().add(separatorOne);

            tokenText = new Text();
            rootLayout.getChildren().add(tokenText);

            final Separator separatorTwo = new Separator();
            separatorTwo.setPrefWidth(300);
            separatorTwo.setValignment(VPos.CENTER);
            rootLayout.getChildren().add(separatorTwo);

            Label labelSourceAddress = new Label("Source: ");
            rootLayout.getChildren().add(labelSourceAddress);

            final Separator separatorThree = new Separator();
            separatorThree.setPrefWidth(300);
            separatorThree.setValignment(VPos.CENTER);

            rootLayout.getChildren().add(separatorThree);
            rootLayout.getChildren().add(sourceText);

            final Separator separatorFour = new Separator();
            separatorFour.setPrefWidth(300);
            separatorFour.setValignment(VPos.CENTER);
            rootLayout.getChildren().add(separatorFour);

            start = new Button("Start");
            start.setMinHeight(30);
            start.setPrefWidth(300);
            if(monitor == 1)
                rootLayout.getChildren().add(start);

            Scene scene = new Scene(rootLayout, 300, 400);
            stage.setScene(scene);
            stage.setResizable(false);
            stage.show();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
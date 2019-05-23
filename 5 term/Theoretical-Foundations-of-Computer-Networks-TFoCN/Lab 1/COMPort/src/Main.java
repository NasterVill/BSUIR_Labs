import gui.MainView;
import javafx.application.Application;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import controller.SerialPortController;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import util.Utils;


public class Main extends Application  {
    private SerialPortController serialPortController;
    private SerialPortEventListener serialPortEventListener;

    private MainView mainView;

    private class SerialPortListener implements SerialPortEventListener {
        public void serialEvent(SerialPortEvent event) {
            if (event.isRXCHAR() && event.getEventValue() > 0) {
                try {
                    byte[] readBytes;

                    if(serialPortController.getXonXoffFlowControlMode()) {
                        readBytes = serialPortController.read(event.getEventValue());

                        boolean containsXonXoff = (readBytes[0] == SerialPortController.XOFF_CHARACTER ||
                                readBytes[0] == SerialPortController.XON_CHARACTER);
                        if(containsXonXoff) {
                            boolean value = readBytes[0] == SerialPortController.XOFF_CHARACTER;
                            serialPortController.setXoffState(value);

                            if(value) {
                                Utils.runOnUIThread(() -> {
                                    mainView.getTextXonXoff().setText(MainView.XOFF_IS_ON_MESSAGE);
                                    mainView.getTextXonXoff().setFill(Color.RED);
                                });
                            } else {
                                Utils.runOnUIThread(() -> {
                                    mainView.getTextXonXoff().setText(MainView.XON_IS_ON_MESSAGE);
                                    mainView.getTextXonXoff().setFill(Color.valueOf("#00c800"));
                                });
                            }
                        }

                        if(!containsXonXoff && event.getEventValue() > 0) {
                            serialPortController.sendXoff();

                            String result = "collocutor> " + new String(readBytes) + "\n";
                            Utils.runOnUIThread(() -> {
                                mainView.getOutputArea().appendText(result);
                                mainView.getBytesReceivedField().setText(Integer.toString(serialPortController.getReceivedBytes()));
                            });

                            serialPortController.sendXon();
                        }
                    } else {
                        Utils.runOnUIThread(() -> {
                            mainView.getTextXonXoff().setText(MainView.XON_XOFF_IS_NOT_SUPPORTED);
                            mainView.getTextXonXoff().setFill(Color.GREY);
                        });

                        readBytes = serialPortController.read(event.getEventValue());

                        if (readBytes != null) {
                            String result = "collocutor> " + new String(readBytes) + "\n";
                            Utils.runOnUIThread(() -> {
                                mainView.getOutputArea().appendText(result);
                                mainView.getBytesReceivedField().setText(Integer.toString(serialPortController.getReceivedBytes()));
                            });
                        }
                    }
                } catch (Exception exception) {
                    Utils.showExceptionDialog(exception);
                }
            }
        }
    }

    /**
     * This method is used to set up all the labels, buttons and
     * and other graphical objects, which the user will
     * interact with during the program's execution
     */
    @Override
    public void init() {
        serialPortEventListener = new SerialPortListener();
        serialPortController = new SerialPortController(serialPortEventListener);
        mainView = new MainView(serialPortController);
    }

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("COM Port");
        primaryStage.setResizable(false);
        primaryStage.setScene(mainView.getScene());

        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
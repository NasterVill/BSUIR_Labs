package main;

import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;

public class Controller {
    private static final int MAX_ATTEMPTS = 10;
    private static final char COLLISION_SYMBOL = 'X';
    private static final int COLLISION_WINDOW_DURATION = 500;

    private byte buffer = 0;

    @FXML private Button sendButton;
    @FXML private Button clearButton;
    @FXML private TextArea inputArea;
    @FXML private TextArea outputArea;
    @FXML private TextArea debugArea;

    private void send(byte data) {
        this.buffer = data;
    }

    private boolean generateBoolean() {
        return (System.currentTimeMillis() % 2) == 1;
    }

    private boolean isChannelBusy() {
        return generateBoolean();
    }

    private boolean isCollision() {
        return generateBoolean();
    }

    private void sleep(int millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private int computeDelay(int n, int max) {
        return (int)Math.round(Math.random() * Math.pow(2, Math.min(n, max)));
    }

    private static void runOnUIThread(Runnable task) {
        if(task == null) throw new NullPointerException("Param task can not be null");

        if(Platform.isFxApplicationThread()) task.run();
        else Platform.runLater(task);
    }

    private class SendTask extends Task<Void> {
        @Override
        protected Void call() {
            runOnUIThread(() -> {
                inputArea.setEditable(false);
                sendButton.setDisable(true);
                clearButton.setDisable(true);
            });

            byte[] message = inputArea.getText().getBytes();

            for (byte symbol: message) {
                int attempts = 0;
                boolean isSending = true;
                StringBuilder collisions = new StringBuilder();

                while(isSending) {
                    while (isChannelBusy());
                    send(symbol);

                    sleep(COLLISION_WINDOW_DURATION);

                    if(isCollision()) {
                        collisions.append(COLLISION_SYMBOL);
                        attempts += 1;

                        if (attempts > MAX_ATTEMPTS) {
                            runOnUIThread(() -> debugArea.appendText(collisions + "\n"));
                            isSending = false;
                        } else {
                            sleep(computeDelay(attempts, MAX_ATTEMPTS));
                        }
                    } else {
                        runOnUIThread(() -> {
                            outputArea.appendText((char)symbol + "");
                            debugArea.appendText(collisions + "\n");
                        });
                        isSending = false;
                    }
                }
            }
            runOnUIThread(() -> {
                inputArea.setEditable(true);
                sendButton.setDisable(false);
                clearButton.setDisable(false);
            });
            return null;
        }
    }

    @FXML protected void onClear(ActionEvent event) {
        inputArea.setText("");
        outputArea.setText("");
        debugArea.setText("");
    }

    @FXML protected void onSend(ActionEvent event) {
        new Thread(new SendTask()).start();
    }
}
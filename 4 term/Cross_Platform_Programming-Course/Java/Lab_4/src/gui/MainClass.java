package gui;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.control.TextArea;
import javafx.scene.control.Alert;
import java.io.PrintStream;
import javafx.scene.layout.VBox;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import logic.entities.ChopStick;
import logic.entities.Seat;
import logic.man.Philosopher;
import logic.man.Waiter;
import java.util.concurrent.*;
import java.util.ArrayList;

/**<h1>MainClass</h1>
 *
 * The MainClass class represents the program data
 * structure, which is being manipulated by the
 * user with the help of GUI class.
 */
public class MainClass extends Application {
    private VBox rootLayout;
    private GridPane gridpane;
    private Label philosopherslabel;
    private Label sticksAndSeatslabel;
    private TextField philosophersAmountField;
    private TextField sticksAmountField;
    private Button eatButton;
    private Button clearButton;
    private TextArea textArea;
    private CustomConsole console;

    /**
     * This method is used to set up all the labels, buttons and
     * and other graphical objects, which the user will
     * interact with during the program's execution
     */
    @Override
    public void init() {
        rootLayout = new VBox();
        rootLayout.setPadding(new Insets(10));
        rootLayout.setSpacing(5);

        gridpane = new GridPane();
        gridpane.setPadding(new Insets(0));
        gridpane.setHgap(5);
        gridpane.setVgap(5);

        philosopherslabel = new Label("Amount of philosophers: ");
        gridpane.add(philosopherslabel, 0, 0);
        philosophersAmountField = new TextField();
        gridpane.add(philosophersAmountField, 1, 0);

        sticksAndSeatslabel = new Label("Amount of seats/chopsticks: ");
        gridpane.add(sticksAndSeatslabel, 0, 1);
        sticksAmountField = new TextField();
        gridpane.add(sticksAmountField, 1, 1);

        rootLayout.getChildren().add(gridpane);

        eatButton = new Button("Eat");
        eatButton.setPrefWidth(340);
        rootLayout.getChildren().add(eatButton);

        textArea = new TextArea();
        textArea.setWrapText(true);
        rootLayout.getChildren().add(textArea);

        console = new CustomConsole(textArea);
        PrintStream printStream = new PrintStream(console, true);
        System.setOut(printStream);
        System.setErr(printStream);

        clearButton = new Button("Clear");
        clearButton.setPrefWidth(340);
        rootLayout.getChildren().add(clearButton);

        eatButton.setOnAction(event -> {
            textArea.clear();
            eatButton.setDisable(true);
            try {
                int delayTime = 10;

                int amountOfPhilosophers = Integer.parseInt(philosophersAmountField.getText());
                if(amountOfPhilosophers < 1 || amountOfPhilosophers > 100) {
                    String message;
                    if (amountOfPhilosophers < 1) {
                        message = "The value of param amountOfPhilosophers must be positive";
                    } else {
                        message = "The amount of philosophers, bigger than 100, is restricted.";
                    }
                    Alert alert = new Alert(Alert.AlertType.WARNING);
                    alert.setTitle("Warning Dialog");
                    alert.setHeaderText("Wrong input data!");
                    alert.setContentText("Description: " + message);
                    alert.showAndWait();
                    eatButton.setDisable(false);
                    return;
                }

                int amountOfSticks = Integer.parseInt(sticksAmountField.getText());
                if(amountOfSticks < 2) {
                    Alert alert = new Alert(Alert.AlertType.WARNING);
                    alert.setTitle("Warning Dialog");
                    alert.setHeaderText("Wrong input data!");
                    alert.setContentText("Description: The value of param amountOfSticks must be greater than 1");
                    alert.showAndWait();
                    eatButton.setDisable(false);
                    return;
                }

                ExecutorService exec = Executors.newCachedThreadPool();
                ChopStick[] sticks = new ChopStick[amountOfSticks];
                Seat[] seats = new Seat[amountOfSticks];

                for (int i = 0; i < amountOfSticks; i++) {
                    sticks[i] = new ChopStick();
                }

                for (int i = 0; i < amountOfSticks; i++) {
                    seats[i] = new Seat();
                }

                Waiter waiter = new Waiter(sticks, seats);

                ArrayList<Callable<Object>> philosopherList = new ArrayList<>();
                for (int i = 0; i < amountOfPhilosophers; i++) {
                    philosopherList.add(Executors.callable(new Philosopher(
                            i, delayTime, waiter)));
                }

                exec.invokeAll(philosopherList);
                exec.shutdown();

                System.out.println("Every philosopher has finished eating!");
            } catch (Exception exception) {
                Alert alert = new Alert(Alert.AlertType.WARNING);
                alert.setTitle("Warning Dialog");
                alert.setHeaderText("Exception caught!");
                alert.setContentText("Description: " + exception.getMessage());

                alert.showAndWait();
            }
            eatButton.setDisable(false);
        });

        clearButton.setOnAction(event -> textArea.clear());
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Dining Philosophers");
        primaryStage.setResizable(false);
        primaryStage.setScene(new Scene(rootLayout, 340, 300));
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
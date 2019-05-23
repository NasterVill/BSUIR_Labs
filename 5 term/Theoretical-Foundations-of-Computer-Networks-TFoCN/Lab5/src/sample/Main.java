package sample;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

public class Main extends Application {
    private static final int DELAY = 1000;
    private static final int PERIOD = 1000;
    private static final String MARKER = "*";

    private Timer timer;
    private Package _package;
    private byte addresses[] = { 1 , 10, 100 };


    public static void main(String[] args){
        launch(args);
    }

    @Override
    public void start(Stage stage) {
        ArrayList<Station> stations = new ArrayList<>();
        stations.add(new Station(addresses[0],addresses[1],(byte)1));
        stations.add(new Station(addresses[1],addresses[2], (byte)0));
        stations.add(new Station(addresses[2], addresses[0], (byte)0));

        stage = new Stage();
        Stage stage2 = new Stage();
        Stage stage3 = new Stage();

        stage.setX(440);
        stage.setY(320);
        stage2.setX(790);
        stage2.setY(320);
        stage3.setX(1140);
        stage3.setY(320);

        stations.get(0).start(stage);
        stations.get(1).start(stage2);
        stations.get(2).start(stage3);

        timer = new Timer();

        stations.get(0).getStartButton().setOnAction((event) -> {
            _package = new Package();
            for (int i = 0; i < 3; i++) {
                int currentStation = i;

                timer.schedule(new TimerTask() {
                    @Override
                    public void run() {
                        System.out.println(currentStation);
                        stationRoutine(stations.get(currentStation), _package);
                    }
                }, DELAY, PERIOD);

                stations.get(0).getStartButton().setDisable(true);
            }
        });

        stations.get(0).getTokenText().textProperty().addListener(observable -> sendPackage(stations.get(0)));
        stations.get(1).getTokenText().textProperty().addListener(observable -> sendPackage(stations.get(1)));
        stations.get(2).getTokenText().textProperty().addListener(observable -> sendPackage(stations.get(2)));
    }

    public static void stationRoutine(Station station, Package _package){
        try {
            if (_package.getControl() == 0) {
                _package.setSource(station.getSourceAddress());
                _package.setDestination(station.getDestinationAddress());
                _package.setMonitor(station.getMonitor());
            } else {
                runOnUIThread(() -> {
                    if (_package.getDestination() == station.getSourceAddress()) {
                        String data = "";
                        data += (char)_package.getData();

                        _package.setStatus((byte) 1);
                        station.getOutputArea().appendText(data);
                    }
                    if (_package.getStatus() == 1 && station.getMonitor() == 1) {
                        _package.setControl((byte) 0);
                        _package.freeData();
                        _package.setStatus((byte) 0);
                    }
                });
            }
            runOnUIThread(() -> station.getTokenText().setText(MARKER));
            Thread.sleep(DELAY);
            runOnUIThread(() -> station.getTokenText().setText(""));

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void sendPackage(Station station) {
        if (!station.getInputArea().getText().equals("")
                && !station.getDestination().equals("")
                && _package.getControl() == 0
                && _package != null) {
            if (station.getTokenText().getText().equals("*")) {
                if (station.getDestination().equals("1")) {
                    station.setDestinationAddress(addresses[0]);
                }
                if (station.getDestination().equals("10")) {
                    station.setDestinationAddress(addresses[1]);
                }
                if (station.getDestination().equals("100")) {
                    station.setDestinationAddress(addresses[2]);
                }

                _package.setControl((byte) 1);
                _package.setDestination(station.getDestinationAddress());
                _package.setSource(station.getSourceAddress());
                _package.setMonitor(station.getMonitor());

                String reduced = station.getInputArea().getText().substring(1);
                _package.setData(station.getInputArea().getText().getBytes()[0]);
                station.getInputArea().setText(reduced);
            }
        }
    }

    private static void runOnUIThread(Runnable task) {
        if(task == null) throw new NullPointerException("Param task can not be null");

        if(Platform.isFxApplicationThread()) task.run();
        else Platform.runLater(task);
    }
}
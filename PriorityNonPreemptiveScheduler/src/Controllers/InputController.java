package Controllers;

import Models.ProcessHolder;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.event.ActionEvent;
import Models.Process;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class InputController {

    @FXML
    private TextField numProcessesField;

    @FXML
    private VBox processFieldsContainer;

    // List to hold all user inputs
    private final List<HBox> processRows = new ArrayList<>();

    @FXML
    public void generateFields(ActionEvent event) {
        processFieldsContainer.getChildren().clear();
        processRows.clear();

        int count;
        try {
            count = Integer.parseInt(numProcessesField.getText());
            if (count <= 0) throw new NumberFormatException();
        } catch (NumberFormatException e) {
            showAlert("Invalid number of processes. Please enter a positive integer.");
            return;
        }

        for (int i = 1; i <= count; i++) {
            HBox row = new HBox(10);
            row.getChildren().addAll(
                    new Label("P" + i),

                    new Label("Arrival:"),
                    new TextField(),

                    new Label("Burst:"),
                    new TextField(),

                    new Label("Priority:"),
                    new TextField()
            );
            processRows.add(row);
            processFieldsContainer.getChildren().add(row);
        }
    }

    @FXML
    public void simulateScheduling(ActionEvent event) throws IOException {
        List<Process> processes = new ArrayList<>();

        for (int i = 0; i < processRows.size(); i++) {
            HBox row = processRows.get(i);
            String id = "P" + (i + 1);
            try {
                int arrival = Integer.parseInt(((TextField) row.getChildren().get(2)).getText());
                int burst = Integer.parseInt(((TextField) row.getChildren().get(4)).getText());
                int priority = Integer.parseInt(((TextField) row.getChildren().get(6)).getText());

                if (arrival < 0 || burst <= 0 || priority < 0)
                    throw new NumberFormatException();

                processes.add(new Process(id, arrival, burst, priority));
            } catch (NumberFormatException e) {
                showAlert("Invalid input in process " + id + ". Make sure all values are positive integers.");
                return;
            }
        }

        // Pass process list to result page (You can use a static holder or Singleton)
        ProcessHolder.setProcesses(processes);

        Main.switchScene("/View/ResultPage.fxml");
    }

    private void showAlert(String msg) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Input Error");
        alert.setContentText(msg);
        alert.showAndWait();
    }
}

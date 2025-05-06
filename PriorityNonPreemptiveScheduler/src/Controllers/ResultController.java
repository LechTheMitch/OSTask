package Controllers;

import Models.ProcessHolder;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import Models.Process;

import javafx.scene.control.cell.PropertyValueFactory;
import java.io.IOException;
import java.util.*;

public class ResultController {

    @FXML private TableView<Process> tableView;
    @FXML private TableColumn<Process, String> pidCol;
    @FXML private TableColumn<Process, Integer> arrivalCol, burstCol, priorityCol;
    @FXML private TableColumn<Process, Integer> startCol, completionCol, waitingCol, turnaroundCol, responseCol;

    @FXML private HBox ganttChartBox;
    @FXML private Label avgWT, avgTAT, avgRT;

    @FXML
    public void initialize() {
        List<Process> processList = new ArrayList<>(ProcessHolder.getProcesses());

        // Sort by arrival, then priority
        processList.sort(Comparator.comparingInt((Process p) -> p.arrivalTime)
                .thenComparingInt(p -> p.priority));

        int time = 0;
        for (Process p : processList) {
            if (time < p.arrivalTime) time = p.arrivalTime;
            p.startTime = time;
            p.completionTime = time + p.burstTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.startTime - p.arrivalTime;
            p.responseTime = p.waitingTime;
            time += p.burstTime;
        }

        setupTable(processList);
        drawGanttChart(processList);
        showAverages(processList);
    }

    private void setupTable(List<Process> processes) {
        pidCol.setCellValueFactory(new PropertyValueFactory<>("id"));
        arrivalCol.setCellValueFactory(new PropertyValueFactory<>("arrivalTime"));
        burstCol.setCellValueFactory(new PropertyValueFactory<>("burstTime"));
        priorityCol.setCellValueFactory(new PropertyValueFactory<>("priority"));
        startCol.setCellValueFactory(new PropertyValueFactory<>("startTime"));
        completionCol.setCellValueFactory(new PropertyValueFactory<>("completionTime"));
        waitingCol.setCellValueFactory(new PropertyValueFactory<>("waitingTime"));
        turnaroundCol.setCellValueFactory(new PropertyValueFactory<>("turnaroundTime"));
        responseCol.setCellValueFactory(new PropertyValueFactory<>("responseTime"));

        tableView.getItems().setAll(processes);
    }

    private void drawGanttChart(List<Process> processes) {
        ganttChartBox.getChildren().clear();
        for (Process p : processes) {
            VBox box = new VBox(5);
            Rectangle rect = new Rectangle(60, 30);
            rect.setFill(Color.LIGHTBLUE);
            rect.setStroke(Color.BLACK);

            Label pidLabel = new Label(p.id);
            pidLabel.setStyle("-fx-font-weight: bold");

            Label timeLabel = new Label(String.valueOf(p.startTime));
            box.getChildren().addAll(rect, pidLabel, timeLabel);
            ganttChartBox.getChildren().add(box);
        }

        // Add end time
        Process last = processes.get(processes.size() - 1);
        ganttChartBox.getChildren().add(new Label(String.valueOf(last.completionTime)));
    }

    private void showAverages(List<Process> list) {
        double totalWT = 0, totalTAT = 0, totalRT = 0;
        for (Process p : list) {
            totalWT += p.waitingTime;
            totalTAT += p.turnaroundTime;
            totalRT += p.responseTime;
        }
        int n = list.size();
        avgWT.setText("Avg WT: " + String.format("%.2f", totalWT / n));
        avgTAT.setText("Avg TAT: " + String.format("%.2f", totalTAT / n));
        avgRT.setText("Avg RT: " + String.format("%.2f", totalRT / n));
    }

    @FXML
    public void goBack() throws IOException {
        Main.switchScene("/View/ProcessInputPage.fxml");
    }
}

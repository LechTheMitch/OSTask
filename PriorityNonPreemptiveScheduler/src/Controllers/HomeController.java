package Controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import java.io.IOException;

public class HomeController {
    @FXML
    private Button SMButton;
    @FXML
    private Button OHButton;

    public void startSimulation(ActionEvent event) throws IOException {
        Main.switchScene("/View/ProcessInputPage.fxml");
    }
    public void openOptionsMenu(ActionEvent event) throws IOException {
        Main.switchScene("/View/HomePage.fxml");
    }
}
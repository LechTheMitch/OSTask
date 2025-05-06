package Models;

import java.util.List;

public class ProcessHolder {
    private static List<Process> processes;

    public static void setProcesses(List<Process> list) {
        processes = list;
    }

    public static List<Process> getProcesses() {
        return processes;
    }
}

package Models;

public class Process {
    public String id;
    public int arrivalTime;
    public int burstTime;
    public int priority;

    // Computed values
    public int startTime;
    public int completionTime;
    public int waitingTime;
    public int turnaroundTime;
    public int responseTime;

    public Process(String id, int arrivalTime, int burstTime, int priority) {
        this.id = id;
        this.arrivalTime = arrivalTime;
        this.burstTime = burstTime;
        this.priority = priority;
    }

    public void setId(String id) {
        this.id = id;
    }
    public String getId() {
        return id;
    }
    public void setWaitingTime(int waitingTime) {
        this.waitingTime = waitingTime;
    }
    public int getArrivalTime() {
        return arrivalTime;
    }
    public int getCompletionTime() {
        return completionTime;
    }
    public int getResponseTime() {
        return responseTime;
    }
    public int getTurnaroundTime() {
        return turnaroundTime;
    }
    public int getWaitingTime() {
        return waitingTime;
    }
    public int getBurstTime() {
        return burstTime;
    }
    public int getPriority() {
        return priority;
    }
    public int getStartTime() {
        return startTime;
    }
    public void setArrivalTime(int arrivalTime) {
        this.arrivalTime = arrivalTime;
    }
    public void setBurstTime(int burstTime) {
        this.burstTime = burstTime;
    }
    public void setCompletionTime(int completionTime) {
        this.completionTime = completionTime;
    }
    public void setResponseTime(int responseTime) {
        this.responseTime = responseTime;
    }
    public void setPriority(int priority) {
        this.priority = priority;
    }
    public void setStartTime(int startTime) {
        this.startTime = startTime;
    }
    public void setTurnaroundTime(int turnaroundTime) {
        this.turnaroundTime = turnaroundTime;
    }

}

#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
using namespace std;

struct Process {
    int pid; // Process ID
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
};

void calculate_metrics(vector<Process>& processes) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (auto& p : processes) {
        total_waiting_time += p.waiting_time;
        total_turnaround_time += p.turnaround_time;
    }

    double avg_waiting_time = (double)total_waiting_time / processes.size();
    double avg_turnaround_time = (double)total_turnaround_time / processes.size();

    cout << "\nPerformance Metrics:\n";
    cout << "Average Waiting Time: " << avg_waiting_time << "\n";
    cout << "Average Turnaround Time: " << avg_turnaround_time << "\n";
}

void display_gantt_chart(const vector<pair<int, int>>& gantt_chart) {
    cout << "\nGantt Chart:\n";
    for (const auto& slot : gantt_chart) {
        cout << "| P" << slot.first << " " << setw(3) << "";
    }
    cout << "|\n";

    for (const auto& slot : gantt_chart) {
        cout << setw(3) << slot.second << "    ";
    }
    cout << gantt_chart.back().second + 1 << "\n";
}

void fcfs(vector<Process> processes) {
    cout << "\nSimulating FCFS...\n";
    vector<pair<int, int>> gantt_chart;
    int current_time = 0;

    for (auto& p : processes) {
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        p.waiting_time = current_time - p.arrival_time;
        p.turnaround_time = p.waiting_time + p.burst_time;

        gantt_chart.push_back({p.pid, current_time});
        current_time += p.burst_time;
    }

    display_gantt_chart(gantt_chart);
    calculate_metrics(processes);
}

void sjf(vector<Process> processes) {
    cout << "\nSimulating SJF...\n";
    vector<pair<int, int>> gantt_chart;
    int current_time = 0;

    while (!processes.empty()) {
        vector<Process>::iterator shortest = processes.end();
        for (auto it = processes.begin(); it != processes.end(); ++it) {
            if (it->arrival_time <= current_time && (shortest == processes.end() || it->burst_time < shortest->burst_time)) {
                shortest = it;
            }
        }

        if (shortest == processes.end()) {
            ++current_time;
            continue;
        }

        Process p = *shortest;
        processes.erase(shortest);

        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        p.waiting_time = current_time - p.arrival_time;
        p.turnaround_time = p.waiting_time + p.burst_time;

        gantt_chart.push_back({p.pid, current_time});
        current_time += p.burst_time;
    }

    display_gantt_chart(gantt_chart);
    calculate_metrics(processes);
}

void round_robin(vector<Process> processes, int quantum) {
    cout << "\nSimulating Round Robin...\n";
    vector<pair<int, int>> gantt_chart;
    queue<Process> ready_queue;
    int current_time = 0;

    for (const auto& p : processes) {
        ready_queue.push(p);
    }

    while (!ready_queue.empty()) {
        Process p = ready_queue.front();
        ready_queue.pop();

        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }
        int execute_time = min(quantum, p.burst_time);
        gantt_chart.push_back({p.pid, current_time});

        current_time += execute_time;
        p.burst_time -= execute_time;

        if (p.burst_time > 0) {
            ready_queue.push(p);
        } else {
            p.turnaround_time = current_time - p.arrival_time;
            p.waiting_time = p.turnaround_time - (p.burst_time + execute_time);
        }
    }

    display_gantt_chart(gantt_chart);
    calculate_metrics(processes);
}

int main() {
    int n, quantum;
    vector<Process> processes;

    cout << "Enter the number of processes: ";
    cin >> n;

    for (int i = 0; i < n; ++i) {
        Process p;
        p.pid = i + 1;
        cout << "\nEnter arrival time for Process " << p.pid << ": ";
        cin >> p.arrival_time;
        cout << "Enter burst time for Process " << p.pid << ": ";
        cin >> p.burst_time;
        processes.push_back(p);
    }

    cout << "\nEnter time quantum for Round Robin: ";
    cin >> quantum;

    cout << "\nChoose Scheduling Algorithm:\n";
    cout << "1. FCFS\n2. SJF\n3. Round Robin\n";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            fcfs(processes);
            break;
        case 2:
            sjf(processes);
            break;
        case 3:
            round_robin(processes, quantum);
            break;
        default:
            cout << "Invalid choice!\n";
    }

    return 0;
}

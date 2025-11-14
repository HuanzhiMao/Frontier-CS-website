#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int job, machine, duration, step;
};

int J, M;
vector<vector<pair<int, int>>> jobs; // jobs[j] = [(machine, duration), ...]
vector<vector<int>> machine_order;

int calculate_makespan() {
    vector<int> job_next_step(J, 0);
    vector<int> machine_next_pos(M, 0);
    vector<int> job_finish_time(J, 0);
    vector<int> machine_finish_time(M, 0);
    
    bool progress = true;
    while (progress) {
        progress = false;
        
        for (int m = 0; m < M; m++) {
            if (machine_next_pos[m] < J) {
                int job = machine_order[m][machine_next_pos[m]];
                int step = job_next_step[job];
                
                if (step < M && jobs[job][step].first == m) {
                    // This job is ready for this machine
                    int start_time = max(job_finish_time[job], machine_finish_time[m]);
                    int duration = jobs[job][step].second;
                    int finish_time = start_time + duration;
                    
                    job_finish_time[job] = finish_time;
                    machine_finish_time[m] = finish_time;
                    job_next_step[job]++;
                    machine_next_pos[m]++;
                    progress = true;
                }
            }
        }
    }
    
    int makespan = 0;
    for (int j = 0; j < J; j++) {
        makespan = max(makespan, job_finish_time[j]);
    }
    return makespan;
}

void local_search() {
    int current_makespan = calculate_makespan();
    bool improved = true;
    
    while (improved) {
        improved = false;
        
        for (int m = 0; m < M; m++) {
            for (int i = 0; i < J - 1; i++) {
                // Try swapping adjacent jobs
                swap(machine_order[m][i], machine_order[m][i + 1]);
                int new_makespan = calculate_makespan();
                
                if (new_makespan < current_makespan) {
                    current_makespan = new_makespan;
                    improved = true;
                } else {
                    // Revert swap
                    swap(machine_order[m][i], machine_order[m][i + 1]);
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> J >> M;
    
    jobs.resize(J);
    machine_order.resize(M);
    
    for (int j = 0; j < J; j++) {
        jobs[j].resize(M);
        for (int k = 0; k < M; k++) {
            int machine, duration;
            cin >> machine >> duration;
            jobs[j][k] = {machine, duration};
        }
    }
    
    // Initialize with a simple heuristic: process jobs in order 0,1,2,...,J-1 on each machine
    for (int m = 0; m < M; m++) {
        machine_order[m].resize(J);
        iota(machine_order[m].begin(), machine_order[m].end(), 0);
    }
    
    // Try different initial orderings and pick the best
    int best_makespan = calculate_makespan();
    vector<vector<int>> best_order = machine_order;
    
    // Try reverse order
    for (int m = 0; m < M; m++) {
        reverse(machine_order[m].begin(), machine_order[m].end());
    }
    int makespan = calculate_makespan();
    if (makespan < best_makespan) {
        best_makespan = makespan;
        best_order = machine_order;
    }
    
    // Try random shuffle a few times
    for (int iter = 0; iter < 10; iter++) {
        for (int m = 0; m < M; m++) {
            random_shuffle(machine_order[m].begin(), machine_order[m].end());
        }
        makespan = calculate_makespan();
        if (makespan < best_makespan) {
            best_makespan = makespan;
            best_order = machine_order;
        }
    }
    
    machine_order = best_order;
    
    // Apply local search
    local_search();
    
    // Output the result
    for (int m = 0; m < M; m++) {
        for (int j = 0; j < J; j++) {
            cout << machine_order[m][j];
            if (j < J - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
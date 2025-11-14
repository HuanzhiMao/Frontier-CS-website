#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int job, machine, duration, pos;
};

int J, M;
vector<vector<Operation>> jobs;
vector<vector<int>> machine_order;

int calculate_makespan(const vector<vector<int>>& order) {
    vector<int> job_time(J, 0);
    vector<int> machine_time(M, 0);
    
    vector<vector<int>> machine_queue(M);
    for (int m = 0; m < M; m++) {
        machine_queue[m] = order[m];
    }
    
    vector<int> machine_idx(M, 0);
    vector<int> job_pos(J, 0);
    
    int makespan = 0;
    bool progress = true;
    
    while (progress) {
        progress = false;
        
        for (int m = 0; m < M; m++) {
            if (machine_idx[m] < J) {
                int job = machine_queue[m][machine_idx[m]];
                
                if (job_pos[job] < M) {
                    Operation& op = jobs[job][job_pos[job]];
                    
                    if (op.machine == m) {
                        int start_time = max(job_time[job], machine_time[m]);
                        int end_time = start_time + op.duration;
                        
                        job_time[job] = end_time;
                        machine_time[m] = end_time;
                        makespan = max(makespan, end_time);
                        
                        job_pos[job]++;
                        machine_idx[m]++;
                        progress = true;
                    }
                }
            }
        }
    }
    
    return makespan;
}

void improve_local_search(vector<vector<int>>& order) {
    bool improved = true;
    int iterations = 0;
    
    while (improved && iterations < 1000) {
        improved = false;
        iterations++;
        
        for (int m = 0; m < M; m++) {
            for (int i = 0; i < J - 1; i++) {
                swap(order[m][i], order[m][i + 1]);
                int new_makespan = calculate_makespan(order);
                
                if (new_makespan < calculate_makespan(order)) {
                    improved = true;
                } else {
                    swap(order[m][i], order[m][i + 1]);
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> J >> M;
    
    jobs.resize(J);
    machine_order.resize(M);
    
    for (int j = 0; j < J; j++) {
        jobs[j].resize(M);
        for (int k = 0; k < M; k++) {
            int machine, duration;
            cin >> machine >> duration;
            jobs[j][k] = {j, machine, duration, k};
        }
    }
    
    // Initialize with SPT (Shortest Processing Time) heuristic
    for (int m = 0; m < M; m++) {
        vector<pair<int, int>> job_durations;
        
        for (int j = 0; j < J; j++) {
            for (int k = 0; k < M; k++) {
                if (jobs[j][k].machine == m) {
                    job_durations.push_back({jobs[j][k].duration, j});
                    break;
                }
            }
        }
        
        sort(job_durations.begin(), job_durations.end());
        
        for (int i = 0; i < J; i++) {
            machine_order[m].push_back(job_durations[i].second);
        }
    }
    
    // Try different heuristics and pick the best
    vector<vector<int>> best_order = machine_order;
    int best_makespan = calculate_makespan(best_order);
    
    // Try LPT (Longest Processing Time)
    for (int m = 0; m < M; m++) {
        vector<pair<int, int>> job_durations;
        
        for (int j = 0; j < J; j++) {
            for (int k = 0; k < M; k++) {
                if (jobs[j][k].machine == m) {
                    job_durations.push_back({jobs[j][k].duration, j});
                    break;
                }
            }
        }
        
        sort(job_durations.rbegin(), job_durations.rend());
        machine_order[m].clear();
        
        for (int i = 0; i < J; i++) {
            machine_order[m].push_back(job_durations[i].second);
        }
    }
    
    int lpt_makespan = calculate_makespan(machine_order);
    if (lpt_makespan < best_makespan) {
        best_makespan = lpt_makespan;
        best_order = machine_order;
    }
    
    // Apply local search improvement
    improve_local_search(best_order);
    
    // Output the result
    for (int m = 0; m < M; m++) {
        for (int j = 0; j < J; j++) {
            cout << best_order[m][j];
            if (j < J - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
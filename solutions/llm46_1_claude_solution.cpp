#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int job, machine, time, pos;
};

int J, M;
vector<vector<Operation>> jobs;
vector<vector<int>> machine_order;

int calculate_makespan(const vector<vector<int>>& order) {
    vector<int> job_time(J, 0);
    vector<int> machine_time(M, 0);
    
    vector<vector<int>> job_pos(J, vector<int>(M));
    for (int j = 0; j < J; j++) {
        for (int k = 0; k < M; k++) {
            job_pos[j][jobs[j][k].machine] = k;
        }
    }
    
    vector<vector<int>> completion(J, vector<int>(M));
    
    for (int m = 0; m < M; m++) {
        int current_time = 0;
        for (int job : order[m]) {
            int pos = job_pos[job][m];
            int start_time = current_time;
            if (pos > 0) {
                int prev_machine = jobs[job][pos-1].machine;
                start_time = max(start_time, completion[job][prev_machine]);
            }
            completion[job][m] = start_time + jobs[job][pos].time;
            current_time = completion[job][m];
        }
    }
    
    int makespan = 0;
    for (int j = 0; j < J; j++) {
        for (int m = 0; m < M; m++) {
            makespan = max(makespan, completion[j][m]);
        }
    }
    
    return makespan;
}

void improve_schedule() {
    bool improved = true;
    while (improved) {
        improved = false;
        for (int m = 0; m < M; m++) {
            for (int i = 0; i < J - 1; i++) {
                swap(machine_order[m][i], machine_order[m][i+1]);
                int new_makespan = calculate_makespan(machine_order);
                if (new_makespan < calculate_makespan(machine_order)) {
                    improved = true;
                } else {
                    swap(machine_order[m][i], machine_order[m][i+1]);
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> J >> M;
    
    jobs.resize(J, vector<Operation>(M));
    machine_order.resize(M, vector<int>(J));
    
    for (int j = 0; j < J; j++) {
        for (int k = 0; k < M; k++) {
            int m, p;
            cin >> m >> p;
            jobs[j][k] = {j, m, p, k};
        }
    }
    
    // Priority dispatching heuristic
    vector<vector<pair<int, int>>> machine_jobs(M);
    
    for (int j = 0; j < J; j++) {
        for (int k = 0; k < M; k++) {
            int m = jobs[j][k].machine;
            int remaining_time = 0;
            for (int l = k; l < M; l++) {
                remaining_time += jobs[j][l].time;
            }
            machine_jobs[m].push_back({j, remaining_time});
        }
    }
    
    for (int m = 0; m < M; m++) {
        sort(machine_jobs[m].begin(), machine_jobs[m].end(), 
             [](const pair<int,int>& a, const pair<int,int>& b) {
                 return a.second > b.second; // Longest remaining processing time first
             });
        
        for (int i = 0; i < J; i++) {
            machine_order[m][i] = machine_jobs[m][i].first;
        }
    }
    
    // Try to improve with local search
    for (int iter = 0; iter < 100; iter++) {
        int current_makespan = calculate_makespan(machine_order);
        bool improved = false;
        
        for (int m = 0; m < M && !improved; m++) {
            for (int i = 0; i < J - 1 && !improved; i++) {
                swap(machine_order[m][i], machine_order[m][i+1]);
                int new_makespan = calculate_makespan(machine_order);
                if (new_makespan < current_makespan) {
                    improved = true;
                } else {
                    swap(machine_order[m][i], machine_order[m][i+1]);
                }
            }
        }
        
        if (!improved) break;
    }
    
    for (int m = 0; m < M; m++) {
        for (int j = 0; j < J; j++) {
            if (j > 0) cout << " ";
            cout << machine_order[m][j];
        }
        cout << "\n";
    }
    
    return 0;
}
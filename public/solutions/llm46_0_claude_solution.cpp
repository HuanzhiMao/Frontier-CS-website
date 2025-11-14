#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int job, machine, time, order;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int J, M;
    cin >> J >> M;
    
    vector<vector<pair<int, int>>> jobs(J);
    vector<Operation> operations;
    
    for (int j = 0; j < J; j++) {
        for (int k = 0; k < M; k++) {
            int machine, time;
            cin >> machine >> time;
            jobs[j].push_back({machine, time});
            operations.push_back({j, machine, time, k});
        }
    }
    
    vector<vector<int>> machine_order(M);
    
    // Simple dispatching heuristic - process jobs in order of remaining work
    vector<int> remaining_work(J, 0);
    for (int j = 0; j < J; j++) {
        for (int k = 0; k < M; k++) {
            remaining_work[j] += jobs[j][k].second;
        }
    }
    
    // For each machine, order jobs by longest remaining processing time first
    for (int m = 0; m < M; m++) {
        vector<pair<int, int>> job_priority;
        for (int j = 0; j < J; j++) {
            job_priority.push_back({remaining_work[j], j});
        }
        sort(job_priority.rbegin(), job_priority.rend());
        
        for (auto& p : job_priority) {
            machine_order[m].push_back(p.second);
        }
    }
    
    // Try to improve with local search
    auto calculate_makespan = [&](const vector<vector<int>>& order) -> int {
        vector<int> job_time(J, 0);
        vector<int> machine_time(M, 0);
        
        // Process operations in topological order
        vector<vector<int>> job_pos(J, vector<int>(M));
        for (int j = 0; j < J; j++) {
            for (int k = 0; k < M; k++) {
                int machine = jobs[j][k].first;
                for (int pos = 0; pos < J; pos++) {
                    if (order[machine][pos] == j) {
                        job_pos[j][machine] = pos;
                        break;
                    }
                }
            }
        }
        
        vector<int> machine_job_idx(M, 0);
        vector<int> job_op_idx(J, 0);
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
        
        // Initialize with first operations of each job
        for (int j = 0; j < J; j++) {
            pq.push({0, {j, 0}});
        }
        
        int makespan = 0;
        while (!pq.empty()) {
            auto [start_time, op] = pq.top();
            pq.pop();
            
            int job = op.first;
            int op_idx = op.second;
            
            if (op_idx != job_op_idx[job]) continue;
            
            int machine = jobs[job][op_idx].first;
            int proc_time = jobs[job][op_idx].second;
            
            // Check if this job can be processed on this machine now
            bool can_process = true;
            for (int j2 = 0; j2 < J; j2++) {
                if (job_pos[j2][machine] < job_pos[job][machine] && 
                    machine_job_idx[machine] <= job_pos[j2][machine]) {
                    can_process = false;
                    break;
                }
            }
            
            if (!can_process) {
                pq.push({start_time + 1, {job, op_idx}});
                continue;
            }
            
            int actual_start = max(start_time, max(job_time[job], machine_time[machine]));
            int finish_time = actual_start + proc_time;
            
            job_time[job] = finish_time;
            machine_time[machine] = finish_time;
            machine_job_idx[machine]++;
            job_op_idx[job]++;
            
            makespan = max(makespan, finish_time);
            
            if (op_idx + 1 < M) {
                pq.push({finish_time, {job, op_idx + 1}});
            }
        }
        
        return makespan;
    };
    
    // Simple local search with adjacent swaps
    for (int iter = 0; iter < 100; iter++) {
        bool improved = false;
        for (int m = 0; m < M; m++) {
            for (int i = 0; i < J - 1; i++) {
                swap(machine_order[m][i], machine_order[m][i + 1]);
                if (calculate_makespan(machine_order) < calculate_makespan(machine_order)) {
                    improved = true;
                } else {
                    swap(machine_order[m][i], machine_order[m][i + 1]);
                }
            }
        }
        if (!improved) break;
    }
    
    for (int m = 0; m < M; m++) {
        for (int i = 0; i < J; i++) {
            cout << machine_order[m][i];
            if (i < J - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
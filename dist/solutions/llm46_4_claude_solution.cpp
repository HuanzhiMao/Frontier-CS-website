#include <bits/stdc++.h>
using namespace std;

struct Operation {
    int job, machine, duration;
};

struct Job {
    vector<int> machines;
    vector<int> durations;
};

int computeMakespan(const vector<Job>& jobs, const vector<vector<int>>& machineOrders) {
    int J = jobs.size();
    int M = machineOrders.size();
    
    vector<int> jobProgress(J, 0);
    vector<int> machineTime(M, 0);
    vector<int> jobTime(J, 0);
    
    // Create operation queue based on machine orders
    vector<queue<int>> machineQueues(M);
    for (int m = 0; m < M; m++) {
        for (int job : machineOrders[m]) {
            machineQueues[m].push(job);
        }
    }
    
    // Process operations
    bool progress = true;
    while (progress) {
        progress = false;
        for (int m = 0; m < M; m++) {
            if (!machineQueues[m].empty()) {
                int job = machineQueues[m].front();
                int opIdx = jobProgress[job];
                
                if (opIdx < M && jobs[job].machines[opIdx] == m) {
                    int startTime = max(machineTime[m], jobTime[job]);
                    int duration = jobs[job].durations[opIdx];
                    
                    machineTime[m] = startTime + duration;
                    jobTime[job] = startTime + duration;
                    jobProgress[job]++;
                    machineQueues[m].pop();
                    progress = true;
                }
            }
        }
    }
    
    return *max_element(machineTime.begin(), machineTime.end());
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int J, M;
    cin >> J >> M;
    
    vector<Job> jobs(J);
    for (int j = 0; j < J; j++) {
        jobs[j].machines.resize(M);
        jobs[j].durations.resize(M);
        for (int k = 0; k < M; k++) {
            cin >> jobs[j].machines[k] >> jobs[j].durations[k];
        }
    }
    
    // Simple heuristic: process jobs by earliest due date on each machine
    vector<vector<int>> machineOrders(M);
    
    for (int m = 0; m < M; m++) {
        vector<pair<int, int>> jobPriorities; // (priority, job)
        
        for (int j = 0; j < J; j++) {
            // Find when this job needs machine m
            int priority = 0;
            for (int k = 0; k < M; k++) {
                if (jobs[j].machines[k] == m) {
                    priority = k; // Earlier in job sequence = higher priority (lower value)
                    break;
                }
            }
            jobPriorities.push_back({priority, j});
        }
        
        sort(jobPriorities.begin(), jobPriorities.end());
        
        for (auto& p : jobPriorities) {
            machineOrders[m].push_back(p.second);
        }
    }
    
    // Try to improve with simple local search
    int bestMakespan = computeMakespan(jobs, machineOrders);
    vector<vector<int>> bestOrders = machineOrders;
    
    // Random restarts with different heuristics
    for (int restart = 0; restart < 5; restart++) {
        vector<vector<int>> currentOrders(M);
        
        for (int m = 0; m < M; m++) {
            vector<int> jobList(J);
            iota(jobList.begin(), jobList.end(), 0);
            
            if (restart == 0) {
                // SPT on this machine
                sort(jobList.begin(), jobList.end(), [&](int a, int b) {
                    int durA = 0, durB = 0;
                    for (int k = 0; k < M; k++) {
                        if (jobs[a].machines[k] == m) durA = jobs[a].durations[k];
                        if (jobs[b].machines[k] == m) durB = jobs[b].durations[k];
                    }
                    return durA < durB;
                });
            } else if (restart == 1) {
                // LPT on this machine
                sort(jobList.begin(), jobList.end(), [&](int a, int b) {
                    int durA = 0, durB = 0;
                    for (int k = 0; k < M; k++) {
                        if (jobs[a].machines[k] == m) durA = jobs[a].durations[k];
                        if (jobs[b].machines[k] == m) durB = jobs[b].durations[k];
                    }
                    return durA > durB;
                });
            } else {
                // Random shuffle
                random_shuffle(jobList.begin(), jobList.end());
            }
            
            currentOrders[m] = jobList;
        }
        
        // Local improvement with adjacent swaps
        bool improved = true;
        while (improved) {
            improved = false;
            for (int m = 0; m < M; m++) {
                for (int i = 0; i < J - 1; i++) {
                    swap(currentOrders[m][i], currentOrders[m][i + 1]);
                    int newMakespan = computeMakespan(jobs, currentOrders);
                    
                    if (newMakespan < bestMakespan) {
                        bestMakespan = newMakespan;
                        bestOrders = currentOrders;
                        improved = true;
                    } else {
                        swap(currentOrders[m][i], currentOrders[m][i + 1]); // revert
                    }
                }
            }
            currentOrders = bestOrders;
        }
    }
    
    // Output the best solution found
    for (int m = 0; m < M; m++) {
        for (int j = 0; j < J; j++) {
            cout << bestOrders[m][j];
            if (j < J - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
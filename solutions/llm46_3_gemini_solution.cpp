#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>
#include <limits>
#include <queue>

using namespace std;

// Problem data
int J, M;
vector<vector<pair<int, int>>> job_ops; // For job j, k-th op is on machine m with proc_time p: {m, p}
vector<vector<int>> job_op_idx_on_machine; // For job j, machine m, what is the op index k

// Random number generator
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// Calculate makespan and check for cycles
long long calculate_makespan(const vector<vector<int>>& solution) {
    if (J == 0) return 0;
    
    vector<vector<int>> job_pos_on_machine(M, vector<int>(J));
    for (int m = 0; m < M; ++m) {
        for (int i = 0; i < J; ++i) {
            job_pos_on_machine[m][solution[m][i]] = i;
        }
    }

    vector<vector<long long>> completion_times(J, vector<long long>(M, 0));
    vector<int> in_degree(J * M, 0);
    queue<int> q;

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            int op_idx = j * M + k;
            int num_preds = 0;
            if (k > 0) {
                num_preds++;
            }
            int m = job_ops[j][k].first;
            int pos = job_pos_on_machine[m][j];
            if (pos > 0) {
                num_preds++;
            }
            in_degree[op_idx] = num_preds;
            if (in_degree[op_idx] == 0) {
                q.push(op_idx);
            }
        }
    }

    int processed_ops = 0;
    while (!q.empty()) {
        int op_idx = q.front();
        q.pop();
        processed_ops++;

        int j = op_idx / M;
        int k = op_idx % M;

        int m = job_ops[j][k].first;
        int p_time = job_ops[j][k].second;

        long long pred_job_ct = (k == 0) ? 0 : completion_times[j][k - 1];
        
        long long pred_mach_ct = 0;
        int pos = job_pos_on_machine[m][j];
        if (pos > 0) {
            int prev_j = solution[m][pos - 1];
            int prev_k = job_op_idx_on_machine[prev_j][m];
            pred_mach_ct = completion_times[prev_j][prev_k];
        }

        completion_times[j][k] = max(pred_job_ct, pred_mach_ct) + p_time;

        if (k + 1 < M) {
            int next_op_idx = j * M + (k + 1);
            if (--in_degree[next_op_idx] == 0) {
                q.push(next_op_idx);
            }
        }

        if (pos + 1 < J) {
            int next_j = solution[m][pos + 1];
            int next_k = job_op_idx_on_machine[next_j][m];
            int next_op_idx = next_j * M + next_k;
            if (--in_degree[next_op_idx] == 0) {
                q.push(next_op_idx);
            }
        }
    }

    if (processed_ops < J * M) {
        return numeric_limits<long long>::max(); // Cycle detected
    }

    long long makespan = 0;
    for (int j = 0; j < J; ++j) {
        makespan = max(makespan, completion_times[j][M - 1]);
    }
    return makespan;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> J >> M;

    job_ops.resize(J, vector<pair<int, int>>(M));
    job_op_idx_on_machine.resize(J, vector<int>(M));

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            cin >> job_ops[j][k].first >> job_ops[j][k].second;
            job_op_idx_on_machine[j][job_ops[j][k].first] = k;
        }
    }

    auto start_time = chrono::steady_clock::now();
    double time_limit = 1.95; // Seconds

    // Initial solution
    vector<vector<int>> current_solution;
    long long current_makespan = numeric_limits<long long>::max();

    auto try_heuristic = [&](const vector<vector<int>>& sol) {
        long long makespan = calculate_makespan(sol);
        if (makespan < current_makespan) {
            current_makespan = makespan;
            current_solution = sol;
        }
    };
    
    if (J > 0 && M > 0) {
        // Try SPT heuristic
        vector<vector<int>> spt_sol(M, vector<int>(J));
        for (int m = 0; m < M; ++m) {
            vector<pair<int, int>> p_times;
            for (int j = 0; j < J; ++j) {
                int k = job_op_idx_on_machine[j][m];
                p_times.push_back({job_ops[j][k].second, j});
            }
            sort(p_times.begin(), p_times.end());
            for (int j = 0; j < J; ++j) spt_sol[m][j] = p_times[j].second;
        }
        try_heuristic(spt_sol);

        // Try LPT heuristic
        vector<vector<int>> lpt_sol(M, vector<int>(J));
        for (int m = 0; m < M; ++m) {
            vector<pair<int, int>> p_times;
            for (int j = 0; j < J; ++j) {
                int k = job_op_idx_on_machine[j][m];
                p_times.push_back({job_ops[j][k].second, j});
            }
            sort(p_times.rbegin(), p_times.rend());
            for (int j = 0; j < J; ++j) lpt_sol[m][j] = p_times[j].second;
        }
        try_heuristic(lpt_sol);
    }


    if (current_makespan == numeric_limits<long long>::max()) {
        current_solution.assign(M, vector<int>(J));
        for (int m = 0; m < M; ++m) {
            iota(current_solution[m].begin(), current_solution[m].end(), 0);
        }
        current_makespan = calculate_makespan(current_solution);
    }
    
    vector<vector<int>> best_solution = current_solution;
    long long best_makespan = current_makespan;

    // SA parameters
    double initial_T = 0;
    if (J > 1) {
        double total_delta = 0;
        int uphill_moves = 0;
        for (int i = 0; i < 100; ++i) {
            vector<vector<int>> temp_sol = current_solution;
            int m = uniform_int_distribution<int>(0, M - 1)(rng);
            uniform_int_distribution<int> pos_dist(0, J - 1);
            int p1 = pos_dist(rng), p2 = pos_dist(rng);
            if (p1 == p2) continue;
            swap(temp_sol[m][p1], temp_sol[m][p2]);
            
            long long temp_makespan = calculate_makespan(temp_sol);
            if (temp_makespan != numeric_limits<long long>::max()) {
                if (temp_makespan > current_makespan) {
                    total_delta += temp_makespan - current_makespan;
                    uphill_moves++;
                }
            }
        }
        if (uphill_moves > 0) {
            double avg_delta = total_delta / uphill_moves;
            initial_T = -avg_delta / log(0.8); // 80% initial acceptance
        } else {
            long long total_proc_time = 0;
            for(const auto& job : job_ops) for(const auto& op : job) total_proc_time += op.second;
            initial_T = (double)total_proc_time / (J*M*10.0);
        }
    }
    if (initial_T < 1.0) initial_T = 1.0;

    double T = initial_T;
    double final_T = 1e-3;

    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - start_time).count();
        if (elapsed > time_limit) break;
        if (J <= 1) break;
        
        vector<vector<int>> new_solution = current_solution;
        int m = uniform_int_distribution<int>(0, M - 1)(rng);
        
        uniform_int_distribution<int> pos_dist(0, J - 1);
        int i = pos_dist(rng), j = pos_dist(rng);
        if (i == j) continue;
        swap(new_solution[m][i], new_solution[m][j]);

        long long new_makespan = calculate_makespan(new_solution);

        if (new_makespan == numeric_limits<long long>::max()) continue;

        long long delta = new_makespan - current_makespan;

        if (delta < 0) {
            current_solution = new_solution;
            current_makespan = new_makespan;
            if (current_makespan < best_makespan) {
                best_solution = current_solution;
                best_makespan = current_makespan;
            }
        } else {
            uniform_real_distribution<double> dist(0.0, 1.0);
            if (dist(rng) < exp(-delta / T)) {
                current_solution = new_solution;
                current_makespan = new_makespan;
            }
        }
        
        T = initial_T * pow(final_T / initial_T, elapsed / time_limit);
    }

    for (int m = 0; m < M; ++m) {
        for (int j = 0; j < J; ++j) {
            cout << best_solution[m][j] << (j == J - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
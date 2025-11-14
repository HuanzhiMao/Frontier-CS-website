#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include <queue>
#include <limits>

using namespace std;

int J, M;

struct Operation {
    int machine;
    int time;
};

vector<vector<Operation>> job_routes;
vector<vector<int>> p_times_jm;
vector<int> p_times_flat;
vector<vector<int>> op_by_jm;

long long calculate_makespan(const vector<vector<int>>& permutations) {
    int num_ops = J * M;
    vector<vector<int>> adj(num_ops);
    vector<int> in_degree(num_ops, 0);

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M - 1; ++k) {
            int u = j * M + k;
            int v = j * M + k + 1;
            adj[u].push_back(v);
            in_degree[v]++;
        }
    }

    for (int m = 0; m < M; ++m) {
        for (int i = 0; i < J - 1; ++i) {
            int j1 = permutations[m][i];
            int j2 = permutations[m][i + 1];
            int k1 = op_by_jm[j1][m];
            int k2 = op_by_jm[j2][m];
            int u = j1 * M + k1;
            int v = j2 * M + k2;
            adj[u].push_back(v);
            in_degree[v]++;
        }
    }

    queue<int> q;
    vector<long long> start_times(num_ops, 0);
    for (int i = 0; i < num_ops; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    int count = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        count++;

        long long u_completion_time = start_times[u] + p_times_flat[u];

        for (int v : adj[u]) {
            start_times[v] = max(start_times[v], u_completion_time);
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (count < num_ops) {
        return numeric_limits<long long>::max();
    }

    long long makespan = 0;
    for (int i = 0; i < num_ops; ++i) {
        makespan = max(makespan, start_times[i] + p_times_flat[i]);
    }
    return makespan;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> J >> M;
    job_routes.resize(J, vector<Operation>(M));
    p_times_jm.resize(J, vector<int>(M));
    op_by_jm.resize(J, vector<int>(M));
    p_times_flat.resize(J * M);

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            cin >> job_routes[j][k].machine >> job_routes[j][k].time;
            p_times_jm[j][job_routes[j][k].machine] = job_routes[j][k].time;
            op_by_jm[j][job_routes[j][k].machine] = k;
            p_times_flat[j * M + k] = job_routes[j][k].time;
        }
    }

    vector<vector<int>> best_perms;
    long long best_makespan = -1;

    // Heuristic 1: SPT on each machine
    {
        vector<vector<int>> perms(M, vector<int>(J));
        for(int m = 0; m < M; ++m) {
            vector<pair<int, int>> machine_jobs;
            for(int j = 0; j < J; ++j) {
                machine_jobs.push_back({p_times_jm[j][m], j});
            }
            sort(machine_jobs.begin(), machine_jobs.end());
            for(int i = 0; i < J; ++i) {
                perms[m][i] = machine_jobs[i].second;
            }
        }
        long long makespan = calculate_makespan(perms);
        if (makespan != numeric_limits<long long>::max() && (best_makespan == -1 || makespan < best_makespan)) {
            best_makespan = makespan;
            best_perms = perms;
        }
    }

    // Heuristic 2: LPT on each machine
    {
        vector<vector<int>> perms(M, vector<int>(J));
        for(int m = 0; m < M; ++m) {
            vector<pair<int, int>> machine_jobs;
            for(int j = 0; j < J; ++j) {
                machine_jobs.push_back({p_times_jm[j][m], j});
            }
            sort(machine_jobs.rbegin(), machine_jobs.rend());
            for(int i = 0; i < J; ++i) {
                perms[m][i] = machine_jobs[i].second;
            }
        }
        long long makespan = calculate_makespan(perms);
        if (makespan != numeric_limits<long long>::max() && (best_makespan == -1 || makespan < best_makespan)) {
            best_makespan = makespan;
            best_perms = perms;
        }
    }

    // Heuristic 3: Sort by total processing time
    {
        vector<long long> total_job_time(J, 0);
        for(int j = 0; j < J; ++j) {
            for(int k = 0; k < M; ++k) {
                total_job_time[j] += job_routes[j][k].time;
            }
        }
        vector<pair<long long, int>> sorted_jobs;
        for(int j = 0; j < J; ++j) sorted_jobs.push_back({total_job_time[j], j});
        sort(sorted_jobs.begin(), sorted_jobs.end());
        vector<int> job_order(J);
        for(int i = 0; i < J; ++i) job_order[i] = sorted_jobs[i].second;
        
        vector<vector<int>> perms(M, job_order);
        long long makespan = calculate_makespan(perms);
        if (makespan != numeric_limits<long long>::max() && (best_makespan == -1 || makespan < best_makespan)) {
            best_makespan = makespan;
            best_perms = perms;
        }
    }
    
    if (best_perms.empty()) {
        best_perms.assign(M, vector<int>(J));
        for(int m=0; m<M; ++m) {
            iota(best_perms[m].begin(), best_perms[m].end(), 0);
        }
        best_makespan = calculate_makespan(best_perms);
    }

    vector<vector<int>> current_perms = best_perms;
    long long current_makespan = best_makespan;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dist(0.0, 1.0);

    auto start_time = chrono::high_resolution_clock::now();
    long long time_limit_ms = 1950;

    double initial_temp = 0.0;
    if (J > 0 && M > 0) {
        long long total_p_time_sum = 0;
        for(int j = 0; j < J; ++j) for(int k = 0; k < M; ++k) total_p_time_sum += job_routes[j][k].time;
        initial_temp = (double)total_p_time_sum / (J * M) / 10.0;
        if (initial_temp < 1.0) initial_temp = 1.0;
    }
    
    double temperature = initial_temp;

    uniform_int_distribution<int> machine_dist(0, M - 1);
    uniform_int_distribution<int> move_dist(0, 2);
    uniform_int_distribution<int> job_dist(0, J > 0 ? J - 1 : 0);
    uniform_int_distribution<int> adj_swap_dist(0, J > 1 ? J - 2 : 0);

    while (true) {
        auto now = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > time_limit_ms) {
            break;
        }

        if (J <= 1) break;

        vector<vector<int>> next_perms = current_perms;
        int m = machine_dist(rng);
        
        int move_type = move_dist(rng);
        if (move_type == 0) { // Adjacent swap
            int i = adj_swap_dist(rng);
            swap(next_perms[m][i], next_perms[m][i + 1]);
        } else if (move_type == 1) { // General swap
            int i = job_dist(rng);
            int j = job_dist(rng);
            if (i != j) {
                swap(next_perms[m][i], next_perms[m][j]);
            }
        } else { // Insertion
            int i = job_dist(rng);
            int j = job_dist(rng);
            if (i != j) {
                int job_to_move = next_perms[m][i];
                next_perms[m].erase(next_perms[m].begin() + i);
                next_perms[m].insert(next_perms[m].begin() + j, job_to_move);
            }
        }
        
        long long next_makespan = calculate_makespan(next_perms);
        
        if (next_makespan < current_makespan) {
            current_perms = next_perms;
            current_makespan = next_makespan;
            if (current_makespan < best_makespan) {
                best_makespan = current_makespan;
                best_perms = next_perms;
            }
        } else {
            double p = exp(-(double)(next_makespan - current_makespan) / temperature);
            if (dist(rng) < p) {
                current_perms = next_perms;
                current_makespan = next_makespan;
            }
        }
        
        double time_elapsed_ratio = (double)chrono::duration_cast<chrono::milliseconds>(now - start_time).count() / time_limit_ms;
        temperature = initial_temp * (1.0 - time_elapsed_ratio);
        if (temperature < 1e-9) temperature = 1e-9;
    }

    for (int m = 0; m < M; ++m) {
        for (int j = 0; j < J; ++j) {
            cout << best_perms[m][j] << (j == J - 1 ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>

// Problem data
int J, M;
struct Operation { int machine, time; };
std::vector<std::vector<Operation>> jobs;
std::vector<std::vector<int>> job_op_idx; // job_op_idx[j][m] = k (op index in job's route)

// Precomputed data for performance
std::vector<std::vector<int>> op_machine; // op_machine[j][k] = machine
std::vector<int> op_proc_time_flat; // op_proc_time_flat[j*M+k] = time

// Random number generator
std::mt19937 rng;

// Converts (job, op_k) to a single integer ID
inline int op_id(int j, int k) {
    return j * M + k;
}

long long calculate_makespan(const std::vector<std::vector<int>>& schedules, std::vector<int>& critical_path) {
    critical_path.clear();
    int num_ops = J * M;
    int source = num_ops, sink = num_ops + 1;

    std::vector<std::vector<int>> adj(num_ops + 2);
    std::vector<std::vector<int>> rev_adj(num_ops + 2);
    std::vector<int> in_degree(num_ops + 2, 0);

    // Job precedence edges
    for (int j = 0; j < J; ++j) {
        adj[source].push_back(op_id(j, 0));
        rev_adj[op_id(j, 0)].push_back(source);
        in_degree[op_id(j, 0)]++;
        for (int k = 0; k < M - 1; ++k) {
            adj[op_id(j, k)].push_back(op_id(j, k + 1));
            rev_adj[op_id(j, k + 1)].push_back(op_id(j, k));
            in_degree[op_id(j, k + 1)]++;
        }
        adj[op_id(j, M - 1)].push_back(sink);
        rev_adj[sink].push_back(op_id(j, M - 1));
        in_degree[sink]++;
    }

    // Machine precedence edges
    for (int m = 0; m < M; ++m) {
        for (size_t i = 0; i < J - 1; ++i) {
            int j1 = schedules[m][i];
            int j2 = schedules[m][i + 1];
            int u1 = op_id(j1, job_op_idx[j1][m]);
            int u2 = op_id(j2, job_op_idx[j2][m]);
            adj[u1].push_back(u2);
            rev_adj[u2].push_back(u1);
            in_degree[u2]++;
        }
    }

    std::vector<int> q; q.reserve(num_ops + 2);
    q.push_back(source);
    
    std::vector<int> topo_order; topo_order.reserve(num_ops + 2);
    int head = 0;
    while (head < q.size()) {
        int u = q[head++];
        topo_order.push_back(u);
        for (int v : adj[u]) {
            if (--in_degree[v] == 0) {
                q.push_back(v);
            }
        }
    }

    if (topo_order.size() != num_ops + 2) return -1;
    
    std::vector<long long> dist(num_ops + 2, 0);
    std::vector<int> pred(num_ops + 2, -1);

    for (int u : topo_order) {
        int u_time = (u < num_ops) ? op_proc_time_flat[u] : 0;
        for (int v : adj[u]) {
            if (dist[v] < dist[u] + u_time) {
                dist[v] = dist[u] + u_time;
                pred[v] = u;
            }
        }
    }

    long long makespan = dist[sink];
    
    if (makespan > 0) {
        int curr = sink;
        while (curr != -1) {
            critical_path.push_back(curr);
            curr = pred[curr];
        }
        std::reverse(critical_path.begin(), critical_path.end());
    }
    
    return makespan;
}

void generate_initial_solution(std::vector<std::vector<int>>& schedules) {
    std::vector<long long> total_time(J, 0);
    std::vector<int> job_order(J);
    std::iota(job_order.begin(), job_order.end(), 0);

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            total_time[j] += jobs[j][k].time;
        }
    }

    std::sort(job_order.begin(), job_order.end(), [&](int j1, int j2) {
        return total_time[j1] > total_time[j2];
    });

    for (int m = 0; m < M; ++m) {
        schedules[m] = job_order;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::cin >> J >> M;
    jobs.resize(J, std::vector<Operation>(M));
    job_op_idx.resize(J, std::vector<int>(M));
    op_machine.resize(J, std::vector<int>(M));
    op_proc_time_flat.resize(J * M);

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            std::cin >> jobs[j][k].machine >> jobs[j][k].time;
            job_op_idx[j][jobs[j][k].machine] = k;
            op_machine[j][k] = jobs[j][k].machine;
            op_proc_time_flat[op_id(j, k)] = jobs[j][k].time;
        }
    }
    
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    auto start_time = std::chrono::high_resolution_clock::now();
    double time_limit_seconds = 1.95; 

    std::vector<std::vector<int>> current_schedules(M, std::vector<int>(J));
    generate_initial_solution(current_schedules);

    std::vector<std::vector<int>> best_schedules = current_schedules;
    std::vector<int> dummy_path;
    long long current_makespan = calculate_makespan(current_schedules, dummy_path);
    long long best_makespan = current_makespan;

    double T_initial = std::max(1.0, (double)current_makespan / 15.0);
    double T_final = 0.01;

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - start_time;
        if (elapsed.count() > time_limit_seconds) {
            break;
        }
        
        double progress = elapsed.count() / time_limit_seconds;
        double T = T_initial * std::pow(T_final / T_initial, progress);

        std::vector<int> critical_path;
        calculate_makespan(current_schedules, critical_path); 
        
        std::vector<std::pair<int, int>> critical_blocks;
        if(critical_path.size() > 3){
            for (size_t i = 1; i + 2 < critical_path.size(); ++i) {
                int u1 = critical_path[i];
                int u2 = critical_path[i+1];
                
                int j1 = u1 / M, k1 = u1 % M;
                int j2 = u2 / M, k2 = u2 % M;
                
                if (op_machine[j1][k1] == op_machine[j2][k2]) {
                     critical_blocks.push_back({u1, u2});
                }
            }
        }
        
        std::vector<std::vector<int>> next_schedules = current_schedules;
        bool move_made = false;

        if (critical_blocks.empty()) {
            if (J > 1) {
                int m = std::uniform_int_distribution<int>(0, M - 1)(rng);
                int i = std::uniform_int_distribution<int>(0, J - 2)(rng);
                std::swap(next_schedules[m][i], next_schedules[m][i+1]);
                move_made = true;
            }
        } else {
            int block_idx = std::uniform_int_distribution<int>(0, critical_blocks.size() - 1)(rng);
            int u1 = critical_blocks[block_idx].first;
            int u2 = critical_blocks[block_idx].second;

            int j1 = u1 / M;
            int j2 = u2 / M;
            int m = op_machine[j1][u1 % M];

            auto& sched_m = next_schedules[m];
            int pos1 = -1;
            for(int i = 0; i < J; ++i) {
                if(sched_m[i] == j1) {
                    pos1 = i;
                    break;
                }
            }
            
            if(pos1 != -1 && pos1 + 1 < J && sched_m[pos1+1] == j2) {
                std::swap(sched_m[pos1], sched_m[pos1+1]);
                move_made = true;
            }
        }

        if (move_made) {
            long long next_makespan = calculate_makespan(next_schedules, dummy_path);
            if (next_makespan != -1) {
                long long delta = next_makespan - current_makespan;
                if (delta < 0 || (T > 1e-9 && std::uniform_real_distribution<double>(0.0, 1.0)(rng) < exp(-delta / T))) {
                    current_schedules = next_schedules;
                    current_makespan = next_makespan;
                    if (current_makespan < best_makespan) {
                        best_makespan = current_makespan;
                        best_schedules = current_schedules;
                    }
                }
            }
        }
    }

    for (int m = 0; m < M; ++m) {
        for (int j = 0; j < J; ++j) {
            std::cout << best_schedules[m][j] << (j == J - 1 ? "" : " ");
        }
        std::cout << "\n";
    }

    return 0;
}
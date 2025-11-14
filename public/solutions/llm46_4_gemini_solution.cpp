#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <set>

// --- Global state & problem data ---
int J, M;

struct Operation {
    int machine;
    int duration;
};
std::vector<std::vector<Operation>> jobs_by_op_order;

struct OpInfo {
    int op_k; // index in jobs_by_op_order
    int duration;
};
std::vector<std::vector<OpInfo>> jobs_by_machine;

std::mt19937 rng;

// --- Helper for indexing ---
inline int op_idx(int job, int k) {
    return job * M + k;
}

// --- Makespan calculation and Critical Path analysis ---
long long calculate_makespan(const std::vector<std::vector<int>>& machine_sequences, std::vector<std::pair<int, int>>* critical_blocks) {
    int N = J * M;
    std::vector<std::vector<int>> adj(N), rev_adj(N);
    std::vector<int> in_degree(N, 0);
    std::vector<int> proc_times(N);

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            proc_times[op_idx(j, k)] = jobs_by_op_order[j][k].duration;
            if (k > 0) {
                int u = op_idx(j, k - 1);
                int v = op_idx(j, k);
                adj[u].push_back(v);
                rev_adj[v].push_back(u);
                in_degree[v]++;
            }
        }
    }

    for (int m = 0; m < M; ++m) {
        if (J > 1) {
            for (int i = 0; i < J - 1; ++i) {
                int j_prev = machine_sequences[m][i];
                int j_curr = machine_sequences[m][i + 1];
                int k_prev = jobs_by_machine[j_prev][m].op_k;
                int k_curr = jobs_by_machine[j_curr][m].op_k;
                int u = op_idx(j_prev, k_prev);
                int v = op_idx(j_curr, k_curr);
                adj[u].push_back(v);
                rev_adj[v].push_back(u);
                in_degree[v]++;
            }
        }
    }

    std::vector<long long> es(N, 0);
    std::vector<int> q;
    std::vector<int> current_in_degree = in_degree;
    for (int i = 0; i < N; ++i) {
        if (current_in_degree[i] == 0) {
            q.push_back(i);
        }
    }

    int head = 0;
    while (head < q.size()) {
        int u = q[head++];
        long long ec_u = es[u] + proc_times[u];
        for (int v : adj[u]) {
            es[v] = std::max(es[v], ec_u);
            current_in_degree[v]--;
            if (current_in_degree[v] == 0) {
                q.push_back(v);
            }
        }
    }

    if (q.size() < N) return -1; // Cycle detected

    long long makespan = 0;
    for (int i = 0; i < N; ++i) {
        makespan = std::max(makespan, es[i] + proc_times[i]);
    }

    if (critical_blocks) {
        critical_blocks->clear();
        std::vector<long long> lc(N, makespan);
        std::vector<int> out_degree(N, 0);
        for (int i = 0; i < N; ++i) out_degree[i] = adj[i].size();

        std::vector<int> q_rev;
        for (int i = 0; i < N; ++i) if (out_degree[i] == 0) q_rev.push_back(i);

        head = 0;
        while (head < q_rev.size()) {
            int u = q_rev[head++];
            long long ls_u = lc[u] - proc_times[u];
            for (int p : rev_adj[u]) {
                lc[p] = std::min(lc[p], ls_u);
                out_degree[p]--;
                if (out_degree[p] == 0) q_rev.push_back(p);
            }
        }

        for (int m = 0; m < M; ++m) {
            if (J > 1) {
                for (int i = 0; i < J - 1; ++i) {
                    int j1 = machine_sequences[m][i], j2 = machine_sequences[m][i + 1];
                    int k1 = jobs_by_machine[j1][m].op_k, k2 = jobs_by_machine[j2][m].op_k;
                    int u = op_idx(j1, k1), v = op_idx(j2, k2);

                    if (es[u] + proc_times[u] == es[v] && es[u] == lc[u] - proc_times[u] && es[v] == lc[v] - proc_times[v]) {
                        critical_blocks->push_back({m, i});
                    }
                }
            }
        }
    }
    return makespan;
}

// --- Initial Solution ---
std::vector<std::vector<int>> generate_initial_solution() {
    std::vector<std::vector<int>> machine_sequences(M);
    std::vector<int> job_op_ptr(J, 0);
    std::vector<long long> job_completion_time(J, 0);
    std::vector<long long> machine_completion_time(M, 0);

    std::set<int> schedulable_jobs;
    for (int j = 0; j < J; ++j) schedulable_jobs.insert(j);

    for (int scheduled_count = 0; scheduled_count < J * M; ++scheduled_count) {
        int best_j = -1;
        long long min_ect = -1;

        for (int j : schedulable_jobs) {
            int k = job_op_ptr[j];
            int m = jobs_by_op_order[j][k].machine;
            int p = jobs_by_op_order[j][k].duration;

            long long start_time = std::max(job_completion_time[j], machine_completion_time[m]);
            long long completion_time = start_time + p;

            if (best_j == -1 || completion_time < min_ect) {
                min_ect = completion_time;
                best_j = j;
            }
        }

        int k = job_op_ptr[best_j];
        int m = jobs_by_op_order[best_j][k].machine;

        machine_sequences[m].push_back(best_j);
        job_completion_time[best_j] = min_ect;
        machine_completion_time[m] = min_ect;

        job_op_ptr[best_j]++;
        if (job_op_ptr[best_j] >= M) {
            schedulable_jobs.erase(best_j);
        }
    }
    return machine_sequences;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    auto start_time_total = std::chrono::steady_clock::now();
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    std::cin >> J >> M;
    jobs_by_op_order.resize(J, std::vector<Operation>(M));
    jobs_by_machine.resize(J, std::vector<OpInfo>(M));

    for (int j = 0; j < J; ++j) {
        for (int k = 0; k < M; ++k) {
            std::cin >> jobs_by_op_order[j][k].machine >> jobs_by_op_order[j][k].duration;
            jobs_by_machine[j][jobs_by_op_order[j][k].machine] = {k, jobs_by_op_order[j][k].duration};
        }
    }

    auto current_sol = generate_initial_solution();
    std::vector<std::pair<int, int>> critical_blocks;
    long long current_makespan = calculate_makespan(current_sol, &critical_blocks);

    auto best_sol = current_sol;
    long long best_makespan = current_makespan;

    double T = 1.0;
    if (J > 1) {
        double sum_delta = 0;
        int count_delta = 0;
        for (int i = 0; i < 100; ++i) {
            auto temp_sol = current_sol;
            int m = std::uniform_int_distribution<int>(0, M - 1)(rng);
            int p = std::uniform_int_distribution<int>(0, J - 2)(rng);
            std::swap(temp_sol[m][p], temp_sol[m][p + 1]);
            long long temp_makespan = calculate_makespan(temp_sol, nullptr);
            if (temp_makespan > current_makespan) {
                sum_delta += temp_makespan - current_makespan;
                count_delta++;
            }
        }
        if (count_delta > 0) {
            T = (sum_delta / count_delta) * 0.3;
        }
    }
    double cooling_rate = 0.99995;

    int time_limit_ms = 1900;
    if (J * M > 500) time_limit_ms = 4800;
    if (J > 30 || M > 15) time_limit_ms = 4800;

    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time_total).count() < time_limit_ms) {
        if (J <= 1) break;
        
        auto next_sol = current_sol;
        
        if (critical_blocks.empty() || std::uniform_real_distribution<double>(0.0, 1.0)(rng) < 0.1) {
            int m = std::uniform_int_distribution<int>(0, M - 1)(rng);
            int pos = std::uniform_int_distribution<int>(0, J - 2)(rng);
            std::swap(next_sol[m][pos], next_sol[m][pos + 1]);
        } else {
            int block_idx = std::uniform_int_distribution<int>(0, critical_blocks.size() - 1)(rng);
            int m = critical_blocks[block_idx].first;
            int pos = critical_blocks[block_idx].second;
            std::swap(next_sol[m][pos], next_sol[m][pos + 1]);
        }

        std::vector<std::pair<int, int>> next_critical_blocks;
        long long next_makespan = calculate_makespan(next_sol, &next_critical_blocks);
        if (next_makespan == -1) continue;

        long long delta = next_makespan - current_makespan;
        if (delta < 0 || (T > 1e-9 && std::exp(-(double)delta / T) > std::uniform_real_distribution<double>(0.0, 1.0)(rng))) {
            current_sol = next_sol;
            current_makespan = next_makespan;
            critical_blocks = next_critical_blocks;
            if (current_makespan < best_makespan) {
                best_makespan = current_makespan;
                best_sol = current_sol;
            }
        }
        T *= cooling_rate;
    }

    for (int m = 0; m < M; ++m) {
        for (int j = 0; j < J; ++j) {
            std::cout << best_sol[m][j] << (j == J - 1 ? "" : " ");
        }
        std::cout << "\n";
    }

    return 0;
}
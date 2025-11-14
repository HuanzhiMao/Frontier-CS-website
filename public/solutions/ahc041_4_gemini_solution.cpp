#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <queue>
#include <tuple>
#include <chrono>
#include <random>
#include <cmath>

using namespace std;

const int N_MAX = 1000;
int N, M, H;
vector<int> A;
vector<vector<int>> adj;

struct State {
    vector<int> parent;
    vector<vector<int>> children;
    vector<int> height;
    vector<long long> rel_sum_A;
    vector<int> rel_max_h;
    vector<int> roots;
    long long score;

    State() : parent(N, -1), children(N), height(N, 0), rel_sum_A(N), rel_max_h(N, 0), score(0) {}

    void compute_rel_dfs(int u) {
        rel_sum_A[u] = A[u];
        rel_max_h[u] = 0;
        for (int v : children[u]) {
            compute_rel_dfs(v);
            rel_sum_A[u] += rel_sum_A[v];
            rel_max_h[u] = max(rel_max_h[u], 1 + rel_max_h[v]);
        }
    }

    void compute_height_dfs(int u, int h) {
        height[u] = h;
        score += (long long)(h + 1) * A[u];
        for (int v : children[u]) {
            compute_height_dfs(v, h + 1);
        }
    }

    void compute_all() {
        for (int i = 0; i < N; ++i) {
            children[i].clear();
        }
        roots.clear();
        for (int i = 0; i < N; ++i) {
            if (parent[i] == -1) {
                roots.push_back(i);
            } else {
                children[parent[i]].push_back(i);
            }
        }

        for (int root : roots) {
            compute_rel_dfs(root);
        }
        
        score = 0;
        for (int root : roots) {
            compute_height_dfs(root, 0);
        }
    }
};

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::steady_clock::now();

    cin >> N >> M >> H;
    A.resize(N);
    adj.resize(N);
    vector<int> x(N), y(N);
    for (int i = 0; i < N; ++i) cin >> A[i];
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < N; ++i) cin >> x[i] >> y[i];

    State best_state;

    // Greedy Initial Solution
    {
        vector<int> p(N, -1);
        vector<int> h(N, 0);
        vector<bool> is_root(N, true);
        priority_queue<tuple<long long, int, int>> pq;

        for (int i = 0; i < N; ++i) {
            for (int neighbor : adj[i]) {
                if (h[neighbor] + 1 <= H) {
                    long long gain = (long long)(h[neighbor] + 1) * A[i];
                    pq.emplace(gain, i, neighbor);
                }
            }
        }

        while (!pq.empty()) {
            auto [gain, u, v] = pq.top();
            pq.pop();

            if (!is_root[u] || h[v] + 1 > H) {
                continue;
            }

            p[u] = v;
            h[u] = h[v] + 1;
            is_root[u] = false;

            for (int neighbor : adj[u]) {
                if (is_root[neighbor]) {
                    if (h[u] + 1 <= H) {
                        long long new_gain = (long long)(h[u] + 1) * A[neighbor];
                        pq.emplace(new_gain, neighbor, u);
                    }
                }
            }
        }
        best_state.parent = p;
        best_state.compute_all();
    }
    
    State current_state = best_state;

    double start_temp = 2000;
    double end_temp = 1;
    
    uniform_int_distribution<int> dist_v(0, N - 1);
    uniform_real_distribution<double> dist_prob(0.0, 1.0);

    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        if (elapsed_ms > 2950) break;

        double temp = start_temp * pow(end_temp / start_temp, elapsed_ms / 2950.0);

        int move_type = dist_prob(rng) * 100;
        
        if (move_type < 80) { // Re-parent
            int v = dist_v(rng);
            if (current_state.parent[v] == -1 || adj[v].size() <= 1) continue;
            
            int u = current_state.parent[v];
            int w_idx = uniform_int_distribution<int>(0, adj[v].size() - 1)(rng);
            int w = adj[v][w_idx];
            if (w == u) continue;

            bool is_ancestor = false;
            int curr = w;
            while(curr != -1) {
                if (curr == v) {
                    is_ancestor = true;
                    break;
                }
                curr = current_state.parent[curr];
            }
            if(is_ancestor) continue;
            
            if (current_state.height[w] + 1 + current_state.rel_max_h[v] > H) continue;

            long long score_delta = (long long)(current_state.height[w] - current_state.height[u]) * current_state.rel_sum_A[v];

            if (score_delta > 0 || dist_prob(rng) < exp(score_delta / temp)) {
                current_state.parent[v] = w;
                current_state.compute_all();
                if (current_state.score > best_state.score) {
                    best_state = current_state;
                }
            }
        } else if (move_type < 90) { // Make root
            int v = dist_v(rng);
            if (current_state.parent[v] == -1) continue;
            
            long long score_delta = -(long long)(current_state.height[v]) * current_state.rel_sum_A[v];

            if (score_delta > 0 || dist_prob(rng) < exp(score_delta / temp)) {
                current_state.parent[v] = -1;
                current_state.compute_all();
                if (current_state.score > best_state.score) {
                    best_state = current_state;
                }
            }
        } else { // Root to non-root
            if (current_state.roots.empty()) continue;
            int r_idx = uniform_int_distribution<int>(0, current_state.roots.size() - 1)(rng);
            int r = current_state.roots[r_idx];
            if (adj[r].empty()) continue;

            int u_idx = uniform_int_distribution<int>(0, adj[r].size() - 1)(rng);
            int u = adj[r][u_idx];
            
            int root_of_u = u;
            while(current_state.parent[root_of_u] != -1) root_of_u = current_state.parent[root_of_u];
            if (root_of_u == r) continue;

            if (current_state.height[u] + 1 + current_state.rel_max_h[r] > H) continue;
            
            long long score_delta = (long long)(current_state.height[u] + 1) * current_state.rel_sum_A[r];

            if (score_delta > 0 || dist_prob(rng) < exp(score_delta / temp)) {
                current_state.parent[r] = u;
                current_state.compute_all();
                if (current_state.score > best_state.score) {
                    best_state = current_state;
                }
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << best_state.parent[i] << (i == N - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}
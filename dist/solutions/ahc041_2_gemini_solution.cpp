#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>

// --- Timer ---
auto start_time = std::chrono::steady_clock::now();
double time_limit = 2.95; // seconds

bool is_time_up() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - start_time;
    return elapsed.count() > time_limit;
}

// --- Random Number Generation ---
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// --- Problem Constants and Input ---
int N, M, H;
std::vector<int> A;
std::vector<std::vector<int>> adj;

// --- Solution State ---
std::vector<int> parent;
std::vector<std::vector<int>> children;
std::vector<long long> sum_A_subtree;
std::vector<int> max_rel_h;

// --- Helper Functions for State Management ---
void build_children() {
    for (int i = 0; i < N; ++i) {
        children[i].clear();
    }
    for (int i = 0; i < N; ++i) {
        if (parent[i] != -1) {
            children[parent[i]].push_back(i);
        }
    }
}

void dfs_rebuild(int u) {
    sum_A_subtree[u] = A[u];
    max_rel_h[u] = 0;
    for (int c : children[u]) {
        dfs_rebuild(c);
        sum_A_subtree[u] += sum_A_subtree[c];
        max_rel_h[u] = std::max(max_rel_h[u], 1 + max_rel_h[c]);
    }
}

void rebuild_aux_data() {
    build_children();
    for (int i = 0; i < N; ++i) {
        if (parent[i] == -1) {
            dfs_rebuild(i);
        }
    }
}

int get_height(int v) {
    if (v == -1) return -1;
    int h = 0;
    int curr = v;
    while (parent[curr] != -1) {
        curr = parent[curr];
        h++;
    }
    return h;
}

bool is_ancestor(int u, int v) { // is u an ancestor of v?
    if (u == v) return true;
    if (u == -1) return false;
    int curr = v;
    while (curr != -1) {
        if (curr == u) return true;
        curr = parent[curr];
    }
    return false;
}

void update_ancestors(int v, int old_p, int new_p) {
    long long sum_A_v = sum_A_subtree[v];

    int curr = old_p;
    while (curr != -1) {
        sum_A_subtree[curr] -= sum_A_v;
        curr = parent[curr];
    }
    curr = new_p;
    while (curr != -1) {
        sum_A_subtree[curr] += sum_A_v;
        curr = parent[curr];
    }

    curr = old_p;
    while (curr != -1) {
        int old_mrh = max_rel_h[curr];
        int new_mrh = 0;
        for (int c : children[curr]) {
            new_mrh = std::max(new_mrh, 1 + max_rel_h[c]);
        }
        if (old_mrh == new_mrh) break;
        max_rel_h[curr] = new_mrh;
        curr = parent[curr];
    }

    curr = new_p;
    int mrh_from_v = 1 + max_rel_h[v];
    while (curr != -1) {
        int old_mrh = max_rel_h[curr];
        int new_mrh = std::max(old_mrh, mrh_from_v);
        if (new_mrh == old_mrh) break;
        max_rel_h[curr] = new_mrh;
        mrh_from_v = 1 + new_mrh;
        curr = parent[curr];
    }
}

void initial_solution() {
    parent.assign(N, -2);
    std::vector<bool> visited(N, false);
    std::vector<int> p_order(N);
    std::iota(p_order.begin(), p_order.end(), 0);
    std::shuffle(p_order.begin(), p_order.end(), rng);
    
    for (int i : p_order) {
        if (!visited[i]) {
            parent[i] = -1;
            visited[i] = true;
            std::vector<int> q;
            q.push_back(i);
            int head = 0;
            std::vector<int> heights(N);
            heights[i] = 0;

            while(head < q.size()){
                int u = q[head++];
                if(heights[u] >= H) continue;

                std::vector<int> neighbors = adj[u];
                std::shuffle(neighbors.begin(), neighbors.end(), rng);

                for(int v_node : neighbors){
                    if(!visited[v_node]){
                        visited[v_node] = true;
                        parent[v_node] = u;
                        heights[v_node] = heights[u] + 1;
                        q.push_back(v_node);
                    }
                }
            }
        }
    }
}

void simulated_annealing() {
    initial_solution();
    rebuild_aux_data();
    
    std::vector<int> best_parent = parent;
    long long current_score = 0;
    for(int i=0; i<N; ++i) current_score += (long long)(get_height(i) + 1) * A[i];
    long long best_score = current_score;

    double start_temp = 200;
    double end_temp = 0.1;

    int iterations = 0;
    while (!is_time_up()) {
        iterations++;
        
        int v = rng() % N;
        int new_p = -1;
        if (!adj[v].empty()) {
            int rand_idx = rng() % (adj[v].size() + 1);
            if (rand_idx < adj[v].size()) {
                new_p = adj[v][rand_idx];
            }
        }
        int old_p = parent[v];

        if (new_p == old_p) continue;
        if (new_p != -1 && is_ancestor(v, new_p)) continue;
        
        int h_new_p = get_height(new_p);
        if (h_new_p + 1 + max_rel_h[v] > H) continue;

        long long h_v_old = get_height(v);
        long long h_v_new = h_new_p + 1;
        long long delta_h = h_v_new - h_v_old;
        long long delta_score = delta_h * sum_A_subtree[v];
        
        double temp = start_temp;
        double progress = std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count() / time_limit;
        if (progress < 1.0) {
            temp = start_temp * std::pow(end_temp / start_temp, progress);
        } else {
            temp = end_temp;
        }
        
        double probability = std::exp(delta_score / temp);

        if (delta_score > 0 || std::uniform_real_distribution<double>(0.0, 1.0)(rng) < probability) {
            current_score += delta_score;
            
            // Apply move
            // 1. Update children lists
            if (old_p != -1) {
                children[old_p].erase(std::remove(children[old_p].begin(), children[old_p].end(), v), children[old_p].end());
            }
            if (new_p != -1) {
                children[new_p].push_back(v);
            }

            // 2. Update parent
            parent[v] = new_p;
            
            // 3. Update ancestor aux data
            update_ancestors(v, old_p, new_p);
            
            if (current_score > best_score) {
                best_score = current_score;
                best_parent = parent;
            }
        }
    }
    parent = best_parent;
}

void solve() {
    A.resize(N);
    adj.assign(N, std::vector<int>());
    parent.resize(N);
    children.assign(N, std::vector<int>());
    sum_A_subtree.resize(N);
    max_rel_h.resize(N);

    for (int i = 0; i < N; ++i) std::cin >> A[i];
    for (int i = 0; i < M; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < N; ++i) {
        int x, y;
        std::cin >> x >> y;
    }
    
    simulated_annealing();

    for (int i = 0; i < N; ++i) {
        std::cout << parent[i] << (i == N - 1 ? "" : " ");
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    std::cin >> N >> M >> H;
    solve();

    return 0;
}
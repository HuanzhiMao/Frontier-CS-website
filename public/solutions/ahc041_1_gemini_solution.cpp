#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>

using namespace std;

int N_in, M_in, H_in;
int A[1000];
vector<int> adj[1000];

// SA state
int parent[1000];
vector<int> children[1000];
long long sum_A_subtree[1000];
int max_depth_subtree[1000];
long long current_score;

int p_best[1000];
long long best_score;

mt19937 rng;

int get_height(int v) {
    if (v == -1) return -1;
    int h = 0;
    while (parent[v] != -1) {
        v = parent[v];
        h++;
    }
    return h;
}

void dfs_init(int u) {
    sum_A_subtree[u] = A[u];
    max_depth_subtree[u] = 0;
    for (int v : children[u]) {
        dfs_init(v);
        sum_A_subtree[u] += sum_A_subtree[v];
        max_depth_subtree[u] = max(max_depth_subtree[u], max_depth_subtree[v] + 1);
    }
}

void recompute_max_d(int u) {
    int d = -1;
    for (int c : children[u]) {
        d = max(d, max_depth_subtree[c]);
    }
    max_depth_subtree[u] = d + 1;
}

void generate_initial_solution() {
    vector<int> p_init(N_in, -2);
    vector<int> q;
    vector<int> h(N_in);

    vector<int> nodes(N_in);
    iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), rng);

    for (int i : nodes) {
        if (p_init[i] == -2) {
            q.push_back(i);
            p_init[i] = -1;
            h[i] = 0;
            int head = 0;
            while(head < q.size()) {
                int u = q[head++];
                if (h[u] >= H_in) continue;
                for (int v : adj[u]) {
                    if (p_init[v] == -2) {
                        p_init[v] = u;
                        h[v] = h[u] + 1;
                        q.push_back(v);
                    }
                }
            }
            q.clear();
        }
    }

    for(int i = 0; i < N_in; ++i) parent[i] = p_init[i];
}


void init_sa_state() {
    for (int i = 0; i < N_in; ++i) children[i].clear();
    for (int i = 0; i < N_in; ++i) {
        if (parent[i] != -1) {
            children[parent[i]].push_back(i);
        }
    }

    for (int i = 0; i < N_in; ++i) {
        if (parent[i] == -1) {
            dfs_init(i);
        }
    }

    current_score = 0;
    for(int i = 0; i < N_in; ++i) {
        current_score += (long long)(get_height(i) + 1) * A[i];
    }

    best_score = current_score;
    for(int i = 0; i < N_in; ++i) p_best[i] = parent[i];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::steady_clock::now();
    rng.seed(chrono::steady_clock::now().time_since_epoch().count());

    cin >> N_in >> M_in >> H_in;
    for (int i = 0; i < N_in; ++i) cin >> A[i];
    for (int i = 0; i < M_in; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // coordinates are not used in this approach
    for (int i = 0; i < N_in; ++i) {
        int x_dummy, y_dummy;
        cin >> x_dummy >> y_dummy;
    }

    generate_initial_solution();
    init_sa_state();

    double T_start = 2000.0, T_end = 0.1;
    double time_limit = 2.95;

    uniform_int_distribution<int> dist_v(0, N_in - 1);
    uniform_real_distribution<double> dist_p(0.0, 1.0);

    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count() / 1000.0;
        if (elapsed > time_limit) break;

        double progress = elapsed / time_limit;
        double T = T_start * pow(T_end / T_start, progress);

        int v = dist_v(rng);
        
        int p_old = parent[v];
        int u_cand_size = adj[v].size() + 1;
        uniform_int_distribution<int> dist_u(0, u_cand_size - 1);
        int u_cand_idx = dist_u(rng);
        int p_new = (u_cand_idx == (int)adj[v].size()) ? -1 : adj[v][u_cand_idx];

        if (p_new == p_old) continue;

        int curr = p_new;
        bool is_ancestor = false;
        while (curr != -1) {
            if (curr == v) {
                is_ancestor = true;
                break;
            }
            curr = parent[curr];
        }
        if (is_ancestor) continue;
        
        int h_p_new = get_height(p_new);
        int h_v_new = h_p_new + 1;
        
        if (h_v_new + max_depth_subtree[v] > H_in) continue;

        int h_v_old = get_height(v);
        long long score_change = (long long)(h_v_new - h_v_old) * sum_A_subtree[v];
        
        if (score_change >= 0 || exp(score_change / T) > dist_p(rng)) {
            current_score += score_change;

            long long sum_A_v_subtree = sum_A_subtree[v];

            if (p_old != -1) {
                children[p_old].erase(remove(children[p_old].begin(), children[p_old].end(), v), children[p_old].end());
            }
            if (p_new != -1) {
                children[p_new].push_back(v);
            }
            parent[v] = p_new;

            vector<int> path_old, path_new;
            curr = p_old;
            while(curr != -1) { path_old.push_back(curr); curr = parent[curr]; }
            curr = p_new;
            while(curr != -1) { path_new.push_back(curr); curr = parent[curr]; }

            for(int z : path_old) sum_A_subtree[z] -= sum_A_v_subtree;
            for(int z : path_new) sum_A_subtree[z] += sum_A_v_subtree;
            
            for(int z : path_old) recompute_max_d(z);
            for(int z : path_new) recompute_max_d(z);
            
            if (current_score > best_score) {
                best_score = current_score;
                for(int i=0; i<N_in; ++i) p_best[i] = parent[i];
            }
        }
    }

    for (int i = 0; i < N_in; ++i) {
        cout << p_best[i] << (i == N_in - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}
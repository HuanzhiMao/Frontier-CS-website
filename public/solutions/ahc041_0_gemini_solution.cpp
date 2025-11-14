#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>

using namespace std;

const int MAXN = 1000;
int N, M, H;
int A[MAXN];
vector<int> adj[MAXN];
int X[MAXN], Y[MAXN];

struct State {
    vector<int> p;
    vector<int> h;
    vector<vector<int>> children;
    vector<long long> subtree_sum_A;
    vector<int> subtree_max_depth;
    long long score;

    State() : p(N), h(N), children(N), subtree_sum_A(N), subtree_max_depth(N), score(0) {}
    
    // Copy constructor
    State(const State& other) = default;
    
    // Assignment operator
    State& operator=(const State& other) = default;


    void build_children_from_p() {
        for (int i = 0; i < N; ++i) {
            children[i].clear();
        }
        for (int i = 0; i < N; ++i) {
            if (p[i] != -1) {
                children[p[i]].push_back(i);
            }
        }
    }

    void full_rebuild() {
        build_children_from_p();

        vector<int> q;
        q.reserve(N);
        fill(h.begin(), h.end(), -1);
        for (int i = 0; i < N; ++i) {
            if (p[i] == -1) {
                h[i] = 0;
                q.push_back(i);
            }
        }

        int head = 0;
        while(head < q.size()) {
            int u = q[head++];
            for (int v : children[u]) {
                h[v] = h[u] + 1;
                q.push_back(v);
            }
        }
        
        for (int i = N - 1; i >= 0; --i) {
            int u = q[i];
            subtree_sum_A[u] = A[u];
            subtree_max_depth[u] = 0;
            for (int v : children[u]) {
                subtree_sum_A[u] += subtree_sum_A[v];
                subtree_max_depth[u] = max(subtree_max_depth[u], 1 + subtree_max_depth[v]);
            }
        }
        
        score = 0;
        for (int i = 0; i < N; ++i) {
            score += (long long)(h[i] + 1) * A[i];
        }
    }

    void recalc_depth_up(int u) {
        while (u != -1) {
            int old_depth = subtree_max_depth[u];
            int new_depth = 0;
            for (int c : children[u]) {
                new_depth = max(new_depth, 1 + subtree_max_depth[c]);
            }
            if (new_depth == old_depth) {
                break;
            }
            subtree_max_depth[u] = new_depth;
            u = p[u];
        }
    }
};

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto start_time = chrono::high_resolution_clock::now();

    cin >> N >> M >> H;
    for (int i = 0; i < N; ++i) cin >> A[i];
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < N; ++i) cin >> X[i] >> Y[i];

    State current_state;
    
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return A[a] < A[b];
    });

    vector<int> temp_p(N, -2);
    vector<int> temp_h(N, 0);

    for (int v : order) {
        int best_parent = -1;
        int max_h = -1;
        for (int u : adj[v]) {
            if (temp_p[u] != -2) {
                if (temp_h[u] < H) {
                    if(temp_h[u] > max_h) {
                         max_h = temp_h[u];
                         best_parent = u;
                    }
                }
            }
        }
        if (best_parent != -1) {
            temp_p[v] = best_parent;
            temp_h[v] = max_h + 1;
        } else {
            temp_p[v] = -1;
            temp_h[v] = 0;
        }
    }
    current_state.p = temp_p;
    current_state.full_rebuild();

    State best_state = current_state;

    double start_temp = 5000;
    double end_temp = 0.1;
    
    uniform_real_distribution<double> dist_real(0.0, 1.0);

    while (true) {
        auto now = chrono::high_resolution_clock::now();
        double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        if (elapsed_ms > 1950) {
            break;
        }

        double progress = elapsed_ms / 1950.0;
        double temp = start_temp * pow(end_temp / start_temp, progress);

        int v = rng() % N;
        int neighbor_idx = rng() % (adj[v].size() + 1);
        int new_p = (neighbor_idx == adj[v].size()) ? -1 : adj[v][neighbor_idx];
        int old_p = current_state.p[v];
        
        if (new_p == old_p) continue;

        bool is_ancestor = false;
        if (new_p != -1) {
            int curr = new_p;
            while (curr != -1) {
                if (curr == v) {
                    is_ancestor = true;
                    break;
                }
                curr = current_state.p[curr];
            }
        }
        if (is_ancestor) continue;

        int new_h_v = (new_p == -1) ? 0 : current_state.h[new_p] + 1;
        if (new_h_v + current_state.subtree_max_depth[v] > H) continue;

        long long delta_score = (long long)(new_h_v - current_state.h[v]) * current_state.subtree_sum_A[v];

        if (delta_score > 0 || exp(delta_score / temp) > dist_real(rng)) {
            long long v_sum_A = current_state.subtree_sum_A[v];
            int old_h_v = current_state.h[v];

            int curr = old_p;
            while (curr != -1) {
                current_state.subtree_sum_A[curr] -= v_sum_A;
                curr = current_state.p[curr];
            }
            curr = new_p;
            while (curr != -1) {
                current_state.subtree_sum_A[curr] += v_sum_A;
                curr = current_state.p[curr];
            }

            if (old_p != -1) {
                auto& ch = current_state.children[old_p];
                auto it = find(ch.begin(), ch.end(), v);
                if(it != ch.end()) ch.erase(it);
            }
            if (new_p != -1) {
                current_state.children[new_p].push_back(v);
            }

            current_state.p[v] = new_p;

            int delta_h = new_h_v - old_h_v;
            vector<int> q;
            q.reserve(N);
            q.push_back(v);
            current_state.h[v] = new_h_v;
            int head = 0;
            while(head < q.size()){
                int u = q[head++];
                for(int c : current_state.children[u]){
                    current_state.h[c] += delta_h;
                    q.push_back(c);
                }
            }
            
            current_state.recalc_depth_up(old_p);
            current_state.recalc_depth_up(new_p);
            
            current_state.score += delta_score;

            if (current_state.score > best_state.score) {
                best_state = current_state;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        cout << best_state.p[i] << (i == N - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}
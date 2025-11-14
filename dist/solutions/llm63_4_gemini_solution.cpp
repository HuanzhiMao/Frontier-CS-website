#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <map>

using namespace std;

int N, M;
vector<pair<int, int>> edges_orig;
vector<vector<int>> adj;
map<pair<int, int>, int> edge_idx;

vector<int> orientation;

// DFS to make a connected component strongly connected
void make_scc_dfs(int u, int p, const vector<bool>& node_set, vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!node_set[v] || v == p) continue;
        
        int u_small = min(u, v);
        int v_large = max(u, v);
        int idx = edge_idx.at({u_small, v_large});

        if (visited[v]) { // Back edge
            orientation[idx] = (u == edges_orig[idx].second);
        } else { // Tree edge
            orientation[idx] = (u == edges_orig[idx].second);
            make_scc_dfs(v, u, node_set, visited);
        }
    }
}

int ask_query(const vector<bool>& S) {
    orientation.assign(M, 0);

    // Make subgraphs on S and V\S strongly connected
    vector<bool> visited(N, false);
    for (int i = 0; i < N; ++i) {
        if (S[i] && !visited[i]) {
            make_scc_dfs(i, -1, S, visited);
        }
    }
    fill(visited.begin(), visited.end(), false);
    vector<bool> notS(N);
    for(int i=0; i<N; ++i) notS[i] = !S[i];
    for (int i = 0; i < N; ++i) {
        if (notS[i] && !visited[i]) {
            make_scc_dfs(i, -1, notS, visited);
        }
    }

    // Orient edges between S and V\S to go from S to V\S
    for (int i = 0; i < M; ++i) {
        int u = edges_orig[i].first;
        int v = edges_orig[i].second;
        if (S[u] != S[v]) {
            if (S[u]) { // u in S, v not in S. Orient u -> v
                orientation[i] = (u == edges_orig[i].second);
            } else { // v in S, u not in S. Orient v -> u
                orientation[i] = (v == edges_orig[i].second);
            }
        }
    }
    
    cout << 0;
    for (int i = 0; i < M; ++i) {
        cout << " " << orientation[i];
    }
    cout << endl;

    int result;
    cin >> result;
    return result;
}

enum CheckResult { A_IN_B_OUT, A_OUT_B_IN, SAME_SIDE };

CheckResult check(const vector<bool>& S) {
    int res1 = ask_query(S);
    if (res1 == 0) {
        return A_OUT_B_IN;
    }

    vector<bool> S_comp(N);
    for (int i = 0; i < N; ++i) S_comp[i] = !S[i];
    int res2 = ask_query(S_comp);
    if (res2 == 0) {
        return A_IN_B_OUT;
    }

    return SAME_SIDE;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M;
    adj.resize(N);
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        edges_orig.push_back({u, v});
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge_idx[{min(u, v), max(u, v)}] = i;
    }

    int D = 0;
    int a_known_mask = 0;
    int a_known_bits = 0;
    
    int logN_ceil = 0;
    if (N > 1) {
        logN_ceil = floor(log2(N - 1)) + 1;
    }


    for (int k = 0; k < logN_ceil; ++k) {
        vector<bool> S(N, false);
        for (int i = 0; i < N; ++i) {
            if ((i >> k) & 1) {
                S[i] = true;
            }
        }
        CheckResult res = check(S);
        if (res == A_IN_B_OUT) {
            D |= (1 << k);
            a_known_mask |= (1 << k);
            a_known_bits |= (1 << k);
        } else if (res == A_OUT_B_IN) {
            D |= (1 << k);
            a_known_mask |= (1 << k);
        }
    }

    vector<int> cand_A;
    for (int i = 0; i < N; ++i) {
        if ((i & a_known_mask) == a_known_bits) {
            cand_A.push_back(i);
        }
    }

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    while (cand_A.size() > 1) {
        vector<bool> S(N, false);
        for (int i = 0; i < N; ++i) {
            if (uniform_int_distribution<int>(0, 1)(rng)) {
                S[i] = true;
            }
        }
        
        CheckResult res = check(S);

        vector<int> next_cand_A;
        for (int u : cand_A) {
            int b = u ^ D;
            bool u_in_S = S[u];
            bool b_in_S = S[b];

            if (res == A_IN_B_OUT) {
                if (u_in_S && !b_in_S) {
                    next_cand_A.push_back(u);
                }
            } else if (res == A_OUT_B_IN) {
                if (!u_in_S && b_in_S) {
                    next_cand_A.push_back(u);
                }
            } else { // SAME_SIDE
                if (u_in_S == b_in_S) {
                    next_cand_A.push_back(u);
                }
            }
        }
        cand_A = next_cand_A;
    }

    int final_A = cand_A[0];
    int final_B = final_A ^ D;

    cout << 1 << " " << final_A << " " << final_B << endl;

    return 0;
}
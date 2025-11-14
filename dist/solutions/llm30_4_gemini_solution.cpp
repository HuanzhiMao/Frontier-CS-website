#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

// Set up for 1-based indexing
const int N_MAX = 5005;
const int LOGN_MAX = 13; // 2^13 > 5000

int n;
std::vector<int> adj[N_MAX];
int parent[N_MAX];
int depth[N_MAX];
int startTime[N_MAX], endTime[N_MAX], timer;
int ancestor[N_MAX][LOGN_MAX];
int subtree_counts_helper[N_MAX];

void dfs_precompute(int u, int p, int d) {
    parent[u] = p;
    depth[u] = d;
    startTime[u] = ++timer;
    ancestor[u][0] = p;

    for (int v : adj[u]) {
        if (v != p) {
            dfs_precompute(v, u, d + 1);
        }
    }
    endTime[u] = ++timer;
}

void build_binary_lifting() {
    for (int k = 1; k < LOGN_MAX; ++k) {
        for (int i = 1; i <= n; ++i) {
            if (ancestor[i][k - 1] != 0) {
                ancestor[i][k] = ancestor[ancestor[i][k - 1]][k - 1];
            } else {
                ancestor[i][k] = 0;
            }
        }
    }
}

int get_ancestor(int u, int d) {
    if (d < 0) return 0;
    if (d == 0) return u;
    if (u == 0) return 0;
    
    for (int k = LOGN_MAX - 1; k >= 0; --k) {
        if ((d >> k) & 1) {
            u = ancestor[u][k];
            if (u == 0) break;
        }
    }
    return u;
}

bool is_in_subtree(int u, int v) {
    if (u == 0 || v == 0) return false;
    return startTime[v] <= startTime[u] && endTime[u] <= endTime[v];
}

void dfs_counts(int u, int p) {
    for (int v : adj[u]) {
        if (v != p) {
            dfs_counts(v, u);
            subtree_counts_helper[u] += subtree_counts_helper[v];
        }
    }
}

void solve() {
    std::cin >> n;
    for (int i = 1; i <= n; ++i) {
        adj[i].clear();
    }
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer = 0;
    dfs_precompute(1, 1, 0); // parent of root is itself
    build_binary_lifting();

    std::vector<int> S(n);
    std::iota(S.begin(), S.end(), 1);
    int moves = 0;

    while (true) {
        if (S.size() <= 1) break;

        std::vector<int> current_locations;
        current_locations.reserve(S.size());
        for (int v : S) {
            current_locations.push_back(get_ancestor(v, moves));
        }

        bool all_same = true;
        if (S.size() > 1) {
            int first_pos = current_locations[0];
            for (size_t i = 1; i < current_locations.size(); ++i) {
                if (current_locations[i] != first_pos) {
                    all_same = false;
                    break;
                }
            }
        }
        if (all_same) break;

        for (int i = 1; i <= n; ++i) {
            subtree_counts_helper[i] = 0;
        }

        for (int pos : current_locations) {
            if (pos != 0) {
                subtree_counts_helper[pos]++;
            }
        }
        
        dfs_counts(1, 0);

        int best_x = -1;
        int min_max_split = n + 1;
        int min_depth = n + 1;

        for (int i = 1; i <= n; ++i) {
            int in_count = subtree_counts_helper[i];
            int out_count = S.size() - in_count;
            int current_max_split = std::max(in_count, out_count);

            if (current_max_split < min_max_split) {
                min_max_split = current_max_split;
                best_x = i;
                min_depth = depth[i];
            } else if (current_max_split == min_max_split) {
                if (depth[i] < min_depth) {
                    best_x = i;
                    min_depth = depth[i];
                }
            }
        }
        
        if (best_x == -1) best_x = 1; 

        std::cout << "? " << best_x << std::endl;
        int response;
        std::cin >> response;

        std::vector<int> S_new;
        if (response == 1) {
            for (int v : S) {
                if (is_in_subtree(get_ancestor(v, moves), best_x)) {
                    S_new.push_back(v);
                }
            }
        } else { // response == 0
            for (int v : S) {
                if (!is_in_subtree(get_ancestor(v, moves), best_x)) {
                    S_new.push_back(v);
                }
            }
            moves++;
        }
        S = S_new;
    }

    int final_pos = 1;
    if (!S.empty()) {
       final_pos = get_ancestor(S[0], moves);
    }
    
    std::cout << "! " << final_pos << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
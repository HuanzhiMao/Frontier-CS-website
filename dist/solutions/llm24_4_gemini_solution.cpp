#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

// Function to check if an edge (u, v) exists for a given color
bool has_edge(int u, int v, int color, const vector<vector<int>>& C) {
    return C[u - 1][v - 1] == color;
}

// Tries to find a Hamiltonian path for a given color and start node
vector<int> find_hamiltonian_path(int n, const vector<vector<int>>& C, int color, int start_node) {
    vector<int> path;
    path.push_back(start_node);
    vector<bool> used(n + 1, false);
    used[start_node] = true;

    while (path.size() < n) {
        bool extended = false;
        // Try to extend from the back of the path
        int last = path.back();
        // To maintain determinism and potentially find a lexicographically smaller path,
        // we greedily pick the smallest possible next node.
        for (int next_node = 1; next_node <= n; ++next_node) {
            if (!used[next_node] && has_edge(last, next_node, color, C)) {
                path.push_back(next_node);
                used[next_node] = true;
                extended = true;
                break;
            }
        }

        if (extended) {
            continue;
        }

        // If cannot extend, must rotate the path
        bool rotated = false;
        // Find the first possible rotation to maintain determinism.
        for (size_t i = 0; i < path.size() - 1; ++i) { // Path has at least 2 vertices to be non-extendable
            // A rotation is possible with p_i if p_k is connected to p_i.
            // The edge case is connecting to p_{k-1}, which offers no new endpoint.
            if (i < path.size() - 2 && has_edge(path.back(), path[i], C)) {
                reverse(path.begin() + i + 1, path.end());
                rotated = true;
                break;
            }
        }
        
        // If no rotation is possible, this path is stuck
        if (!rotated) {
            return {}; // Failed to find a path from this start_node
        }
    }

    return path;
}

void solve() {
    int n;
    while (cin >> n) {
        vector<vector<int>> C(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            string row_str;
            cin >> row_str;
            for (int j = 0; j < n; ++j) {
                C[i][j] = row_str[j] - '0';
            }
        }

        vector<int> best_p;

        // Try for both colors
        for (int color = 0; color < 2; ++color) {
            // Try all possible starting nodes to find the lexicographically smallest
            for (int start_node = 1; start_node <= n; ++start_node) {
                // If we already have a solution, don't try starting with a larger node
                if (!best_p.empty() && start_node > best_p[0]) {
                    continue;
                }

                vector<int> p = find_hamiltonian_path(n, C, color, start_node);

                if (p.size() == n) {
                    if (best_p.empty() || p < best_p) {
                        best_p = p;
                    }
                }
            }
        }
        
        if (best_p.empty()) {
             cout << -1 << endl;
        } else {
            for (int i = 0; i < n; ++i) {
                cout << best_p[i] << (i == n - 1 ? "" : " ");
            }
            cout << endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
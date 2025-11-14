#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;

int n;
vector<vector<int>> C;

// Tries to build a permutation by greedily picking the smallest available vertex.
// The resulting path p_1, ..., p_n has edge colors C(p_i, p_{i+1}) forming a sequence
// of one color, possibly followed by a sequence of the other color.
vector<int> try_build(int start_node, int start_color) {
    vector<int> p;
    p.push_back(start_node);
    vector<bool> visited(n + 1, false);
    visited[start_node] = true;

    int current_node = start_node;

    // Phase 1: build path with start_color
    int color1 = start_color;
    while (true) {
        int next_node = -1;
        // Find smallest unvisited neighbor with color1
        for (int i = 1; i <= n; ++i) {
            if (!visited[i] && C[current_node - 1][i - 1] == color1) {
                next_node = i;
                break;
            }
        }
        if (next_node == -1) {
            break; // No more nodes can be added with this color
        }
        p.push_back(next_node);
        visited[next_node] = true;
        current_node = next_node;
    }

    // Phase 2: if path is incomplete, switch color and continue
    if (p.size() < n) {
        int color2 = 1 - start_color;
        while (p.size() < n) {
            int next_node = -1;
            // Find smallest unvisited neighbor with color2
            for (int i = 1; i <= n; ++i) {
                if (!visited[i] && C[current_node - 1][i - 1] == color2) {
                    next_node = i;
                    break;
                }
            }
            if (next_node == -1) {
                return {}; // Failed to build a full path
            }
            p.push_back(next_node);
            visited[next_node] = true;
            current_node = next_node;
        }
    }

    return p;
}

void solve() {
    // Iterate through all possible starting nodes to find the lexicographically smallest permutation
    for (int s = 1; s <= n; ++s) {
        // Try starting with a path of color 0
        vector<int> p0 = try_build(s, 0);
        // Try starting with a path of color 1
        vector<int> p1 = try_build(s, 1);

        vector<int> best_p;
        if (!p0.empty() && !p1.empty()) {
            if (lexicographical_compare(p0.begin(), p0.end(), p1.begin(), p1.end())) {
                best_p = p0;
            } else {
                best_p = p1;
            }
        } else if (!p0.empty()) {
            best_p = p0;
        } else if (!p1.empty()) {
            best_p = p1;
        }

        if (!best_p.empty()) {
            for (int i = 0; i < n; ++i) {
                cout << best_p[i] << (i == n - 1 ? "" : " ");
            }
            cout << "\n";
            return;
        }
    }

    cout << -1 << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    while (cin >> n) {
        C.assign(n, vector<int>(n));
        for (int i = 0; i < n; ++i) {
            string row;
            cin >> row;
            for (int j = 0; j < n; ++j) {
                C[i][j] = row[j] - '0';
            }
        }
        solve();
    }

    return 0;
}
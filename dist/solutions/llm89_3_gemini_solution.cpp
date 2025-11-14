#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// Wrapper for judge interaction to ask if vertex v is on Steiner(S).
bool ask(int v, const vector<int>& s) {
    if (s.empty()) {
        return false;
    }
    cout << "? " << s.size() << " " << v;
    for (int u : s) {
        cout << " " << u;
    }
    cout << endl;
    int response;
    cin >> response;
    if (response == -1) {
        exit(0);
    }
    return response == 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "!" << endl;
        return 0;
    }

    // Step 1: Determine depth of each node relative to root 1.
    // depth(v) = dist(1, v) = |path(1,v)| - 1.
    vector<int> depth(n + 1, 0);
    vector<vector<int>> nodes_by_depth(n);
    nodes_by_depth[0].push_back(1);
    
    for (int i = 2; i <= n; ++i) {
        int path_len = 0;
        // The number of nodes on the path(1,i) can be found by
        // checking for every node j if it lies on this path.
        for (int j = 1; j <= n; ++j) {
            if (ask(j, {1, i})) {
                path_len++;
            }
        }
        depth[i] = path_len - 1;
        if (depth[i] < n) {
            nodes_by_depth[depth[i]].push_back(i);
        }
    }

    vector<pair<int, int>> edges;
    // Step 2: For each node, find its parent.
    // The parent of a node u at depth d is the unique node v at depth d-1
    // which is an ancestor of u.
    for (int d = 1; d < n; ++d) {
        for (int u : nodes_by_depth[d]) {
            for (int v : nodes_by_depth[d-1]) {
                // v is an ancestor of u (w.r.t root 1) iff v is on path(1, u).
                if (ask(v, {1, u})) {
                    edges.push_back({u, v});
                    break; // Found unique parent
                }
            }
        }
    }

    cout << "!" << endl;
    for (const auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}
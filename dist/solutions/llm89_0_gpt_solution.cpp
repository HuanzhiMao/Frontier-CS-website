#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int r = 1;

    vector<vector<unsigned char>> onpath(n + 1, vector<unsigned char>(n + 1, 0));
    vector<int> depth(n + 1, 0);

    // For v = r, we know: onpath[w][r] = (w == r), and depth[r] = 0
    depth[r] = 0;

    // Query for all v != r
    for (int v = 1; v <= n; ++v) {
        if (v == r) continue;
        int cnt = 0;
        for (int w = 1; w <= n; ++w) {
            cout << "? 2 " << w << " " << r << " " << v << "\n" << flush;
            int ans;
            if (!(cin >> ans)) return 0;
            if (ans == -1) return 0;
            onpath[w][v] = (unsigned char)(ans == 1);
            if (ans == 1) cnt++;
        }
        depth[v] = cnt - 1; // path length from r to v
    }

    vector<pair<int,int>> edges;
    edges.reserve(n - 1);
    for (int v = 1; v <= n; ++v) {
        if (v == r) continue;
        int parent = -1;
        int targetDepth = depth[v] - 1;
        for (int u = 1; u <= n; ++u) {
            if (depth[u] == targetDepth && onpath[u][v]) {
                parent = u;
                break;
            }
        }
        if (parent == -1) return 0; // should not happen if queries are correct
        edges.emplace_back(parent, v);
    }

    cout << "!\n";
    for (auto &e : edges) {
        cout << e.first << " " << e.second << "\n";
    }
    cout << flush;

    return 0;
}
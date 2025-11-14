#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n;
        if(!(cin >> n)) return 0;

        if (n <= 1) {
            cout << "!" << "\n" << flush;
            continue;
        }

        // Query all pairwise distances
        vector<vector<long long>> d(n + 1, vector<long long>(n + 1, 0));
        for (int i = 1; i <= n; ++i) d[i][i] = 0;

        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                cout << "? " << i << " " << j << "\n" << flush;
                long long dist;
                if(!(cin >> dist)) return 0;
                d[i][j] = d[j][i] = dist;
            }
        }

        // Prim's algorithm on complete graph with distances as weights
        vector<long long> key(n + 1, (long long)4e18);
        vector<int> parent(n + 1, -1);
        vector<char> inMST(n + 1, 0);

        key[1] = 0;
        for (int it = 0; it < n; ++it) {
            int u = -1;
            long long best = (long long)4e18;
            for (int v = 1; v <= n; ++v) {
                if (!inMST[v] && key[v] < best) {
                    best = key[v];
                    u = v;
                }
            }
            if (u == -1) break;
            inMST[u] = 1;

            for (int v = 1; v <= n; ++v) {
                if (!inMST[v] && d[u][v] < key[v]) {
                    key[v] = d[u][v];
                    parent[v] = u;
                }
            }
        }

        cout << "!";
        for (int v = 2; v <= n; ++v) {
            int u = parent[v];
            if (u == -1) u = 1; // fallback, should not happen
            cout << " " << u << " " << v << " " << d[u][v];
        }
        cout << "\n" << flush;
    }

    return 0;
}
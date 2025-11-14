#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        if (!(cin >> n)) return 0;
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Prebuild the string for querying sum over all nodes
        ostringstream oss;
        oss << n;
        for (int i = 1; i <= n; ++i) oss << " " << i;
        string all_nodes = oss.str();

        vector<long long> T(n + 1, 0);
        // Toggle each node once and after each toggle, query the sum over all nodes
        for (int i = 1; i <= n; ++i) {
            cout << "? 2 " << i << endl;
            cout << "? 1 " << all_nodes << endl;
            cout.flush();
            long long val;
            if (!(cin >> val)) return 0;
            T[i] = val;
        }

        // Compute deltas
        vector<long long> delta(n + 1, 0);
        long long sumDelta = 0;
        for (int i = 2; i <= n; ++i) {
            delta[i] = T[i] - T[i - 1];
            sumDelta += delta[i];
        }
        delta[1] = 2 * T[n] - sumDelta;

        // Compute initial x_i from delta: delta[i] = -2 * x_i * s_i
        vector<int> x_init(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            long long a = -delta[i] / 2; // a = x_i * s_i, s_i >= 1
            x_init[i] = (a > 0 ? 1 : -1);
        }

        // Final values after toggling each node once: -x_init[i]
        cout << "! ";
        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << " ";
            cout << -x_init[i];
        }
        cout << endl;
        cout.flush();
    }
    return 0;
}
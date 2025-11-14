#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;

    vector<pair<int,int>> edges;
    bool ok = true;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        if (!(cin >> u >> v)) {
            ok = false;
            break;
        }
        edges.emplace_back(u, v);
    }

    if (!ok || (int)edges.size() != n - 1) {
        edges.clear();
        for (int i = 2; i <= n; ++i) edges.emplace_back(1, i);
    }

    cout << 1;
    for (auto &e : edges) {
        cout << ' ' << e.first << ' ' << e.second;
    }
    cout << '\n';
    return 0;
}
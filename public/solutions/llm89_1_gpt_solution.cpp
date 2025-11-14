#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    if (n == 1) {
        cout << "!" << endl;
        cout.flush();
        return 0;
    }

    int r = 1;
    vector<vector<unsigned char>> M(n + 1, vector<unsigned char>(n + 1, 0));
    vector<int> depth(n + 1, 0);

    for (int y = 1; y <= n; ++y) {
        if (y == r) continue;
        for (int x = 1; x <= n; ++x) {
            cout << "? 2 " << x << " " << r << " " << y << endl;
            cout.flush();
            int ret;
            if (!(cin >> ret)) return 0;
            if (ret == -1) return 0;
            M[x][y] = (ret ? 1 : 0);
            depth[y] += ret;
        }
    }

    depth[r] = 0;
    for (int y = 1; y <= n; ++y) {
        if (y == r) continue;
        depth[y] -= 1; // number of vertices on path - 1
    }

    vector<pair<int, int>> edges;
    edges.reserve(n - 1);
    for (int y = 1; y <= n; ++y) {
        if (y == r) continue;
        int parent = -1;
        for (int x = 1; x <= n; ++x) {
            if (M[x][y] && depth[x] == depth[y] - 1) {
                parent = x;
                break;
            }
        }
        if (parent == -1) return 0; // Should not happen in a valid interaction
        edges.emplace_back(parent, y);
    }

    cout << "!" << endl;
    for (auto &e : edges) {
        cout << e.first << " " << e.second << endl;
    }
    cout.flush();

    return 0;
}
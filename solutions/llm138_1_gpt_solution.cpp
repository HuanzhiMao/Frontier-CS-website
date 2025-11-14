#include <bits/stdc++.h>
using namespace std;

struct Op {
    int t, x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    vector<string> A(n), B(n);
    for (int i = 0; i < n; ++i) cin >> A[i];
    for (int i = 0; i < n; ++i) cin >> B[i];
    // Read presets (ignored in solution)
    vector<pair<int,int>> dims(k);
    vector<vector<string>> presets(k);
    for (int t = 0; t < k; ++t) {
        int np, mp;
        cin >> np >> mp;
        dims[t] = {np, mp};
        presets[t].resize(np);
        for (int i = 0; i < np; ++i) cin >> presets[t][i];
    }

    // Check solvability by multiset equality
    array<long long, 256> cntA{}, cntB{};
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cntA[(unsigned char)A[i][j]]++;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cntB[(unsigned char)B[i][j]]++;
    for (int c = 0; c < 256; ++c) {
        if (cntA[c] != cntB[c]) {
            cout << -1 << '\n';
            return 0;
        }
    }

    vector<Op> ops;
    ops.reserve(8000);

    auto doLeft = [&](int x, int y) {
        // Swap (x,y) with (x,y-1), moving the jelly at (x,y) left by one
        // Print -2 x+1 y+1
        ops.push_back({-2, x + 1, y + 1});
        swap(A[x][y], A[x][y - 1]);
    };
    auto doUp = [&](int x, int y) {
        // Swap (x,y) with (x-1,y), moving the jelly at (x,y) up by one
        // Print -3 x+1 y+1
        ops.push_back({-3, x + 1, y + 1});
        swap(A[x][y], A[x - 1][y]);
    };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (A[i][j] == B[i][j]) continue;
            // Find a matching jelly in the suffix submatrix [i..n-1] x [j..m-1]
            int sx = -1, sy = -1;
            // We can pick the first found; still keeps operations within safe bounds
            for (int x = i; x < n && sx == -1; ++x) {
                for (int y = j; y < m; ++y) {
                    if (A[x][y] == B[i][j]) {
                        sx = x; sy = y; break;
                    }
                }
            }
            if (sx == -1) {
                // Should not happen due to multiset check, but safe-guard
                cout << -1 << '\n';
                return 0;
            }
            // Move (sx, sy) to (i, j) within the suffix by left then up moves
            for (int y = sy; y > j; --y) {
                doLeft(sx, y);
            }
            for (int x = sx; x > i; --x) {
                doUp(x, j);
            }
        }
    }

    // Final check (optional)
    // for (int i = 0; i < n; ++i) if (A[i] != B[i]) { cout << -1 << '\n'; return 0; }

    cout << ops.size() << '\n';
    for (auto &op : ops) {
        cout << op.t << ' ' << op.x << ' ' << op.y << '\n';
    }
    return 0;
}
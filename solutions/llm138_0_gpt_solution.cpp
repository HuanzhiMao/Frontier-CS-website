#include <bits/stdc++.h>
using namespace std;

struct Op {
    int op, x, y;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    vector<string> S(n), T(n);
    for (int i = 0; i < n; ++i) cin >> S[i];
    for (int i = 0; i < n; ++i) cin >> T[i];

    struct Preset { int r, c; vector<string> a; };
    vector<Preset> presets(k+1);
    for (int i = 1; i <= k; ++i) {
        int r, c;
        cin >> r >> c;
        presets[i].r = r; presets[i].c = c;
        presets[i].a.resize(r);
        for (int rr = 0; rr < r; ++rr) cin >> presets[i].a[rr];
    }

    auto idxOf = [&](char ch)->int{
        if ('a' <= ch && ch <= 'z') return ch - 'a';
        if ('A' <= ch && ch <= 'Z') return 26 + (ch - 'A');
        if ('0' <= ch && ch <= '9') return 52 + (ch - '0');
        return -1;
    };

    // find 1x1 presets for characters
    vector<int> one1(62, -1);
    for (int i = 1; i <= k; ++i) {
        if (presets[i].r == 1 && presets[i].c == 1) {
            int id = idxOf(presets[i].a[0][0]);
            if (id >= 0 && one1[id] == -1) one1[id] = i;
        }
    }

    // Prepare operations vector
    vector<Op> ops;
    int preset_used = 0;

    auto apply_preset_1x1 = [&](int pid, int x, int y) {
        // pid corresponds to 1x1
        if (preset_used >= 400) return false;
        char ch = presets[pid].a[0][0];
        S[x-1][y-1] = ch;
        ops.push_back({pid, x, y});
        preset_used++;
        return true;
    };

    auto swap_left = [&](int x, int y) {
        // swap (x,y) with (x,y-1); requires y>1
        ops.push_back({-2, x, y});
        swap(S[x-1][y-1], S[x-1][y-2]);
    };
    auto swap_up = [&](int x, int y) {
        // swap (x,y) with (x-1,y); requires x>1
        ops.push_back({-3, x, y});
        swap(S[x-2][y-1], S[x-1][y-1]);
    };

    auto move_tile = [&](int sx, int sy, int tx, int ty) {
        // assume tx<=sx, ty<=sy
        int x = sx, y = sy;
        while (y > ty) {
            swap_left(x, y);
            y--;
        }
        while (x > tx) {
            swap_up(x, y);
            x--;
        }
    };

    // On-the-fly fill: for each cell except last, try to bring target char from suffix;
    // if not available, try 1x1 preset at the cell.
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (i == n && j == m) continue;
            if (S[i-1][j-1] == T[i-1][j-1]) continue;
            // search in suffix [i..n] x [j..m]
            int fx = -1, fy = -1;
            for (int x = i; x <= n; ++x) {
                for (int y = j; y <= m; ++y) {
                    if (S[x-1][y-1] == T[i-1][j-1]) {
                        fx = x; fy = y;
                        break;
                    }
                }
                if (fx != -1) break;
            }
            if (fx != -1) {
                move_tile(fx, fy, i, j);
            } else {
                int id = idxOf(T[i-1][j-1]);
                if (id == -1 || one1[id] == -1) {
                    // Try a last resort: if counts already match globally, we shouldn't reach here,
                    // but since they may not, we cannot proceed.
                    cout << -1 << "\n";
                    return 0;
                }
                if (!apply_preset_1x1(one1[id], i, j)) {
                    cout << -1 << "\n";
                    return 0;
                }
            }
        }
    }

    // Final cell
    if (S[n-1][m-1] != T[n-1][m-1]) {
        int id = idxOf(T[n-1][m-1]);
        if (id == -1 || one1[id] == -1) {
            // If counts matched initially, last should already match; else we cannot fix.
            // Before failing, check if we can move from anywhere (should not be allowed since we'd disturb fixed cells).
            cout << -1 << "\n";
            return 0;
        }
        if (!apply_preset_1x1(one1[id], n, m)) {
            cout << -1 << "\n";
            return 0;
        }
    }

    // Output operations
    if ((int)ops.size() > 400000) {
        cout << -1 << "\n";
        return 0;
    }
    cout << ops.size() << "\n";
    for (auto &op : ops) {
        cout << op.op << " " << op.x << " " << op.y << "\n";
    }
    return 0;
}
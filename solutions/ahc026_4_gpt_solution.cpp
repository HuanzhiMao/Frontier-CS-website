#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<int>> st(m);
    int per = n / m;
    for (int i = 0; i < m; ++i) {
        st[i].resize(per);
        for (int j = 0; j < per; ++j) cin >> st[i][j];
    }

    vector<pair<int,int>> ops; // (v, i)

    for (int v = 1; v <= n; ++v) {
        int s = -1, pos = -1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < (int)st[i].size(); ++j) {
                if (st[i][j] == v) { s = i; pos = j; break; }
            }
            if (s != -1) break;
        }
        if (s == -1) continue; // should not happen

        int h = (int)st[s].size();
        if (pos != h - 1) {
            int w = st[s][pos + 1];
            int t = -1;
            size_t minsz = SIZE_MAX;
            for (int i = 0; i < m; ++i) {
                if (i == s) continue;
                if (st[i].size() < minsz) {
                    minsz = st[i].size();
                    t = i;
                }
            }
            // move chunk starting at pos+1 to stack t
            vector<int> chunk;
            for (int k = pos + 1; k < h; ++k) chunk.push_back(st[s][k]);
            st[s].resize(pos + 1);
            st[t].insert(st[t].end(), chunk.begin(), chunk.end());
            ops.emplace_back(w, t + 1);
        }
        // now v is at top of stack s
        st[s].pop_back();
        ops.emplace_back(v, 0);
    }

    for (auto &op : ops) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}
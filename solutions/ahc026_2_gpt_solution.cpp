#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<int>> st(m);
    for (int i = 0; i < m; ++i) {
        st[i].resize(n / m);
        for (int j = 0; j < n / m; ++j) cin >> st[i][j];
    }

    vector<pair<int,int>> ops;
    ops.reserve(2 * n + 1000);

    auto moveSegment = [&](int s, int startIdx, int d) {
        // Move st[s][startIdx..end] to top of stack d
        int sz = (int)st[s].size();
        if (startIdx >= sz) return; // nothing to move
        vector<int> seg;
        seg.reserve(sz - startIdx);
        for (int k = startIdx; k < sz; ++k) seg.push_back(st[s][k]);
        int vbottom = seg[0];
        st[s].resize(startIdx);
        for (int x : seg) st[d].push_back(x);
        ops.emplace_back(vbottom, d + 1);
    };

    auto removeTop = [&](int s) {
        int v = st[s].back();
        st[s].pop_back();
        ops.emplace_back(v, 0);
    };

    auto findLabel = [&](int val) -> pair<int,int> {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < (int)st[i].size(); ++j) {
                if (st[i][j] == val) return {i, j};
            }
        }
        return {-1, -1};
    };

    const int INF_TOP = n + 1;
    const int LOOKAHEAD = 8; // avoid burying next few top elements

    auto selectDest = [&](int source, int v_current) -> int {
        vector<char> blocked(m, 0);
        // Avoid burying soon-to-be-removed top elements among next LOOKAHEAD numbers
        for (int w = v_current + 1; w <= min(n, v_current + LOOKAHEAD); ++w) {
            auto [si, idx] = findLabel(w);
            if (si != -1 && idx == (int)st[si].size() - 1) {
                blocked[si] = 1;
            }
        }
        int best = -1;
        int bestTop = -1;
        int bestSize = INT_MAX;

        auto consider = [&](int i) {
            int topv = st[i].empty() ? INF_TOP : st[i].back();
            int sz = (int)st[i].size();
            if (topv > bestTop || (topv == bestTop && sz < bestSize)) {
                bestTop = topv;
                bestSize = sz;
                best = i;
            }
        };

        // Prefer unblocked stacks
        for (int i = 0; i < m; ++i) {
            if (i == source) continue;
            if (!blocked[i]) consider(i);
        }
        if (best != -1) return best;

        // If all candidates are blocked, choose the best among them
        for (int i = 0; i < m; ++i) {
            if (i == source) continue;
            consider(i);
        }
        if (best == -1) {
            // Fallback (should not happen as there are m-1 candidates)
            for (int i = 0; i < m; ++i) if (i != source) { best = i; break; }
        }
        return best;
    };

    for (int v = 1; v <= n; ++v) {
        auto [s, idx] = findLabel(v);
        if (s == -1) continue; // should not happen
        if (idx != (int)st[s].size() - 1) {
            int dest = selectDest(s, v);
            moveSegment(s, idx + 1, dest);
        }
        // Now v should be at top of stack s
        // It's guaranteed smallest among remaining since we process in order
        removeTop(s);
    }

    for (auto &p : ops) {
        cout << p.first << ' ' << p.second << '\n';
    }
    return 0;
}
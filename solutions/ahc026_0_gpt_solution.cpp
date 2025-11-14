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
        st[i].reserve(n);
        for (int j = 0; j < per; ++j) {
            int x; cin >> x;
            st[i].push_back(x);
        }
    }
    vector<pair<int,int>> pos(n + 1, {-1, -1});
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < (int)st[i].size(); ++j) {
            pos[st[i][j]] = {i, j};
        }
    }

    auto vmin_of_stack = [&](const vector<int>& v) -> int {
        if (v.empty()) return -1;
        int mn = v[0];
        for (int x : v) if (x < mn) mn = x;
        return mn;
    };

    auto choose_destination = [&](int s, int nextMin, int segMin) -> int {
        const int H = 3; // horizon to avoid burying immediate next minima
        // Prefer empty stack if available
        int bucket = (segMin - 1) / (n / m);
        vector<int> empties;
        for (int i = 0; i < m; ++i) if (i != s && st[i].empty()) empties.push_back(i);
        if (!empties.empty()) {
            // Prefer the bucket stack if it's empty
            for (int i : empties) if (i == bucket) return i;
            return empties[0];
        }
        // Build candidate list avoiding burying near-future tops
        vector<int> cand;
        for (int i = 0; i < m; ++i) {
            if (i == s) continue;
            if (st[i].empty()) continue;
            int topv = st[i].back();
            if (topv >= nextMin && topv <= nextMin + H) continue; // avoid bury
            cand.push_back(i);
        }
        if (cand.empty()) {
            for (int i = 0; i < m; ++i) {
                if (i == s) continue;
                if (st[i].empty()) continue;
                cand.push_back(i);
            }
        }
        // If bucket stack is a candidate, prefer it
        for (int i : cand) if (i == bucket) return i;
        // Otherwise choose by heuristic: largest min value in stack, then larger top, then smaller height
        int best = cand[0];
        auto score = [&](int i) {
            int vmn = vmin_of_stack(st[i]);
            int topv = st[i].empty() ? -1 : st[i].back();
            int h = (int)st[i].size();
            // combine into a tuple-like ordering
            return tuple<int,int,int>(vmn, topv, -h);
        };
        auto bestScore = score(best);
        for (int i = 1; i < (int)cand.size(); ++i) {
            auto sc = score(cand[i]);
            if (sc > bestScore) {
                bestScore = sc;
                best = cand[i];
            }
        }
        return best;
    };

    vector<pair<int,int>> ops;

    int t = 1;
    while (t <= n) {
        auto [s, j] = pos[t];
        int h = (int)st[s].size();
        if (j == h - 1) {
            // Carry out
            ops.emplace_back(t, 0);
            st[s].pop_back();
            // No need to update other positions; removing top doesn't change indices below
            t++;
        } else {
            int idx = j + 1; // bottom of the segment above t
            int segMin = INT_MAX;
            for (int p = idx; p < h; ++p) segMin = min(segMin, st[s][p]);
            int dest = choose_destination(s, t, segMin);
            int v_move = st[s][idx];

            // Move segment [idx, h)
            vector<int> seg;
            seg.reserve(h - idx);
            for (int p = idx; p < h; ++p) seg.push_back(st[s][p]);
            st[s].resize(idx);
            int destSize = (int)st[dest].size();
            for (int p = 0; p < (int)seg.size(); ++p) {
                int x = seg[p];
                st[dest].push_back(x);
                pos[x] = {dest, destSize + p};
            }
            // pos[t] remains valid (same stack, same index j)
            ops.emplace_back(v_move, dest + 1); // stack index is 1-based
        }
    }

    for (auto &pr : ops) {
        cout << pr.first << " " << pr.second << "\n";
    }
    return 0;
}
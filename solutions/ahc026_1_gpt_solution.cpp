#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    vector<vector<int>> stacks(m);
    vector<int> pos_stack(n + 1), pos_index(n + 1);
    int L = n / m;
    for (int i = 0; i < m; i++) {
        stacks[i].reserve(n);
        for (int j = 0; j < L; j++) {
            int x; cin >> x;
            stacks[i].push_back(x);
            pos_stack[x] = i;
            pos_index[x] = j;
        }
    }
    
    vector<pair<int,int>> ops;
    
    auto moveSegmentByBox = [&](int v, int t) {
        int s = pos_stack[v];
        int j = pos_index[v];
        if (s == t) {
            for (int i = 0; i < m; i++) if (i != s) { t = i; break; }
        }
        ops.emplace_back(v, t + 1);
        int old_t_size = (int)stacks[t].size();
        int old_s_size = (int)stacks[s].size();
        vector<int> seg;
        seg.reserve(old_s_size - j);
        for (int idx = j; idx < old_s_size; idx++) seg.push_back(stacks[s][idx]);
        stacks[s].resize(j);
        stacks[t].insert(stacks[t].end(), seg.begin(), seg.end());
        for (int p = 0; p < (int)seg.size(); p++) {
            int x = seg[p];
            pos_stack[x] = t;
            pos_index[x] = old_t_size + p;
        }
    };
    
    auto carryOut = [&](int v) {
        int s = pos_stack[v];
        ops.emplace_back(v, 0);
        stacks[s].pop_back();
        pos_stack[v] = -1;
        pos_index[v] = -1;
    };
    
    int cur = 1;
    while (cur <= n) {
        int s = pos_stack[cur];
        int j = pos_index[cur];
        if (j != (int)stacks[s].size() - 1) {
            int t = -1, minSize = INT_MAX;
            for (int i = 0; i < m; i++) if (i != s) {
                if ((int)stacks[i].size() < minSize) {
                    minSize = (int)stacks[i].size();
                    t = i;
                }
            }
            int v_above = stacks[s][j + 1];
            moveSegmentByBox(v_above, t);
        } else {
            carryOut(cur);
            cur++;
        }
    }
    
    for (auto &p : ops) cout << p.first << " " << p.second << "\n";
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    int h = n / m;
    vector<vector<int>> stacks(m);
    for (int i = 0; i < m; ++i) {
        stacks[i].resize(h);
        for (int j = 0; j < h; ++j) cin >> stacks[i][j];
    }

    vector<pair<int,int>> ops;

    for (int t = 1; t <= n; ++t) {
        bool carried = false;
        // If t is already at the top of some stack, carry it out.
        for (int s = 0; s < m; ++s) {
            if (!stacks[s].empty() && stacks[s].back() == t) {
                ops.emplace_back(t, 0);
                stacks[s].pop_back();
                carried = true;
                break;
            }
        }
        if (carried) continue;

        // Otherwise, find t and move the segment above it to another stack.
        for (int s = 0; s < m; ++s) {
            int sz = (int)stacks[s].size();
            int idx = -1;
            for (int j = 0; j < sz; ++j) {
                if (stacks[s][j] == t) { idx = j; break; }
            }
            if (idx == -1) continue;

            // idx < sz - 1 since t is not at top yet
            int u = stacks[s][idx + 1]; // the box right above t
            int d = (s + 1) % m;        // choose a different stack
            if (d == s) d = (s + 1) % m; // just in case (m>=2 per problem)

            // Move u and all boxes above it to stack d
            vector<int> segment(stacks[s].begin() + idx + 1, stacks[s].end());
            stacks[d].insert(stacks[d].end(), segment.begin(), segment.end());
            stacks[s].erase(stacks[s].begin() + idx + 1, stacks[s].end());
            ops.emplace_back(u, d + 1);

            // Now t is at the top of stack s; carry it out
            ops.emplace_back(t, 0);
            stacks[s].pop_back();

            break;
        }
    }

    for (auto &op : ops) {
        cout << op.first << ' ' << op.second << '\n';
    }
    return 0;
}
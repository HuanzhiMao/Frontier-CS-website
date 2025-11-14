#include <bits/stdc++.h>
using namespace std;

int n, k;

bool ask(int c) {
    cout << "? " << c << endl;
    cout.flush();
    string s;
    if (!(cin >> s)) exit(0);
    return s[0] == 'Y';
}

void reset() {
    cout << "R" << endl;
    cout.flush();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> k)) return 0;

    int s = (k == 1 ? 1 : k / 2);
    if (s < 1) s = 1;

    vector<vector<int>> groups;
    for (int i = 1; i <= n; i += s) {
        vector<int> g;
        for (int j = i; j <= n && j < i + s; ++j) g.push_back(j);
        groups.push_back(g);
    }

    int m = (int)groups.size();
    vector<vector<int>> reps(m);
    vector<char> isRep(n + 1, 0);

    // For each group, get representatives (unique within group)
    for (int gi = 0; gi < m; ++gi) {
        reset();
        for (int idx : groups[gi]) {
            bool y = ask(idx);
            if (!y) { // 'N' means new within this group
                reps[gi].push_back(idx);
                isRep[idx] = 1;
            }
        }
    }

    // Initially all reps are considered unique globally
    vector<char> uniqueRep(n + 1, 0);
    for (int i = 1; i <= n; ++i) if (isRep[i]) uniqueRep[i] = 1;

    // For each pair of groups, mark duplicates across them
    for (int i = 0; i < m; ++i) {
        if (reps[i].empty()) continue;
        for (int j = i + 1; j < m; ++j) {
            if (reps[j].empty()) continue;
            reset();
            // Load reps of group i
            for (int x : reps[i]) {
                ask(x);
            }
            // Test reps of group j
            for (int yIdx : reps[j]) {
                bool y = ask(yIdx);
                if (y) uniqueRep[yIdx] = 0;
            }
        }
    }

    int d = 0;
    for (int i = 1; i <= n; ++i) if (uniqueRep[i]) ++d;

    cout << "! " << d << endl;
    cout.flush();
    return 0;
}
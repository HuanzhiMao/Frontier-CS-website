#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n=0): n(n), bit(n+1, 0) {}
    void init(int n_) { n = n_; bit.assign(n+1, 0); }
    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }
    int sum(int idx) const {
        int r = 0;
        for (; idx > 0; idx -= idx & -idx) r += bit[idx];
        return r;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n+1), pos(n+1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        pos[a[i]] = i;
    }

    // Find smallest k such that pos[k] < pos[k+1] < ... < pos[n]
    int k = n;
    while (k > 1 && pos[k-1] < pos[k]) --k;

    int moves = k - 1;
    long long total_cost = moves; // y = 1 for all moves
    long long final_cost = (total_cost + 1) * ( (long long)moves + 1 );

    // Prepare Fenwick and positions for "move to front" simulation (Movie Collection trick)
    int M = n + moves + 5;
    Fenwick fw(M);
    vector<int> curpos(n+1);
    int topPtr = moves; // reserved top area [1..moves]
    for (int i = 1; i <= n; ++i) {
        curpos[a[i]] = moves + i;
        fw.add(curpos[a[i]], 1);
    }

    vector<pair<int,int>> ops;
    ops.reserve(moves);
    for (int val = k-1; val >= 1; --val) {
        int x = fw.sum(curpos[val]);
        ops.emplace_back(x, 1);
        fw.add(curpos[val], -1);
        curpos[val] = topPtr;
        fw.add(curpos[val], 1);
        --topPtr;
    }

    cout << final_cost << " " << ops.size() << "\n";
    for (auto &p : ops) {
        cout << p.first << " " << p.second << "\n";
    }
    return 0;
}
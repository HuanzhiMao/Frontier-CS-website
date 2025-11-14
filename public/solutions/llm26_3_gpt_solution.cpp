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
    vector<int> v(n+1), pos(n+1);
    for (int i = 1; i <= n; ++i) {
        cin >> v[i];
        pos[v[i]] = i;
    }

    int k = 1;
    int last = n + 1;
    for (int i = n; i >= 1; --i) {
        if (pos[i] < last) last = pos[i];
        else { k = i + 1; break; }
        if (i == 1) k = 1;
    }

    int M = 2 * n + 5;
    Fenwick fw(M);
    vector<int> key(n+1);
    for (int t = 1; t <= n; ++t) {
        key[t] = n + pos[t];
        fw.add(key[t], 1);
    }

    int front = n;
    vector<pair<int,int>> moves;
    moves.reserve(max(0, k-1));
    long long sum_cost = 0;

    for (int t = k-1; t >= 1; --t) {
        int oldKey = key[t];
        int x = fw.sum(oldKey);
        moves.emplace_back(x, 1);
        sum_cost += 1;
        fw.add(oldKey, -1);
        fw.add(front, +1);
        key[t] = front;
        --front;
    }

    long long num_moves = (long long)moves.size();
    long long final_cost = (sum_cost + 1) * (num_moves + 1);

    cout << final_cost << " " << moves.size() << "\n";
    for (auto &mv : moves) {
        cout << mv.first << " " << mv.second << "\n";
    }
    return 0;
}
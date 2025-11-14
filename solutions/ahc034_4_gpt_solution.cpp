#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> h(N, vector<int>(N));
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j) cin >> h[i][j];

    vector<tuple<int,int,int>> pos; // (r,c,amount) h>0
    vector<tuple<int,int,int>> neg; // (r,c,amount) -h for h<0
    long long baseHeight = h[0][0];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == 0 && j == 0) continue;
            if (h[i][j] > 0) pos.emplace_back(i, j, h[i][j]);
            else if (h[i][j] < 0) neg.emplace_back(i, j, -h[i][j]);
        }
    }

    auto distToBase = [&](const tuple<int,int,int>& t) {
        return get<0>(t) + get<1>(t);
    };
    sort(pos.begin(), pos.end(), [&](const auto& a, const auto& b) {
        return distToBase(a) < distToBase(b);
    });
    sort(neg.begin(), neg.end(), [&](const auto& a, const auto& b) {
        return distToBase(a) < distToBase(b);
    });

    vector<string> ops;
    int r = 0, c = 0;
    auto go = [&](int tr, int tc) {
        while (r < tr) { ops.emplace_back("D"); ++r; }
        while (r > tr) { ops.emplace_back("U"); --r; }
        while (c < tc) { ops.emplace_back("R"); ++c; }
        while (c > tc) { ops.emplace_back("L"); --c; }
    };

    // Stage 1: Bring all positive soil to base (0,0)
    for (auto &t : pos) {
        int pr, pc, amt;
        tie(pr, pc, amt) = t;
        if (amt <= 0) continue;
        go(pr, pc);
        ops.emplace_back("+" + to_string(amt)); // load from positive cell
        go(0, 0);
        ops.emplace_back("-" + to_string(amt)); // unload to base
        baseHeight += amt;
    }

    // Stage 2: Deliver from base to all negative cells (excluding base)
    for (auto &t : neg) {
        int nr, nc, amt;
        tie(nr, nc, amt) = t;
        if (amt <= 0) continue;
        go(0, 0);
        ops.emplace_back("+" + to_string(amt)); // load from base
        baseHeight -= amt;
        go(nr, nc);
        ops.emplace_back("-" + to_string(amt)); // unload to negative cell
        go(0, 0);
    }

    for (const auto &s : ops) {
        cout << s << '\n';
    }
    return 0;
}
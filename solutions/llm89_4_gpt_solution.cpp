#include <bits/stdc++.h>
using namespace std;

static inline void flushout() {
    cout.flush();
}

int n;

int ask2(int v, int a, int b) {
    // Query: is v on path between a and b?
    cout << "? 2 " << v << " " << a << " " << b << endl;
    flushout();
    int ans;
    if (!(cin >> ans)) {
        exit(0);
    }
    if (ans == -1) exit(0);
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    if (n == 1) {
        cout << "!" << endl;
        flushout();
        return 0;
    }

    int r = 1;
    vector<pair<int,int>> edges;
    edges.reserve(n - 1);

    for (int u = 1; u <= n; ++u) {
        if (u == r) continue;
        int cand = -1;
        for (int v = 1; v <= n; ++v) {
            if (v == u || v == r) continue;
            int on = ask2(v, r, u);
            if (on == 1) {
                if (cand == -1) {
                    cand = v;
                } else {
                    int closer = ask2(cand, v, u); // 1 if cand is between v and u (i.e., closer to u)
                    if (closer == 1) {
                        // keep cand
                    } else {
                        cand = v;
                    }
                }
            }
        }
        if (cand == -1) cand = r;
        edges.emplace_back(u, cand);
    }

    cout << "!" << endl;
    for (auto &e : edges) {
        cout << e.first << " " << e.second << endl;
    }
    flushout();
    return 0;
}
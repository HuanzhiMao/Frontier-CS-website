#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    if (n == 1) {
        cout << 1 << " " << 1 << endl;
        cout.flush();
        return 0;
    }
    vector<vector<int>> bitv(n + 1, vector<int>(10));
    int root = 1;
    for (int k = 0; k < 10; k++) {
        int mask = 1 << k;
        int m = 0;
        for (int i = 1; i <= n; i++) {
            if (i & mask) m++;
        }
        if (m == 0 || m == n) {
            int b = (m == n ? 1 : 0);
            for (int v = 1; v <= n; v++) {
                bitv[v][k] = b;
            }
            continue;
        }
        vector<int> ds(n + 1, 0);
        for (int v = 1; v <= n; v++) {
            if (v == root) continue;
            cout << 0;
            for (int i = 1; i <= n; i++) {
                int qval = (i & mask) ? v : root;
                cout << " " << qval;
            }
            cout << endl;
            cout.flush();
            int x;
            cin >> x;
            ds[v] = x - 1;
        }
        int mind = 2, maxd = -2;
        for (int v = 1; v <= n; v++) {
            if (v == root) continue;
            mind = min(mind, ds[v]);
            maxd = max(maxd, ds[v]);
        }
        int broot;
        if (maxd == 1) {
            broot = 0;
        } else {
            broot = 1;
        }
        bitv[root][k] = broot;
        for (int v = 1; v <= n; v++) {
            if (v == root) continue;
            bitv[v][k] = broot + ds[v];
        }
    }
    vector<int> pos(n + 1);
    for (int v = 1; v <= n; v++) {
        int p = 0;
        for (int k = 0; k < 10; k++) {
            p += bitv[v][k] * (1 << k);
        }
        pos[v] = p;
    }
    vector<int> perm(n + 1);
    for (int v = 1; v <= n; v++) {
        perm[pos[v]] = v;
    }
    cout << 1;
    for (int i = 1; i <= n; i++) {
        cout << " " << perm[i];
    }
    cout << endl;
    cout.flush();
    return 0;
}
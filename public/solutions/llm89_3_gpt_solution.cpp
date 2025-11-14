#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    int r = 1;
    vector<vector<unsigned char>> mp(n + 1, vector<unsigned char>(n + 1, 0));
    mp[r][r] = 1;

    auto ask2 = [&](int v_query, int s1, int s2) -> int {
        cout << "? 2 " << v_query << " " << s1 << " " << s2 << "\n" << flush;
        int ans;
        if (!(cin >> ans)) exit(0);
        if (ans == -1) exit(0);
        return ans;
    };

    // Fill mp[w][v] = (w lies on path(r, v))
    for (int v = 1; v <= n; ++v) {
        if (v == r) continue; // mp[*][r] is trivial: only r lies on path(r, r)
        for (int w = 1; w <= n; ++w) {
            if (w == r || w == v) {
                mp[w][v] = 1;
            } else {
                mp[w][v] = (unsigned char)ask2(w, r, v);
            }
        }
    }

    auto get_mp = [&](int a, int b) -> int {
        if (b == r) return (a == r) ? 1 : 0; // path(r, r) contains only r
        return mp[a][b];
    };

    vector<int> parent(n + 1, 0);
    for (int v = 1; v <= n; ++v) {
        if (v == r) continue;
        int c2 = r;
        for (int x = 1; x <= n; ++x) {
            if (!mp[x][v]) continue; // x not on path(r, v)
            if (x == v) continue;    // skip v itself
            if (get_mp(c2, x)) c2 = x;
        }
        parent[v] = c2;
    }

    cout << "!\n";
    for (int v = 1; v <= n; ++v) {
        if (v == r) continue;
        cout << parent[v] << " " << v << "\n";
    }
    cout.flush();
    return 0;
}
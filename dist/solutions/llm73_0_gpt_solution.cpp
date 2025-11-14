#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<vector<unsigned char>> f(n + 2, vector<unsigned char>(n + 2, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            cout << 0 << " " << i << " " << j << "\n";
            cout.flush();
            int ans;
            if (!(cin >> ans)) return 0;
            if (ans == -1) return 0;
            f[i][j] = (unsigned char)(ans & 1);
        }
    }

    auto getf = [&](int l, int r) -> unsigned char {
        if (l >= r) return 0;
        return f[l][r];
    };

    auto gcalc = [&](int i, int j) -> unsigned char {
        // valid only for i < j
        return (unsigned char)(getf(i, j) ^ getf(i + 1, j) ^ getf(i, j - 1) ^ getf(i + 1, j - 1));
    };

    vector<int> p(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int lessCnt = 0;
        for (int j = 1; j < i; ++j) {
            // comp(i, j) = 1 - g(j, i)
            lessCnt += 1 - gcalc(j, i);
        }
        for (int j = i + 1; j <= n; ++j) {
            // comp(i, j) = g(i, j)
            lessCnt += gcalc(i, j);
        }
        p[i] = lessCnt + 1;
    }

    cout << 1;
    for (int i = 1; i <= n; ++i) cout << " " << p[i];
    cout << "\n";
    cout.flush();

    return 0;
}
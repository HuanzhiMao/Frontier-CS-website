#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    long long m1 = 1LL * n * (n - 1) / 2;
    long long m2 = 1LL * n * (n + 1) / 2;

    vector<int> arr;
    arr.reserve((size_t)m2);
    int x;
    while (cin >> x) arr.push_back(x & 1);

    vector<vector<unsigned char>> T(n + 2, vector<unsigned char>(n + 2, 0));

    if ((long long)arr.size() >= m1) {
        // Assume upper triangle without diagonal in lex order: (1,2), (1,3), ..., (1,n), (2,3), ..., (n-1,n)
        size_t pos = 0;
        for (int l = 1; l <= n; ++l) {
            for (int r = l + 1; r <= n; ++r) {
                if (pos < arr.size()) T[l][r] = (unsigned char)(arr[pos++] & 1);
            }
        }
    } else if ((long long)arr.size() == m2) {
        // Assume upper triangle including diagonal in lex order: (1,1..n), (2,2..n), ...
        size_t pos = 0;
        for (int l = 1; l <= n; ++l) {
            for (int r = l; r <= n; ++r) {
                int v = arr[pos++] & 1;
                if (l < r) T[l][r] = (unsigned char)v;
            }
        }
    } else {
        // Not enough data; cannot proceed
        // Fallback: output identity permutation
        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << ' ';
            cout << i;
        }
        cout << '\n';
        return 0;
    }

    auto getT = [&](int l, int r) -> unsigned char {
        if (l >= r) return 0;
        return T[l][r];
    };

    vector<vector<unsigned char>> inv(n + 2, vector<unsigned char>(n + 2, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            inv[i][j] = (unsigned char)((getT(i, j) ^ getT(i, j - 1) ^ getT(i + 1, j) ^ getT(i + 1, j - 1)) & 1);
        }
    }

    vector<int> p(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int smaller = 0;
        for (int j = 1; j < i; ++j) {
            if (inv[j][i] == 0) ++smaller; // p_j < p_i
        }
        for (int j = i + 1; j <= n; ++j) {
            if (inv[i][j] == 1) ++smaller; // p_j < p_i
        }
        p[i] = smaller + 1;
    }

    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << p[i];
    }
    cout << '\n';

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    auto ask = [&](const vector<int>& s) -> int {
        cout << 0;
        for (int x : s) {
            cout << ' ' << x;
        }
        cout << '\n' << flush;
        int x;
        if (!(cin >> x)) exit(0);
        if (x == -1) exit(0);
        return x;
    };

    auto give_answer = [&](const vector<int>& perm) {
        cout << 1;
        for (int i = 0; i < (int)perm.size(); ++i) {
            cout << ' ' << perm[i];
        }
        cout << '\n' << flush;
        // exit immediately
    };

    if (n == 1) {
        give_answer(vector<int>{1});
        return 0;
    }

    int m = 0;
    while ((1 << m) < n) ++m;

    int b = 1, c = 2;

    // Precompute masks for each bit
    vector<vector<char>> mask(m, vector<char>(n, 0));
    vector<int> sizeT(m, 0);
    for (int t = 0; t < m; ++t) {
        int cnt = 0;
        for (int j = 0; j < n; ++j) {
            int in = ((j >> t) & 1);
            mask[t][j] = (char)in;
            if (in) ++cnt;
        }
        sizeT[t] = cnt;
    }

    vector<int> pos_bits(n + 1, 0); // stores value of (pos[v]-1), built bit by bit

    vector<int> s(n);
    vector<int> delta(n + 1);

    for (int t = 0; t < m; ++t) {
        // Query r2: 'b' on T, 'c' elsewhere
        for (int j = 0; j < n; ++j) {
            s[j] = mask[t][j] ? b : c;
        }
        int r2 = ask(s);

        long long sumDelta = 0;

        // v = c: delta = 1 - r2
        delta[c] = 1 - r2;

        // v = b: delta = 0
        delta[b] = 0;

        // For other v:
        for (int v = 1; v <= n; ++v) {
            if (v == b || v == c) continue;
            for (int j = 0; j < n; ++j) {
                s[j] = mask[t][j] ? v : c;
            }
            int r1 = ask(s);
            delta[v] = r1 - r2;
        }

        for (int v = 1; v <= n; ++v) sumDelta += delta[v];

        int Ub_num = sizeT[t] - (int)sumDelta;
        int Ub = Ub_num / n; // should be 0 or 1

        for (int v = 1; v <= n; ++v) {
            int bit = delta[v] + Ub; // should be 0 or 1
            if (bit < 0) bit = 0;
            if (bit > 1) bit = 1;
            pos_bits[v] |= (bit << t);
        }
    }

    // Build permutation from positions
    vector<int> perm(n + 1, 0);
    vector<char> used(n + 1, 0);
    vector<int> bad_vals;
    for (int v = 1; v <= n; ++v) {
        int idx = pos_bits[v] + 1;
        if (idx >= 1 && idx <= n && !used[idx]) {
            perm[idx] = v;
            used[idx] = 1;
        } else {
            bad_vals.push_back(v);
        }
    }
    vector<int> free_idx;
    for (int i = 1; i <= n; ++i) {
        if (!used[i]) free_idx.push_back(i);
    }
    for (size_t k = 0; k < bad_vals.size() && k < free_idx.size(); ++k) {
        perm[free_idx[k]] = bad_vals[k];
    }

    vector<int> ans(n);
    for (int i = 1; i <= n; ++i) ans[i - 1] = perm[i];

    give_answer(ans);
    return 0;
}
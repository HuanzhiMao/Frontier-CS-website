#include <bits/stdc++.h>
using namespace std;

static inline int eval_poly_mod7(int idx1based, int x) {
    int t = idx1based - 1;
    int a0 = t % 7; t /= 7;
    int a1 = t % 7; t /= 7;
    int a2 = t % 7; t /= 7;
    int a3 = t % 7; t /= 7;
    int val = a3;
    val = (val * x + a2) % 7;
    val = (val * x + a1) % 7;
    val = (val * x + a0) % 7;
    return val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, H;
    if (!(cin >> R >> H)) return 0;

    const int q = 7;  // field size
    const int k = 4;  // degree < 4 polynomials
    const int s = 7;  // number of evaluation points (must be >= 2*(k-1)+1 = 7)

    // Precompute sets for each (evaluation point j, value v)
    // evaluation points x_j = j for j=0..6 in F7
    vector<vector<vector<int>>> sets(s, vector<vector<int>>(q));
    for (int j = 0; j < s; ++j) {
        int x = j;
        for (int i = 1; i <= 1000; ++i) {
            int v = eval_poly_mod7(i, x);
            sets[j][v].push_back(i);
        }
    }

    // Send queries
    for (int j = 0; j < s; ++j) {
        for (int v = 0; v < q; ++v) {
            auto &vec = sets[j][v];
            cout << "? " << vec.size();
            for (int p : vec) cout << ' ' << p;
            cout << '\n' << flush;
        }
    }

    // Get results after one hour
    cout << "@\n" << flush;

    int L;
    if (!(cin >> L)) return 0;
    vector<int> ans(L);
    for (int i = 0; i < L; ++i) cin >> ans[i];

    // Parse answers into y[j][v]
    vector<vector<int>> y(s, vector<int>(q, 0));
    int idx = 0;
    for (int j = 0; j < s; ++j) {
        for (int v = 0; v < q; ++v) {
            if (idx < L) y[j][v] = ans[idx++];
        }
    }

    // Decode: candidates whose codeword is contained in the observed OR
    vector<int> cand;
    for (int i = 1; i <= 1000; ++i) {
        bool ok = true;
        for (int j = 0; j < s && ok; ++j) {
            int x = j;
            int v = eval_poly_mod7(i, x);
            if (y[j][v] == 0) ok = false;
        }
        if (ok) cand.push_back(i);
    }

    if (cand.size() == 1) {
        cout << "! " << cand[0] << ' ' << cand[0] << '\n' << flush;
    } else if (cand.size() >= 2) {
        cout << "! " << cand[0] << ' ' << cand[1] << '\n' << flush;
    } else {
        // Fallback (should not happen with the used construction)
        cout << "! 1 1\n" << flush;
    }

    return 0;
}
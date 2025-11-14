#include <bits/stdc++.h>
using namespace std;

static const int FULL_MASK = 2047; // since n <= 2048, values are in [0, 2047]

int n;
long long queries_used = 0;

int ask(int i, int j) {
    cout << "? " << i << " " << j << endl;
    cout.flush();
    int x;
    if (!(cin >> x)) exit(0);
    if (x == -1) exit(0);
    ++queries_used;
    return x;
}

void answer(const vector<int>& p) {
    cout << "!";
    for (int i = 1; i <= n; ++i) cout << " " << p[i];
    cout << endl;
    cout.flush();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;

    // If n is small enough, query all pairwise ORs (n*(n-1)/2) and solve offline.
    // Max pairs for n=92 is 4186 < 4269. For larger n, use the two-phase strategy.
    const int PAIRWISE_LIMIT_N = 92;

    vector<int> p(n + 1, 0);

    if (n <= PAIRWISE_LIMIT_N) {
        vector<vector<int>> ORv(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                int x = ask(i, j);
                ORv[i][j] = ORv[j][i] = x;
            }
        }

        // Find a pair (a, b) minimizing W_ij = AND over k != i, j of (OR[i][k] & OR[j][k]).
        int a = 1, b = 2;
        int bestW = FULL_MASK;
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                int w = FULL_MASK;
                for (int k = 1; k <= n; ++k) {
                    if (k == i || k == j) continue;
                    w &= ORv[i][k] & ORv[j][k];
                }
                if (w < bestW) {
                    bestW = w;
                    a = i; b = j;
                }
            }
        }

        // Compute p for indices != a, b
        for (int i = 1; i <= n; ++i) {
            if (i == a || i == b) continue;
            p[i] = ORv[i][a] & ORv[i][b];
        }

        // Compute p[a] and p[b]
        int pa = FULL_MASK, pb = FULL_MASK;
        for (int k = 1; k <= n; ++k) {
            if (k != a) pa &= ORv[a][k];
            if (k != b) pb &= ORv[b][k];
        }
        p[a] = pa;
        p[b] = pb;

        answer(p);
        return 0;
    }

    // For larger n, use sampling to find a pair (a, b) with p[a] & p[b] = 0,
    // then compute all p[i] using 2n - 3 queries.

    int t1 = 1, t2 = 2;
    int s = min(86, n - 2); // sample size to stay within total query limit

    vector<int> sample;
    sample.reserve(s);
    for (int i = 3; i <= n && (int)sample.size() < s; ++i) sample.push_back(i);

    vector<int> M(n + 1, 0);
    for (int idx : sample) {
        int x = ask(idx, t1);
        int y = ask(idx, t2);
        M[idx] = x & y;
    }

    // Find pair (a, b) in sample minimizing (M[i] & M[j])
    int a = sample[0], b = sample.size() >= 2 ? sample[1] : sample[0];
    int best = (M[a] & M[b]);
    for (size_t u = 0; u < sample.size(); ++u) {
        for (size_t v = u + 1; v < sample.size(); ++v) {
            int i = sample[u], j = sample[v];
            int val = M[i] & M[j];
            if (val < best) {
                best = val; a = i; b = j;
            }
        }
    }

    vector<int> X(n + 1, 0), Y(n + 1, 0);

    // Query ask(i, a) for all i != a
    for (int i = 1; i <= n; ++i) {
        if (i == a) continue;
        X[i] = ask(i, a);
    }
    // Query ask(i, b) for all i != b, reuse ask(a, b) from X[b]
    for (int i = 1; i <= n; ++i) {
        if (i == b) continue;
        if (i == a) {
            Y[i] = X[b]; // ask(a, b) == ask(b, a)
        } else {
            Y[i] = ask(i, b);
        }
    }

    // Compute p[i] for i != a, b
    for (int i = 1; i <= n; ++i) {
        if (i == a || i == b) continue;
        p[i] = X[i] & Y[i];
    }

    // Compute p[a] and p[b] using bitwise AND across all ask(a, i) / ask(b, i)
    int pa = FULL_MASK;
    for (int i = 1; i <= n; ++i) if (i != a) pa &= X[i];
    p[a] = pa;

    int pb = FULL_MASK;
    for (int i = 1; i <= n; ++i) if (i != b) pb &= Y[i];
    p[b] = pb;

    answer(p);
    return 0;
}
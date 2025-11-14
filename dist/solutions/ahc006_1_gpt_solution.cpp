#include <bits/stdc++.h>
using namespace std;

struct P {
    int x, y;
};

static inline long long mdist(const P& a, const P& b) {
    return (long long)abs(a.x - b.x) + (long long)abs(a.y - b.y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int N = 1000;
    const int M = 50;
    const P center{400, 400};
    vector<P> A(N), C(N);
    for (int i = 0; i < N; ++i) {
        int a, b, c, d;
        if (!(cin >> a >> b >> c >> d)) return 0;
        A[i] = {a, b};
        C[i] = {c, d};
    }

    vector<long long> w(N), s(N), ca(N), cc(N);
    for (int i = 0; i < N; ++i) {
        w[i] = mdist(A[i], C[i]);
        ca[i] = mdist(center, A[i]);
        cc[i] = mdist(C[i], center);
        s[i] = ca[i] + w[i] + cc[i];
    }

    vector<int> order;
    order.reserve(M);
    vector<char> used(N, 0);

    // Choose the first order as the one with minimal s[i]
    int first = int(min_element(s.begin(), s.end()) - s.begin());
    used[first] = 1;
    order.push_back(first);

    auto insertion_delta = [&](const vector<int>& seq, int cand, int pos) -> long long {
        long long rem = 0, add = 0;
        int m = (int)seq.size();
        if (m == 0) {
            // from center -> a[cand] -> c[cand] -> center
            rem = 0;
            add = mdist(center, A[cand]) + mdist(C[cand], center);
        } else if (pos == 0) {
            rem = mdist(center, A[seq[0]]);
            add = mdist(center, A[cand]) + mdist(C[cand], A[seq[0]]);
        } else if (pos == m) {
            rem = mdist(C[seq[m-1]], center);
            add = mdist(C[seq[m-1]], A[cand]) + mdist(C[cand], center);
        } else {
            rem = mdist(C[seq[pos-1]], A[seq[pos]]);
            add = mdist(C[seq[pos-1]], A[cand]) + mdist(C[cand], A[seq[pos]]);
        }
        return add - rem + w[cand];
    };

    // Build route by cheapest insertion heuristic considering all 1000 candidates
    while ((int)order.size() < M) {
        long long bestDelta = (1LL<<62);
        int bestCand = -1, bestPos = -1;
        int m = (int)order.size();
        for (int j = 0; j < N; ++j) if (!used[j]) {
            for (int pos = 0; pos <= m; ++pos) {
                long long delta = insertion_delta(order, j, pos);
                if (delta < bestDelta) {
                    bestDelta = delta;
                    bestCand = j;
                    bestPos = pos;
                }
            }
        }
        if (bestCand == -1) break;
        order.insert(order.begin() + bestPos, bestCand);
        used[bestCand] = 1;
    }

    // 2-opt improvement on the fixed set, optimizing bridging cost
    auto bridging_cost = [&](const vector<int>& seq) -> long long {
        int m = (int)seq.size();
        if (m == 0) return 0;
        long long cost = 0;
        cost += mdist(center, A[seq[0]]);
        for (int i = 0; i + 1 < m; ++i) {
            cost += mdist(C[seq[i]], A[seq[i+1]]);
        }
        cost += mdist(C[seq[m-1]], center);
        return cost;
    };

    // Sum of internal distances (constant for fixed set)
    long long internal_sum = 0;
    for (int i : order) internal_sum += w[i];

    // Perform 2-opt until no improvement
    {
        bool improved = true;
        long long bestB = bridging_cost(order);
        while (improved) {
            improved = false;
            int m = (int)order.size();
            for (int l = 0; l < m - 1; ++l) {
                for (int r = l + 1; r < m; ++r) {
                    vector<int> cand = order;
                    reverse(cand.begin() + l, cand.begin() + r + 1);
                    long long nb = bridging_cost(cand);
                    if (nb < bestB) {
                        order.swap(cand);
                        bestB = nb;
                        improved = true;
                        goto cont_outer;
                    }
                }
            }
            cont_outer:;
        }
    }

    // Build the route points
    vector<int> xs, ys;
    xs.reserve(2 * M + 2);
    ys.reserve(2 * M + 2);
    xs.push_back(400); ys.push_back(400);
    for (int idx : order) {
        xs.push_back(A[idx].x);
        ys.push_back(A[idx].y);
        xs.push_back(C[idx].x);
        ys.push_back(C[idx].y);
    }
    xs.push_back(400); ys.push_back(400);
    int npts = (int)xs.size();

    // Output
    cout << M;
    for (int idx : order) cout << ' ' << (idx + 1);
    cout << '\n';
    cout << npts;
    for (int i = 0; i < npts; ++i) {
        cout << ' ' << xs[i] << ' ' << ys[i];
    }
    cout << '\n';
    return 0;
}
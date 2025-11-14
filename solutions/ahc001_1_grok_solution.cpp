#include <bits/stdc++.h>
using namespace std;

double compute_p(long long r, long long s) {
    if (s == 0) return 0.0;
    double mn = min(r, s);
    double mx = max(r, s);
    double alpha = mn / mx;
    return 1.0 - (1.0 - alpha) * (1.0 - alpha);
}

int main() {
    int n;
    cin >> n;
    vector<long long> X(n), Y(n), R(n);
    for (int i = 0; i < n; i++) {
        cin >> X[i] >> Y[i] >> R[i];
    }
    vector<int> perm(n);
    for (int i = 0; i < n; i++) perm[i] = i;
    sort(perm.begin(), perm.end(), [&](int a, int b) {
        return R[a] > R[b];
    });
    vector<int> A(n), B(n), C(n), D(n);
    vector<vector<int>> assigned;
    for (int idx = 0; idx < n; idx++) {
        int i = perm[idx];
        // binary search max k
        int low = 0, high = 10000;
        while (low < high) {
            int mid = (low + high + 1) / 2;
            long long lx = X[i] - mid;
            long long rx = X[i] + 1 + mid;
            long long by = Y[i] - mid;
            long long ty = Y[i] + 1 + mid;
            if (lx < 0 || rx > 10000 || by < 0 || ty > 10000) {
                high = mid - 1;
                continue;
            }
            bool ok = true;
            // check assigned
            for (auto& rec : assigned) {
                long long al = rec[0], bl = rec[1], cl = rec[2], dl = rec[3];
                if (lx < cl && al < rx && by < dl && bl < ty) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                high = mid - 1;
                continue;
            }
            // check remaining unit cells
            for (int jdx = idx + 1; jdx < n; jdx++) {
                int j = perm[jdx];
                long long jlx = X[j], jrx = X[j] + 1;
                long long jby = Y[j], jty = Y[j] + 1;
                if (lx < jrx && jlx < rx && by < jty && jby < ty) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                low = mid;
            } else {
                high = mid - 1;
            }
        }
        int kmax = low;
        // find best k
        double best_p = -1.0;
        int best_k = 0;
        for (int k = 0; k <= kmax; k++) {
            long long side = 2LL * k + 1;
            long long s = side * side;
            double p = compute_p(R[i], s);
            if (p > best_p) {
                best_p = p;
                best_k = k;
            }
        }
        // set rect
        A[i] = X[i] - best_k;
        B[i] = Y[i] - best_k;
        C[i] = X[i] + 1 + best_k;
        D[i] = Y[i] + 1 + best_k;
        // add to assigned
        assigned.push_back({A[i], B[i], C[i], D[i]});
    }
    for (int i = 0; i < n; i++) {
        cout << A[i] << " " << B[i] << " " << C[i] << " " << D[i] << endl;
    }
    return 0;
}
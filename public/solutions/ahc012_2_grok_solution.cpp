#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N, K;
    cin >> N >> K;
    vector<long long> a(11);
    for (int d = 1; d <= 10; d++) cin >> a[d];
    vector<long long> X(N), Y(N);
    for (int i = 0; i < N; i++) {
        cin >> X[i] >> Y[i];
    }
    vector<double> fact(11, 1);
    for (int i = 1; i <= 10; i++) fact[i] = fact[i - 1] * i;
    long long best_l = 0;
    double best_exp = 0;
    for (long long ll = 0; ll <= 100; ll++) {
        long long rr = 1 + ll + ll * (ll - 1) / 2;
        double lam = double(N) / rr;
        double s = 0;
        if (ll == 0) {
            if (1 <= N && N <= 10) s = min(double(a[N]), 1.0);
        } else {
            for (int d = 1; d <= 10; d++) {
                double pd = pow(lam, d) / fact[d] * exp(-lam);
                double bd = rr * pd;
                s += min(double(a[d]), bd);
            }
        }
        if (s > best_exp) {
            best_exp = s;
            best_l = ll;
        }
    }
    vector<tuple<long long, long long, long long, long long>> lines;
    for (long long kk = 1; kk <= best_l; kk++) {
        bool found = false;
        for (long long offset = 0; offset <= 1000; offset++) {
            long long px = kk;
            long long py = 0;
            long long qx = 0;
            long long qy = kk * kk + offset;
            long long dx = qx - px;
            long long dy = qy - py;
            bool safe = true;
            for (int j = 0; j < N; j++) {
                long long vx = X[j] - px;
                long long vy = Y[j] - py;
                if (dx * vy == dy * vx) {
                    safe = false;
                    break;
                }
            }
            if (safe) {
                lines.emplace_back(px, py, qx, qy);
                found = true;
                break;
            }
        }
        if (!found) {
            for (long long offset = -1; offset >= -1000; offset--) {
                long long px = kk;
                long long py = 0;
                long long qx = 0;
                long long qy = kk * kk + offset;
                if (qy < -1000000000LL || qy > 1000000000LL) continue;
                long long dx = qx - px;
                long long dy = qy - py;
                bool safe = true;
                for (int j = 0; j < N; j++) {
                    long long vx = X[j] - px;
                    long long vy = Y[j] - py;
                    if (dx * vy == dy * vx) {
                        safe = false;
                        break;
                    }
                }
                if (safe) {
                    lines.emplace_back(px, py, qx, qy);
                    found = true;
                    break;
                }
            }
        }
    }
    cout << lines.size() << endl;
    for (auto [px, py, qx, qy] : lines) {
        cout << px << " " << py << " " << qx << " " << qy << endl;
    }
    return 0;
}
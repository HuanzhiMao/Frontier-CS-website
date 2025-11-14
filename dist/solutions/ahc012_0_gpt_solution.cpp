#include <bits/stdc++.h>
using namespace std;

static vector<int> pickConstants(const unordered_set<int>& forbids, int M, int bound) {
    vector<int> res;
    res.reserve(M);
    unordered_set<int> used;
    used.reserve(M * 2 + 10);
    if (M == 0) return res;
    double step = (2.0 * bound) / (M + 1);
    for (int t = 1; t <= M; ++t) {
        double target = -bound + t * step;
        int c0 = (int)llround(target);
        c0 = max(-bound, min(bound, c0));
        if (!forbids.count(c0) && !used.count(c0)) {
            res.push_back(c0);
            used.insert(c0);
            continue;
        }
        bool found = false;
        for (int d = 1; d <= 2 * bound; ++d) {
            int c1 = c0 + d;
            if (c1 <= bound && !forbids.count(c1) && !used.count(c1)) {
                res.push_back(c1);
                used.insert(c1);
                found = true;
                break;
            }
            int c2 = c0 - d;
            if (c2 >= -bound && !forbids.count(c2) && !used.count(c2)) {
                res.push_back(c2);
                used.insert(c2);
                found = true;
                break;
            }
        }
        if (!found) {
            // Fallback: pick any available value within bounds
            for (int c = -bound; c <= bound; ++c) {
                if (!forbids.count(c) && !used.count(c)) {
                    res.push_back(c);
                    used.insert(c);
                    found = true;
                    break;
                }
            }
            if (!found) {
                // Should not happen with given constraints; push a duplicate as last resort
                res.push_back(c0);
            }
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K;
    if (!(cin >> N >> K)) {
        return 0;
    }
    vector<int> a(11);
    for (int i = 1; i <= 10; ++i) cin >> a[i];
    vector<pair<int,int>> pts(N);
    for (int i = 0; i < N; ++i) cin >> pts[i].first >> pts[i].second;

    unordered_set<int> setD1, setD2;
    setD1.reserve(N * 2 + 10);
    setD2.reserve(N * 2 + 10);
    for (auto &p : pts) {
        setD1.insert(p.second - p.first);
        setD2.insert(p.second + p.first);
    }

    auto expectedScore = [&](int m1, int m2)->double {
        long long C = 1LL * (m1 + 1) * (m2 + 1);
        if (C <= 0) C = 1;
        double lambda = (double)N / (double)C;
        double sc = 0.0;
        if (lambda < 700.0) {
            double p = exp(-lambda);
            double cur = p;
            for (int d = 1; d <= 10; ++d) {
                cur *= lambda / (double)d;
                double expectedCells = (double)C * cur;
                sc += min<double>(a[d], expectedCells);
            }
        } else {
            // For very large lambda, probabilities for small d are negligible
            // expectedCells ~ 0 for d <= 10
            // sc remains 0
        }
        return sc;
    };

    int best_m1 = K / 2, best_m2 = K - best_m1;
    double best_sc = expectedScore(best_m1, best_m2);
    for (int m1 = 0; m1 <= K; ++m1) {
        for (int m2 = 0; m2 <= K - m1; ++m2) {
            double sc = expectedScore(m1, m2);
            if (sc > best_sc) {
                best_sc = sc;
                best_m1 = m1;
                best_m2 = m2;
            }
        }
    }

    const int R = 10000;
    int cmaxRaw = (int)floor(sqrt(2.0) * R) - 1; // ~14141
    if (cmaxRaw < 1) cmaxRaw = 1;
    int slack = 50;
    int bound = max(1, cmaxRaw - slack);

    vector<int> Cs1 = pickConstants(setD1, best_m1, bound);
    vector<int> Cs2 = pickConstants(setD2, best_m2, bound);

    const long long B = 1000000LL;
    vector<array<long long,4>> lines;
    lines.reserve(Cs1.size() + Cs2.size());
    for (int c : Cs1) {
        long long px = -B, py = -B + c;
        long long qx = B, qy = B + c;
        lines.push_back({px, py, qx, qy});
    }
    for (int c : Cs2) {
        long long px = -B, py = (long long)c + B;
        long long qx = B,  qy = (long long)c - B;
        lines.push_back({px, py, qx, qy});
    }

    cout << (int)lines.size() << '\n';
    for (auto &ln : lines) {
        cout << ln[0] << ' ' << ln[1] << ' ' << ln[2] << ' ' << ln[3] << '\n';
    }
    return 0;
}
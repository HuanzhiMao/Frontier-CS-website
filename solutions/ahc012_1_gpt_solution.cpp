#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) return 0;
    vector<int> a(11);
    for (int d = 1; d <= 10; ++d) cin >> a[d];
    vector<int> xs(N), ys(N);
    unordered_set<int> Xset, Yset;
    Xset.reserve(N * 2);
    Yset.reserve(N * 2);
    for (int i = 0; i < N; ++i) {
        int xi, yi;
        cin >> xi >> yi;
        xs[i] = xi;
        ys[i] = yi;
        Xset.insert(xi);
        Yset.insert(yi);
    }

    auto chooseBestVH = [&](int K, int N, const vector<int>& a) -> pair<int,int> {
        double bestScore = -1.0;
        int bestV = 0, bestH = 0;
        for (int V = 0; V <= K; ++V) {
            for (int H = 0; H <= K - V; ++H) {
                long long M = 1LL * (V + 1) * (H + 1);
                if (M <= 0) continue;
                double lambda = (double)N / (double)M;
                double e = exp(-lambda);
                double p_prev = e; // p0
                double expectedSum = 0.0;
                for (int d = 1; d <= 10; ++d) {
                    double p_d = p_prev * lambda / d;
                    double bexp = (double)M * p_d;
                    expectedSum += min((double)a[d], bexp);
                    p_prev = p_d;
                }
                if (expectedSum > bestScore) {
                    bestScore = expectedSum;
                    bestV = V;
                    bestH = H;
                }
            }
        }
        return {bestV, bestH};
    };

    // Determine V and H (number of vertical and horizontal cuts)
    auto [V, H] = chooseBestVH(K, N, a);

    const int R = 9999; // within the cake radius
    auto choosePositions = [&](int count, const unordered_set<int>& forbidSet, int R)->vector<int> {
        vector<int> res;
        res.reserve(count);
        unordered_set<int> used;
        used.reserve(count * 2 + 10);
        for (int j = 1; j <= count; ++j) {
            double pos = -R + ((2.0 * R + 1.0) * j / (count + 1));
            long long c0 = llround(pos);
            if (c0 < -R) c0 = -R;
            if (c0 > R) c0 = R;
            int base = (int)c0;
            int chosen = 0;
            bool found = false;
            for (int d = 0; d <= 2 * R; ++d) {
                int cand1 = base + d;
                if (cand1 >= -R && cand1 <= R && !forbidSet.count(cand1) && !used.count(cand1)) {
                    chosen = cand1; found = true; break;
                }
                if (d != 0) {
                    int cand2 = base - d;
                    if (cand2 >= -R && cand2 <= R && !forbidSet.count(cand2) && !used.count(cand2)) {
                        chosen = cand2; found = true; break;
                    }
                }
            }
            if (!found) {
                // Fallback outside the cake (still a valid line but won't cut the cake)
                chosen = 1000000000 - j; // within bounds
            }
            res.push_back(chosen);
            used.insert(chosen);
        }
        return res;
    };

    vector<int> vcuts = choosePositions(V, Xset, R);
    vector<int> hcuts = choosePositions(H, Yset, R);

    vector<array<long long,4>> lines;
    lines.reserve(vcuts.size() + hcuts.size());
    const long long LIM = 1000000000LL;

    for (int c : vcuts) {
        lines.push_back({(long long)c, -LIM, (long long)c, LIM});
    }
    for (int c : hcuts) {
        lines.push_back({-LIM, (long long)c, LIM, (long long)c});
    }

    cout << lines.size() << "\n";
    for (auto &ln : lines) {
        cout << ln[0] << " " << ln[1] << " " << ln[2] << " " << ln[3] << "\n";
    }
    return 0;
}
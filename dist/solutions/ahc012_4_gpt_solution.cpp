#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, K;
    if (!(cin >> N >> K)) {
        return 0;
    }
    vector<int> a(11);
    for (int i = 1; i <= 10; ++i) cin >> a[i];
    vector<int> xs(N), ys(N);
    for (int i = 0; i < N; ++i) cin >> xs[i] >> ys[i];

    // Build unique sorted coordinates and sets for quick lookup
    vector<int> uniqX = xs, uniqY = ys;
    sort(uniqX.begin(), uniqX.end());
    uniqX.erase(unique(uniqX.begin(), uniqX.end()), uniqX.end());
    sort(uniqY.begin(), uniqY.end());
    uniqY.erase(unique(uniqY.begin(), uniqY.end()), uniqY.end());

    unordered_set<int> usedX(uniqX.begin(), uniqX.end());
    unordered_set<int> usedY(uniqY.begin(), uniqY.end());

    auto chooseCuts = [](const vector<int>& uniq, int mWanted,
                         const unordered_set<int>& used, int LBound, int RBound) -> vector<int> {
        vector<int> cuts;
        cuts.reserve(mWanted);
        unordered_set<int> chosen;
        int U = (int)uniq.size();
        if (U <= 1 || mWanted <= 0) return cuts;
        // mWanted <= U-1 ideally
        mWanted = min(mWanted, U - 1);
        for (int j = 1; j <= mWanted; ++j) {
            // pick an interval index u in [1, U-1]
            long long u = (long long)U * j / (mWanted + 1);
            if (u < 1) u = 1;
            if (u >= U) u = U - 1;
            int left = uniq[u - 1];
            int right = uniq[u];
            // Prefer a point strictly between left and right if possible
            int c = 0;
            bool found = false;
            if (right - left >= 2) {
                // choose middle
                c = left + (right - left) / 2;
                // ensure bounds and uniqueness
                if (c < LBound) c = LBound;
                if (c > RBound) c = RBound;
                if (!used.count(c) && !chosen.count(c)) {
                    cuts.push_back(c);
                    chosen.insert(c);
                    found = true;
                }
            }
            if (!found) {
                // No integer strictly between left and right, or middle was blocked.
                // Search outward from left/right neighbors
                int baseL = left, baseR = right;
                for (int rad = 1; rad <= 20000; ++rad) {
                    long long cand1 = (long long)baseL - rad;
                    long long cand2 = (long long)baseR + rad;
                    if (cand1 >= LBound && cand1 <= RBound && !used.count((int)cand1) && !chosen.count((int)cand1)) {
                        cuts.push_back((int)cand1);
                        chosen.insert((int)cand1);
                        found = true;
                        break;
                    }
                    if (cand2 >= LBound && cand2 <= RBound && !used.count((int)cand2) && !chosen.count((int)cand2)) {
                        cuts.push_back((int)cand2);
                        chosen.insert((int)cand2);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    // As a last resort, pick any available integer within bounds not in used or chosen
                    for (int cand = LBound; cand <= RBound; ++cand) {
                        if (!used.count(cand) && !chosen.count(cand)) {
                            cuts.push_back(cand);
                            chosen.insert(cand);
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (!found) break;
        }
        sort(cuts.begin(), cuts.end());
        return cuts;
    };

    // Decide number of vertical and horizontal cuts
    int maxV = max(0, (int)uniqX.size() - 1);
    int maxH = max(0, (int)uniqY.size() - 1);
    int v = min(K / 2, maxV);
    int h = min(K - v, maxH);
    // If we still have room and available unique intervals, add more lines
    while (v + h < K && (v < maxV || h < maxH)) {
        if (v < maxV) ++v;
        else if (h < maxH) ++h;
        else break;
    }

    // Generate cuts within [-9999, 9999] to ensure intersection with circle
    const int LBound = -9999, RBound = 9999;
    vector<int> cutX = chooseCuts(uniqX, v, usedX, LBound, RBound);
    vector<int> cutY = chooseCuts(uniqY, h, usedY, LBound, RBound);

    // Output
    int k = (int)cutX.size() + (int)cutY.size();
    cout << k << '\n';
    const int LIM = 1000000000;
    for (int c : cutX) {
        cout << c << " " << -LIM << " " << c << " " << LIM << '\n';
    }
    for (int c : cutY) {
        cout << -LIM << " " << c << " " << LIM << " " << c << '\n';
    }
    return 0;
}
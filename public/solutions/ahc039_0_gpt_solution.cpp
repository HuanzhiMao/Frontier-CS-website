#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if(!(cin >> N)) return 0;
    vector<pair<int,int>> mac(N), sar(N);
    for (int i = 0; i < N; ++i) cin >> mac[i].first >> mac[i].second;
    for (int i = 0; i < N; ++i) cin >> sar[i].first >> sar[i].second;

    const int K = 100; // grid divisions per axis; 100000 % 100 == 0
    const int STEP = 100000 / K; // 1000

    // diff[xcell][ycell]
    vector<vector<int>> diff(K, vector<int>(K, 0));
    for (int i = 0; i < N; ++i) {
        int xi = mac[i].first, yi = mac[i].second;
        int cx = min(xi / STEP, K - 1);
        int cy = min(yi / STEP, K - 1);
        diff[cx][cy] += 1;
    }
    for (int i = 0; i < N; ++i) {
        int xi = sar[i].first, yi = sar[i].second;
        int cx = min(xi / STEP, K - 1);
        int cy = min(yi / STEP, K - 1);
        diff[cx][cy] -= 1;
    }

    // Max sub-rectangle on grid using Kadane-like O(K^3)
    int bestSum = INT_MIN;
    int bestL = 0, bestR = 0, bestB = 0, bestT = 0;
    vector<int> temp(K);
    for (int L = 0; L < K; ++L) {
        fill(temp.begin(), temp.end(), 0);
        for (int R = L; R < K; ++R) {
            for (int y = 0; y < K; ++y) temp[y] += diff[R][y];
            // Kadane over y
            int cur = 0, start = 0;
            for (int y = 0; y < K; ++y) {
                cur += temp[y];
                if (cur > bestSum) {
                    bestSum = cur;
                    bestL = L; bestR = R; bestB = start; bestT = y;
                }
                if (cur < 0) {
                    cur = 0;
                    start = y + 1;
                }
            }
        }
    }

    auto x_low = [&](int cx){ return cx * STEP; };
    auto x_high = [&](int cx){ return (cx == K-1) ? 100000 : (cx + 1) * STEP - 1; };
    auto y_low = [&](int cy){ return cy * STEP; };
    auto y_high = [&](int cy){ return (cy == K-1) ? 100000 : (cy + 1) * STEP - 1; };

    int x1, y1, x2, y2;
    if (bestSum <= 0) {
        // Fallback: whole area rectangle yields score 1 safely
        x1 = 0; y1 = 0; x2 = 100000; y2 = 100000;
    } else {
        x1 = x_low(bestL);
        x2 = x_high(bestR);
        y1 = y_low(bestB);
        y2 = y_high(bestT);
        // Ensure positive area; if somehow degenerate (shouldn't happen), fallback
        if (x2 <= x1 || y2 <= y1) {
            x1 = 0; y1 = 0; x2 = 100000; y2 = 100000;
        }
        // Ensure perimeter constraint (it always holds with coordinates in [0,1e5])
        long long per = 2LL * ((x2 - x1) + (y2 - y1));
        if (per > 400000LL) {
            x1 = 0; y1 = 0; x2 = 100000; y2 = 100000;
        }
    }

    cout << 4 << "\n";
    cout << x1 << " " << y1 << "\n";
    cout << x2 << " " << y1 << "\n";
    cout << x2 << " " << y2 << "\n";
    cout << x1 << " " << y2 << "\n";
    return 0;
}
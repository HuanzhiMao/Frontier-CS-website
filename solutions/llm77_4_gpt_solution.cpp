#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    while (cin >> n >> m) {
        vector<long double> wP(n, 1.0L); // weights for participants
        const int K = 6; // meta experts
        vector<long double> wM(K, 1.0L);
        const long double lambda = 0.75L; // penalty factor for wrong predictions
        bool hasPrev = false;
        int prevT = 0;

        for (int round = 0; round < m; ++round) {
            string s;
            if (!(cin >> s)) return 0;

            // Compute weighted votes
            long double sum1 = 0.0L, sum0 = 0.0L;
            int onesCount = 0;
            for (int i = 0; i < n; ++i) {
                if (s[i] == '1') {
                    sum1 += wP[i];
                    ++onesCount;
                } else {
                    sum0 += wP[i];
                }
            }

            bool metaPred[K];
            metaPred[0] = false; // always 0
            metaPred[1] = true;  // always 1
            bool maj = (onesCount * 2 >= n);
            metaPred[2] = maj;        // majority of participants' predictions
            metaPred[3] = !maj;       // minority
            if (hasPrev) {
                metaPred[4] = (bool)prevT;      // previous outcome
                metaPred[5] = !((bool)prevT);   // opposite of previous outcome
            } else {
                metaPred[4] = false; // placeholders; will not be added if !hasPrev
                metaPred[5] = true;
            }

            // Add meta experts' contributions
            sum0 += wM[0];
            sum1 += wM[1];
            sum1 += metaPred[2] ? wM[2] : 0.0L;
            sum0 += !metaPred[2] ? wM[2] : 0.0L;
            sum1 += metaPred[3] ? wM[3] : 0.0L;
            sum0 += !metaPred[3] ? wM[3] : 0.0L;
            if (hasPrev) {
                sum1 += metaPred[4] ? wM[4] : 0.0L;
                sum0 += !metaPred[4] ? wM[4] : 0.0L;
                sum1 += metaPred[5] ? wM[5] : 0.0L;
                sum0 += !metaPred[5] ? wM[5] : 0.0L;
            }

            char guess = (sum1 >= sum0) ? '1' : '0';
            cout << guess << '\n' << flush;

            string tStr;
            if (!(cin >> tStr)) return 0;
            int t = (tStr[0] == '1') ? 1 : 0;
            char tCh = t ? '1' : '0';

            // Update participants' weights
            for (int i = 0; i < n; ++i) {
                if (s[i] != tCh) wP[i] *= lambda;
            }

            // Update meta experts' weights
            if (t == 1) wM[0] *= lambda; // always 0 wrong
            if (t == 0) wM[1] *= lambda; // always 1 wrong
            if (metaPred[2] != (bool)t) wM[2] *= lambda; // majority wrong
            if (metaPred[3] != (bool)t) wM[3] *= lambda; // minority wrong
            if (hasPrev) {
                if (metaPred[4] != (bool)t) wM[4] *= lambda; // prev outcome wrong
                if (metaPred[5] != (bool)t) wM[5] *= lambda; // opposite of prev wrong
            } else {
                hasPrev = true;
            }
            prevT = t;

            // Normalize to avoid underflow
            long double maxw = 0.0L;
            for (int i = 0; i < n; ++i) if (wP[i] > maxw) maxw = wP[i];
            for (int j = 0; j < K; ++j) if (wM[j] > maxw) maxw = wM[j];
            if (maxw < 1e-200L) {
                if (maxw > 0.0L) {
                    long double inv = 1.0L / maxw;
                    for (int i = 0; i < n; ++i) wP[i] *= inv;
                    for (int j = 0; j < K; ++j) wM[j] *= inv;
                } else {
                    // Extreme underflow: reset
                    for (int i = 0; i < n; ++i) wP[i] = 1.0L;
                    for (int j = 0; j < K; ++j) wM[j] = 1.0L;
                }
            }
        }
    }
    return 0;
}
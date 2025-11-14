#include <bits/stdc++.h>
using namespace std;

static inline int ceil_log_base(double n, double base) {
    return (int)ceil(log(n) / log(base));
}

// States:
// 0: no bits yet
// 1: last = 0 (one bit)
// 2: last = 1 (one bit)
// 3: last2 = "00"
// 4: last2 = "11"
// 5: last2 = "01"
// 6: last2 = "10"
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;

        int Qlimit = 2 * ceil_log_base((double)n, 1.116);
        vector<int> state(n + 1, 0);
        vector<char> alive(n + 1, 1);
        int aliveCount = n;

        auto ask = [&](int l, int r)->int{
            cout << "? " << l << " " << r << "\n";
            cout.flush();
            int x; 
            if (!(cin >> x)) x = r - l; // fallback
            int len = r - l + 1;
            int y = len - x; // y in {0,1}
            return y;
        };

        auto update_all = [&](int l, int r, int y){
            for (int i = 1; i <= n; ++i) if (alive[i]) {
                int in = (l <= i && i <= r) ? 1 : 0;
                int z = y ^ in;
                switch (state[i]) {
                    case 0:
                        state[i] = (z == 0 ? 1 : 2);
                        break;
                    case 1: // last 0
                        if (z == 0) state[i] = 3; // 00
                        else state[i] = 5;        // 01
                        break;
                    case 2: // last 1
                        if (z == 1) state[i] = 4; // 11
                        else state[i] = 6;        // 10
                        break;
                    case 3: // 00
                        if (z == 0) { alive[i] = 0; --aliveCount; }
                        else state[i] = 5; // 01
                        break;
                    case 4: // 11
                        if (z == 1) { alive[i] = 0; --aliveCount; }
                        else state[i] = 6; // 10
                        break;
                    case 5: // 01
                        if (z == 1) state[i] = 4; // 11
                        else state[i] = 6;        // 10
                        break;
                    case 6: // 10
                        if (z == 0) state[i] = 3; // 00
                        else state[i] = 5;        // 01
                        break;
                }
            }
        };

        auto choose_prefix = [&]()->pair<int,int>{
            // Compute prefix sums for groups among alive
            vector<int> p0(n + 1, 0), p1(n + 1, 0), p01(n + 1, 0), p10(n + 1, 0);
            int TOT0 = 0, TOT1 = 0, TOT01 = 0, TOT10 = 0;
            for (int i = 1; i <= n; ++i) {
                p0[i] = p0[i - 1];
                p1[i] = p1[i - 1];
                p01[i] = p01[i - 1];
                p10[i] = p10[i - 1];
                if (!alive[i]) continue;
                if (state[i] == 3) { p0[i]++; TOT0++; }
                else if (state[i] == 4) { p1[i]++; TOT1++; }
                else if (state[i] == 5) { p01[i]++; TOT01++; }
                else if (state[i] == 6) { p10[i]++; TOT10++; }
            }
            int TOTmix = TOT01 + TOT10;

            int best_m = 1;
            long long best_worst = (1LL<<60);

            if (TOT0 + TOT1 > 0) {
                for (int m = 1; m <= n; ++m) {
                    int a_in = p0[m];
                    int b_in = p1[m];
                    int s1 = (TOT0 - a_in) + b_in;        // y=1
                    int s0 = a_in + (TOT1 - b_in);        // y=0
                    int worst = max(s1, s0);
                    long long total_worst = (long long)TOTmix + worst;
                    if (total_worst < best_worst) {
                        best_worst = total_worst;
                        best_m = m;
                    }
                }
            } else {
                // No eliminations this step; try to maximize conversion from mix to P0/P1 in worst-case y
                for (int m = 1; m <= n; ++m) {
                    int c01_in = p01[m];
                    int c10_in = p10[m];
                    int conv1 = TOT01 + (c10_in - c01_in); // y=1
                    int conv0 = TOT10 + (c01_in - c10_in); // y=0
                    int minConv = min(conv1, conv0);
                    // We choose m maximizing minConv; tie-breaker choose smaller m
                    if ((long long)minConv > best_worst) {
                        best_worst = minConv;
                        best_m = m;
                    }
                }
            }
            return {1, best_m};
        };

        int queriesUsed = 0;

        while (aliveCount > 2 && queriesUsed < Qlimit) {
            auto seg = choose_prefix();
            int l = seg.first, r = seg.second;
            int y = ask(l, r);
            update_all(l, r, y);
            queriesUsed++;
        }

        // Choose up to two candidates
        vector<int> candidates;
        for (int i = 1; i <= n && (int)candidates.size() < 2; ++i) {
            if (alive[i]) candidates.push_back(i);
        }
        if (candidates.empty()) {
            // Fallback: just guess 1,2
            candidates.push_back(1);
            candidates.push_back(2);
        } else if (candidates.size() == 1) {
            // add some other
            int add = (candidates[0] == 1 ? 2 : 1);
            candidates.push_back(add);
        }

        // First guess
        cout << "! " << candidates[0] << "\n";
        cout.flush();
        int res1; 
        if (!(cin >> res1)) res1 = 0;
        if (res1 == 0) {
            // Second guess
            cout << "! " << candidates[1] << "\n";
            cout.flush();
            int res2; 
            if (!(cin >> res2)) res2 = 0;
            (void)res2;
        }
        cout << "#\n";
        cout.flush();
    }
    return 0;
}
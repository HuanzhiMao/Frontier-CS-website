#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<string> s(m);
    for (int i = 0; i < m; i++) {
        cin >> s[i];
    }
    double prob = 0.0;
    int M = 1 << m;
    for (int mask = 1; mask < M; mask++) {
        int pop = __builtin_popcount(mask);
        int sign = (pop % 2 == 1 ? 1 : -1);
        bool conf = false;
        int free_cnt = 0;
        for (int j = 0; j < n; j++) {
            char req = 0;
            for (int i = 0; i < m; i++) {
                if (mask & (1 << i)) {
                    char c = s[i][j];
                    if (c != '?') {
                        if (req == 0) {
                            req = c;
                        } else if (req != c) {
                            conf = true;
                            break;
                        }
                    }
                }
            }
            if (conf) break;
            if (req == 0) free_cnt++;
        }
        if (conf) continue;
        double contrib = (double)sign * pow(4.0, (double)free_cnt);
        prob += contrib;
    }
    prob /= pow(4.0, (double)n);
    cout << fixed << setprecision(15) << prob << endl;
    return 0;
}
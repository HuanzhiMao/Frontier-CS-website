#include <bits/stdc++.h>

using namespace std;

int main() {
    string line;
    if (!getline(cin, line)) return 0;
    istringstream iss(line);
    int G;
    iss >> G;
    static long long used_t = 0;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        istringstream iss2(line);
        string token;
        iss2 >> token;
        if (token == "-1") return 0;
        if (token == "STATE") {
            int h, r, a, b, P, k;
            iss2 >> h >> r >> a >> b >> P >> k;
            // read ALICE line
            getline(cin, line);
            istringstream iss_al(line);
            string al;
            iss_al >> al;
            int suit1, val1, suit2, val2;
            iss_al >> suit1 >> val1 >> suit2 >> val2;
            // read BOARD line
            getline(cin, line);
            istringstream iss_bd(line);
            string bo;
            iss_bd >> bo;
            vector<pair<int, int>> board(k);
            for (int i = 0; i < k; i++) {
                iss_bd >> board[i].first >> board[i].second;
            }
            // now process
            double e = 0.5;
            int base_t_arr[5] = {0, 10, 30, 60, 200};
            int t = base_t_arr[r];
            t = min(t, (int)(3000000LL - used_t));
            bool queried = false;
            if (t > 0) {
                cout << "RATE " << t << endl;
                string rline;
                getline(cin, rline);
                istringstream iss_r(rline);
                string rt;
                iss_r >> rt;
                if (rt == "RATES") {
                    double w, d;
                    iss_r >> w >> d;
                    e = w + 0.5 * d;
                    queried = true;
                    used_t += t;
                } else if (rt == "-1") {
                    return 0;
                }
            }
            int x = 0;
            if (r == 1) {
                bool is_pair = val1 == val2;
                bool suited = suit1 == suit2;
                int minv = min(val1, val2);
                int maxv = max(val1, val2);
                if (is_pair) {
                    x = 5 + (val1 - 1) / 2;
                } else if (suited && maxv >= 9) {
                    x = 4;
                } else if (maxv >= 10 && minv >= 8) {
                    x = 3;
                }
            } else {
                double thresh = 0.55 + 0.05 * (r - 2);
                if (e > thresh) {
                    int mult = r;
                    x = P * mult / 4 + 1;
                    x = min(x, a);
                    x = max(x, 1);
                }
            }
            if (x > 0) {
                cout << "ACTION RAISE " << x << endl;
            } else {
                cout << "ACTION CHECK" << endl;
            }
        }
    }
    return 0;
}
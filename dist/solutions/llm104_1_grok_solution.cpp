#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    for (int test = 0; test < t; ++test) {
        int n;
        cin >> n;
        vector<int> poss;
        for (int i = 1; i <= n; ++i) {
            poss.push_back(i);
        }
        vector<pair<char, int>> states(n + 1, {' ', 0});
        int query_count = 0;
        while (poss.size() > 2) {
            int curr_size = poss.size();
            vector<vector<int>> cum(5, vector<int>(curr_size + 1, 0));
            for (int i = 0; i < curr_size; ++i) {
                int kk = poss[i];
                int strk = states[kk].second;
                char ls = states[kk].first;
                int s;
                if (strk == 0) s = 0;
                else if (ls == 'H') s = (strk == 1) ? 1 : 2;
                else s = (strk == 1) ? 3 : 4;
                for (int tt = 0; tt < 5; ++tt) {
                    cum[tt][i + 1] = cum[tt][i];
                }
                cum[s][i + 1]++;
            }
            int min_worst = INT_MAX;
            int best_j = -1;
            for (int jj = 1; jj < curr_size; ++jj) {
                auto calc_rem = [&](bool bb) -> int {
                    char ns_left = (bb) ? 'H' : 'D';
                    char ns_right = (!bb) ? 'H' : 'D';
                    int elim_l = (ns_left == 'H') ? cum[2][jj] : cum[4][jj];
                    int surv_l = jj - elim_l;
                    int total2 = cum[2][curr_size] - cum[2][jj];
                    int total4 = cum[4][curr_size] - cum[4][jj];
                    int elim_r = (ns_right == 'H') ? total2 : total4;
                    int surv_r = (curr_size - jj) - elim_r;
                    return surv_l + surv_r;
                };
                int rem0 = calc_rem(false);
                int rem1 = calc_rem(true);
                int worst = max(rem0, rem1);
                if (worst < min_worst) {
                    min_worst = worst;
                    best_j = jj;
                }
            }
            if (best_j == -1) {
                // Should not happen
                break;
            }
            int l = poss[0];
            int r = poss[best_j - 1];
            cout << "? " << l << " " << r << endl;
            cout.flush();
            int x;
            cin >> x;
            int S = r - l + 1;
            bool b = (x == S - 1);
            vector<int> new_possible;
            for (int i = 0; i < curr_size; ++i) {
                int kk = poss[i];
                bool in_range = (l <= kk && kk <= r);
                char new_s = (b == in_range) ? 'H' : 'D';
                int old_strk = states[kk].second;
                char old_last = states[kk].first;
                if (old_strk == 0) {
                    new_possible.push_back(kk);
                    states[kk] = {new_s, 1};
                } else {
                    int new_strk;
                    bool valid = true;
                    if (new_s == old_last) {
                        new_strk = old_strk + 1;
                        if (new_strk >= 3) valid = false;
                    } else {
                        new_strk = 1;
                    }
                    if (valid) {
                        new_possible.push_back(kk);
                        states[kk] = {new_s, new_strk};
                    }
                }
            }
            poss = new_possible;
            query_count++;
        }
        // Now poss.size() <= 2
        if (poss.size() == 1) {
            cout << "! " << poss[0] << endl;
            cout.flush();
            int y;
            cin >> y;
        } else if (poss.size() == 2) {
            cout << "! " << poss[0] << endl;
            cout.flush();
            int y;
            cin >> y;
            if (y == 0) {
                cout << "! " << poss[1] << endl;
                cout.flush();
                int yy;
                cin >> yy;
            }
        }
        cout << "#" << endl;
        cout.flush();
    }
    return 0;
}
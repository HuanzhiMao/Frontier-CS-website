#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

void solve() {
    int n;
    cin >> n;

    vector<int> candidates(n);
    iota(candidates.begin(), candidates.end(), 1);

    vector<pair<char, char>> history(n + 1, {'N', 'N'});

    while (candidates.size() > 2) {
        vector<int> s_hh;
        vector<int> s_dd;
        for (int can : candidates) {
            if (history[can].first == 'H' && history[can].second == 'H') {
                s_hh.push_back(can);
            } else if (history[can].first == 'D' && history[can].second == 'D') {
                s_dd.push_back(can);
            }
        }

        int l, r;
        if (s_hh.empty() && s_dd.empty()) {
            l = candidates.front();
            if (candidates.size() > 1) {
                r = candidates[candidates.size() / 2 - 1];
            } else {
                r = candidates.front();
            }
        } else {
            vector<int> w(n + 1, 0);
            for (int can : s_hh) {
                w[can] = 1;
            }
            for (int can : s_dd) {
                w[can] = -1;
            }

            vector<long long> p(n + 1, 0);
            for (int i = 1; i <= n; ++i) {
                p[i] = p[i - 1] + w[i];
            }

            long long p_max = -3e18; 
            long long p_min = 3e18;
            int idx_max = 0, idx_min = 0;
            
            for (int i = 0; i <= n; ++i) {
                if (p[i] > p_max) {
                    p_max = p[i];
                    idx_max = i;
                }
                if (p[i] < p_min) {
                    p_min = p[i];
                    idx_min = i;
                }
            }

            l = min(idx_min, idx_max) + 1;
            r = max(idx_min, idx_max);
            
            if (l > r) {
                 l = candidates.front();
                 if (candidates.size() > 1) {
                    r = candidates[candidates.size() / 2 - 1];
                 } else {
                    r = candidates.front();
                 }
            }
        }
        
        cout << "? " << l << " " << r << endl;
        int x;
        cin >> x;

        int len = r - l + 1;
        char h_in, h_out;
        if (x == len - 1) {
            h_in = 'H';
            h_out = 'D';
        } else { // x == len
            h_in = 'D';
            h_out = 'H';
        }

        vector<int> next_candidates;
        for (int can : candidates) {
            bool is_in = (can >= l && can <= r);
            char h_new = is_in ? h_in : h_out;

            if (history[can].first == history[can].second && history[can].second == h_new && history[can].first != 'N') {
                // Eliminated
            } else {
                next_candidates.push_back(can);
                history[can].first = history[can].second;
                history[can].second = h_new;
            }
        }
        candidates = next_candidates;
    }

    for (int can : candidates) {
        cout << "! " << can << endl;
        int response;
        cin >> response;
        if (response == 1) {
            break;
        }
    }

    cout << "#" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}
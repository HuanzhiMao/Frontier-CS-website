#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    int n;
    ll T;
    cin >> n >> T;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    int h = n / 2;
    if (n > 50) {
        // greedy
        vector<pair<ll, int>> b;
        for (int i = 0; i < n; i++) b.emplace_back(a[i], i);
        sort(b.rbegin(), b.rend());
        ll current = 0;
        string s(n, '0');
        for (auto &p : b) {
            if (abs(current + p.first - T) < abs(current - T)) {
                current += p.first;
                s[p.second] = '1';
            }
        }
        cout << s << endl;
    } else {
        // meet in middle
        vector<ll> left(a.begin(), a.begin() + h);
        vector<ll> right(a.begin() + h, a.end());
        vector<pair<ll, int>> left_sums;
        function<void(int, ll, int)> gen_left = [&](int i, ll sum, int mask) {
            if (i == (int)left.size()) {
                left_sums.emplace_back(sum, mask);
                return;
            }
            gen_left(i + 1, sum, mask);
            gen_left(i + 1, sum + left[i], mask | (1 << i));
        };
        gen_left(0, 0, 0);
        sort(left_sums.begin(), left_sums.end());
        ll min_err = LLONG_MAX;
        int best_l = 0, best_r = 0;
        function<void(int, ll, int)> gen_right = [&](int i, ll sum, int mask) {
            if (i == (int)right.size()) {
                auto it = lower_bound(left_sums.begin(), left_sums.end(), make_pair(T - sum, -1LL));
                if (it != left_sums.end()) {
                    ll terr = abs(it->first + sum - T);
                    if (terr < min_err) {
                        min_err = terr;
                        best_l = it->second;
                        best_r = mask;
                    }
                }
                if (it != left_sums.begin()) {
                    --it;
                    ll terr = abs(it->first + sum - T);
                    if (terr < min_err) {
                        min_err = terr;
                        best_l = it->second;
                        best_r = mask;
                    }
                }
                return;
            }
            gen_right(i + 1, sum, mask);
            gen_right(i + 1, sum + right[i], mask | (1 << i));
        };
        gen_right(0, 0, 0);
        string s(n, '0');
        for (int i = 0; i < h; i++) {
            if (best_l & (1 << i)) s[i] = '1';
        }
        int rs = right.size();
        for (int i = 0; i < rs; i++) {
            if (best_r & (1 << i)) s[h + i] = '1';
        }
        cout << s << endl;
    }
    return 0;
}
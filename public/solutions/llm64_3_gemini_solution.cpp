#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>

using namespace std;

typedef long long ll;

void generate_sums(int start_idx, int end_idx, const vector<ll>& a, vector<pair<ll, int>>& sums) {
    int len = end_idx - start_idx;
    if (len <= 0) {
        if (len == 0) sums.push_back({0, 0});
        return;
    }
    int num_subsets = 1 << len;
    sums.reserve(num_subsets);
    for (int i = 0; i < num_subsets; ++i) {
        ll current_sum = 0;
        for (int j = 0; j < len; ++j) {
            if ((i >> j) & 1) {
                current_sum += a[start_idx + j];
            }
        }
        sums.push_back({current_sum, i});
    }
}

void solve_mitm(int n, ll T, const vector<ll>& a) {
    int n1 = n / 2;
    vector<pair<ll, int>> sums1, sums2;
    generate_sums(0, n1, a, sums1);
    generate_sums(n1, n, a, sums2);

    sort(sums2.begin(), sums2.end());

    ll min_err = -1;
    int best_mask1 = 0, best_mask2 = 0;

    auto cmp = [](const pair<ll, int>& elem, ll val) {
        return elem.first < val;
    };

    for (const auto& p1 : sums1) {
        ll target = T - p1.first;
        
        auto it = lower_bound(sums2.begin(), sums2.end(), target, cmp);

        if (it != sums2.end()) {
            ll current_sum = p1.first + it->first;
            ll err = abs(current_sum - T);
            if (min_err == -1 || err < min_err) {
                min_err = err;
                best_mask1 = p1.second;
                best_mask2 = it->second;
            }
        }
        if (it != sums2.begin()) {
            it--;
            ll current_sum = p1.first + it->first;
            ll err = abs(current_sum - T);
            if (min_err == -1 || err < min_err) {
                min_err = err;
                best_mask1 = p1.second;
                best_mask2 = it->second;
            }
        }
        if (min_err == 0) break;
    }
    
    string res = "";
    for (int i = 0; i < n1; ++i) {
        res += ((best_mask1 >> i) & 1) ? '1' : '0';
    }
    int n2 = n - n1;
    for (int i = 0; i < n2; ++i) {
        res += ((best_mask2 >> i) & 1) ? '1' : '0';
    }
    cout << res << endl;
}

void solve_sa(int n, ll T, const vector<ll>& a) {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    string current_mask(n, '0');
    ll current_sum = 0;

    for (int i = 0; i < n; ++i) {
        if (uniform_int_distribution<int>(0, 1)(rng)) {
            current_mask[i] = '1';
            current_sum += a[i];
        }
    }

    string best_mask = current_mask;
    ll min_error = abs(current_sum - T);

    double temp = 0;
    for(ll val : a) temp += val;
    if (n > 0) temp /= n;
    if (temp < 1.0) temp = 1.0;

    int iterations = 4000000;

    uniform_int_distribution<int> dist_idx(0, n - 1);
    uniform_real_distribution<double> dist_prob(0.0, 1.0);

    for (int i = 0; i < iterations && min_error > 0; ++i) {
        int idx = dist_idx(rng);
        
        ll next_sum = current_sum;
        char next_char = current_mask[idx];
        if (next_char == '1') {
            next_sum -= a[idx];
            next_char = '0';
        } else {
            next_sum += a[idx];
            next_char = '1';
        }
        
        ll current_error = abs(current_sum - T);
        ll next_error = abs(next_sum - T);
        
        if (next_error < current_error) {
            current_sum = next_sum;
            current_mask[idx] = next_char;
        } else {
            double p = exp((double)(current_error - next_error) / temp);
            if (dist_prob(rng) < p) {
                current_sum = next_sum;
                current_mask[idx] = next_char;
            }
        }
        
        if (abs(current_sum - T) < min_error) {
            min_error = abs(current_sum - T);
            best_mask = current_mask;
        }
        
        temp *= 0.999995;
    }
    
    cout << best_mask << endl;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    ll T;
    cin >> n >> T;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n == 0) {
        cout << "" << endl;
        return 0;
    }

    if (n <= 44) {
        solve_mitm(n, T, a);
    } else {
        solve_sa(n, T, a);
    }

    return 0;
}
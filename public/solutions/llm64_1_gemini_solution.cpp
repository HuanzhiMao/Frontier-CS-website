#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

typedef long long ll;

void generate_sums(int start_idx, int n_sub, const vector<ll>& a, vector<pair<ll, ll>>& sums) {
    int size = 1 << n_sub;
    sums.resize(size);
    for (int i = 0; i < size; ++i) {
        ll current_sum = 0;
        for (int j = 0; j < n_sub; ++j) {
            if ((i >> j) & 1) {
                current_sum += a[start_idx + j];
            }
        }
        sums[i] = {current_sum, (ll)i};
    }
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

    int n1 = n / 2;
    int n2 = n - n1;

    vector<pair<ll, ll>> sums1, sums2;
    generate_sums(0, n1, a, sums1);
    generate_sums(n1, n2, a, sums2);

    sort(sums2.begin(), sums2.end());

    ll min_diff = -1;
    ll best_sum_val = -1;
    ll best_mask1 = 0, best_mask2 = 0;

    for (const auto& p1 : sums1) {
        ll sum1 = p1.first;
        ll mask1 = p1.second;
        ll target = T - sum1;

        auto it = lower_bound(sums2.begin(), sums2.end(), make_pair(target, 0LL));

        if (it != sums2.end()) {
            ll sum2 = it->first;
            ll mask2 = it->second;
            ll current_sum = sum1 + sum2;
            ll current_diff = abs(current_sum - T);

            if (min_diff == -1 || current_diff < min_diff) {
                min_diff = current_diff;
                best_sum_val = current_sum;
                best_mask1 = mask1;
                best_mask2 = mask2;
            } else if (current_diff == min_diff) {
                if (best_sum_val == -1 || current_sum < best_sum_val) {
                    best_sum_val = current_sum;
                    best_mask1 = mask1;
                    best_mask2 = mask2;
                }
            }
        }

        if (it != sums2.begin()) {
            it--;
            ll sum2 = it->first;
            ll mask2 = it->second;
            ll current_sum = sum1 + sum2;
            ll current_diff = abs(current_sum - T);

            if (min_diff == -1 || current_diff < min_diff) {
                min_diff = current_diff;
                best_sum_val = current_sum;
                best_mask1 = mask1;
                best_mask2 = mask2;
            } else if (current_diff == min_diff) {
                if (best_sum_val == -1 || current_sum < best_sum_val) {
                    best_sum_val = current_sum;
                    best_mask1 = mask1;
                    best_mask2 = mask2;
                }
            }
        }
    }

    string result = "";
    for (int i = 0; i < n1; ++i) {
        if ((best_mask1 >> i) & 1) {
            result += '1';
        } else {
            result += '0';
        }
    }
    for (int i = 0; i < n2; ++i) {
        if ((best_mask2 >> i) & 1) {
            result += '1';
        } else {
            result += '0';
        }
    }

    cout << result << endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

void solve() {
    using namespace std;
    using LL = long long;

    const int W = 2000000;

    auto get_sums = [&](const vector<LL>& arr) {
        vector<pair<LL, LL>> sums = {{0, 0}};
        sums.reserve(W + 1);

        for (int i = 0; i < arr.size(); ++i) {
            LL val = arr[i];
            
            vector<pair<LL, LL>> sums_to_add;
            sums_to_add.reserve(sums.size());
            for (const auto& p : sums) {
                sums_to_add.push_back({p.first + val, p.second | (1LL << i)});
            }
            
            vector<pair<LL, LL>> next_sums;
            next_sums.reserve(sums.size() * 2);
            merge(sums.begin(), sums.end(), sums_to_add.begin(), sums_to_add.end(), 
                  back_inserter(next_sums));

            next_sums.erase(unique(next_sums.begin(), next_sums.end(), 
                [](const pair<LL, LL>& a, const pair<LL, LL>& b) {
                    return a.first == b.first;
                }), next_sums.end());

            if (next_sums.size() > W) {
                sums.clear();
                sums.reserve(W);
                for (int j = 0; j < W; ++j) {
                    sums.push_back(next_sums[(long long)j * next_sums.size() / W]);
                }
            } else {
                sums = next_sums;
            }
        }
        return sums;
    };

    int n;
    LL T;
    cin >> n >> T;
    vector<LL> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int n1 = n / 2;
    vector<LL> a1(a.begin(), a.begin() + n1);
    vector<LL> a2(a.begin() + n1, a.end());

    vector<pair<LL, LL>> sums1 = get_sums(a1);
    vector<pair<LL, LL>> sums2 = get_sums(a2);

    sort(sums2.begin(), sums2.end());

    LL min_diff = -1;
    LL best_mask1 = 0, best_mask2 = 0;

    for (const auto& p1 : sums1) {
        LL s1 = p1.first;
        LL m1 = p1.second;
        LL target = T - s1;

        auto it = lower_bound(sums2.begin(), sums2.end(), make_pair(target, 0LL));

        if (it != sums2.end()) {
            LL s2 = it->first;
            LL m2 = it->second;
            LL current_sum = s1 + s2;
            LL diff = abs(current_sum - T);

            if (min_diff == -1 || diff < min_diff) {
                min_diff = diff;
                best_mask1 = m1;
                best_mask2 = m2;
            }
        }
        if (it != sums2.begin()) {
            it--;
            LL s2 = it->first;
            LL m2 = it->second;
            LL current_sum = s1 + s2;
            LL diff = abs(current_sum - T);
            
            if (min_diff == -1 || diff < min_diff) {
                min_diff = diff;
                best_mask1 = m1;
                best_mask2 = m2;
            }
        }
    }

    LL final_mask = best_mask1 | (best_mask2 << n1);

    for (int i = 0; i < n; ++i) {
        if ((final_mask >> i) & 1) {
            cout << '1';
        } else {
            cout << '0';
        }
    }
    cout << endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    solve();
    return 0;
}
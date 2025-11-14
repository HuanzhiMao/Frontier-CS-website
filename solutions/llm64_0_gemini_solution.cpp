#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <string>
#include <queue>
#include <tuple>
#include <map>

using namespace std;

typedef long long ll;
typedef unsigned __int128 u128;

struct SumInfo {
    ll sum;
    u128 mask;

    bool operator<(const SumInfo& other) const {
        return sum < other.sum;
    }
};

string to_binary_string(u128 mask, int n) {
    string s(n, '0');
    for (int i = 0; i < n; ++i) {
        if ((mask >> i) & 1) {
            s[i] = '1';
        }
    }
    return s;
}

vector<SumInfo> generate_sums(const vector<ll>& a, int start_idx, int end_idx) {
    int count = end_idx - start_idx;
    vector<SumInfo> sums;
    if (count < 0) return sums;
    sums.reserve(1 << count);
    for (int i = 0; i < (1 << count); ++i) {
        ll current_sum = 0;
        u128 current_mask = 0;
        for (int j = 0; j < count; ++j) {
            if ((i >> j) & 1) {
                current_sum += a[start_idx + j];
                current_mask |= (u128)1 << (start_idx + j);
            }
        }
        sums.push_back({current_sum, current_mask});
    }
    return sums;
}

void solve() {
    int n;
    ll T;
    cin >> n >> T;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    if (n <= 22) {
        ll best_sum = -1;
        u128 best_mask = 0;
        ll min_diff = -1;

        for (u128 i = 0; i < ((u128)1 << n); ++i) {
            ll current_sum = 0;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    current_sum += a[j];
                }
            }
            ll diff = abs(current_sum - T);
            if (min_diff == -1 || diff < min_diff) {
                min_diff = diff;
                best_sum = current_sum;
                best_mask = i;
            } else if (diff == min_diff) {
                if (current_sum < best_sum) {
                    best_sum = current_sum;
                    best_mask = i;
                } else if (current_sum == best_sum && i < best_mask) {
                    best_mask = i;
                }
            }
        }
        cout << to_binary_string(best_mask, n) << endl;
        return;
    }

    if (n <= 48) {
        int p1 = n / 4;
        int p2 = n / 2;
        int p3 = n * 3 / 4;
        
        vector<SumInfo> s1 = generate_sums(a, 0, p1);
        vector<SumInfo> s2 = generate_sums(a, p1, p2);
        vector<SumInfo> s3 = generate_sums(a, p2, p3);
        vector<SumInfo> s4 = generate_sums(a, p3, n);

        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        sort(s3.begin(), s3.end());
        sort(s4.begin(), s4.end());

        priority_queue<tuple<ll, int, int>, vector<tuple<ll, int, int>>, greater<tuple<ll, int, int>>> pq12;
        for (size_t i = 0; i < s1.size(); ++i) {
            pq12.push({s1[i].sum + s2[0].sum, (int)i, 0});
        }

        priority_queue<tuple<ll, int, int>> pq34;
        for (size_t i = 0; i < s3.size(); ++i) {
            pq34.push({s3[i].sum + s4.back().sum, (int)i, (int)s4.size() - 1});
        }

        ll min_err = -1;
        u128 best_mask = 0;

        while (!pq12.empty() && !pq34.empty()) {
            auto [sum12, i1, i2] = pq12.top();
            auto [sum34, i3, i4] = pq34.top();

            ll current_total = sum12 + sum34;
            ll err = abs(current_total - T);

            if (min_err == -1 || err < min_err) {
                min_err = err;
                best_mask = s1[i1].mask | s2[i2].mask | s3[i3].mask | s4[i4].mask;
            }

            if (min_err == 0) break;

            if (current_total < T) {
                pq12.pop();
                if (i2 + 1 < (int)s2.size()) {
                    pq12.push({s1[i1].sum + s2[i2 + 1].sum, i1, i2 + 1});
                }
            } else {
                pq34.pop();
                if (i4 > 0) {
                    pq34.push({s3[i3].sum + s4[i4 - 1].sum, i3, i4 - 1});
                }
            }
        }
        cout << to_binary_string(best_mask, n) << endl;
        return;
    }
    
    int W = 200000;
    vector<SumInfo> beam;
    beam.push_back({0, 0});

    for (int i = 0; i < n; ++i) {
        vector<SumInfo> next_beam;
        next_beam.reserve(beam.size() * 2);
        for (const auto& p : beam) {
            next_beam.push_back(p);
            next_beam.push_back({p.sum + a[i], p.mask | ((u128)1 << i)});
        }
        
        sort(next_beam.begin(), next_beam.end(), [&](const SumInfo& p1, const SumInfo& p2) {
            return abs(p1.sum - T) < abs(p2.sum - T);
        });

        map<ll, u128> unique_sums;
        vector<SumInfo> filtered_beam;
        filtered_beam.reserve(W);
        for(const auto& p : next_beam) {
            if(unique_sums.size() >= W) break;
            if(unique_sums.find(p.sum) == unique_sums.end()) {
                unique_sums[p.sum] = p.mask;
                filtered_beam.push_back(p);
            }
        }
        beam = filtered_beam;
    }
    
    sort(beam.begin(), beam.end(), [&](const SumInfo& p1, const SumInfo& p2) {
        ll d1 = abs(p1.sum - T);
        ll d2 = abs(p2.sum - T);
        if (d1 != d2) return d1 < d2;
        return p1.mask < p2.mask;
    });

    cout << to_binary_string(beam[0].mask, n) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>

using namespace std;

// Using __int128_t for large numbers to avoid overflow
using int128 = __int128_t;

// Helper to print __int128_t, as it's not supported by default
ostream& operator<<(ostream& os, int128 val) {
    if (val < 0) {
        os << "-";
        val = -val;
    }
    if (val == 0) return os << "0";
    string s;
    while (val > 0) {
        s += (char)((val % 10) + '0');
        val /= 10;
    }
    reverse(s.begin(), s.end());
    return os << s;
}

// Get rank for a given amount of experience points
long long get_rank(int128 exp, const vector<int128>& pb) {
    if (exp < 0) return -1;
    auto it = upper_bound(pb.begin() + 1, pb.end(), exp);
    return distance(pb.begin(), it) - 1;
}

void solve() {
    int n, m;
    long long c_ll;
    cin >> n >> m >> c_ll;

    vector<int128> a(n);
    for (int i = 0; i < n; ++i) {
        long long val;
        cin >> val;
        a[i] = val;
    }

    vector<int128> b(m);
    for (int i = 0; i < m; ++i) {
        long long val;
        cin >> val;
        b[i] = val;
    }
    
    vector<int128> pa(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pa[i + 1] = pa[i] + a[i];
    }

    vector<int128> pb(m + 1, 0);
    for (int i = 0; i < m; ++i) {
        pb[i + 1] = pb[i] + b[i];
    }

    vector<int128> dp(n + 1, 0);
    
    // A 'record' is a pair {pa[j], dp[j]-pa[j]} for a past day j,
    // which is not dominated by any other past day's record.
    // A record (p1, v1) dominates (p2, v2) if p1 <= p2 and v1 >= v2.
    // We only store non-dominated records, which will have pa[j] and dp[j]-pa[j] increasing.
    vector<pair<int128, int128>> records;
    records.push_back({pa[0], dp[0] - pa[0]});

    // Precompute B_k = k - pb_k
    vector<int128> B(m + 1);
    for (int k = 1; k <= m; ++k) {
        B[k] = k - pb[k];
    }
    
    // Precompute prefix maximums of B_k
    vector<int128> premax_B(m + 1, -3e18); // Initialize with a very small number
    if (m > 0) {
        premax_B[0] = -3e18; 
        for (int k = 1; k <= m; ++k) {
            premax_B[k] = max(premax_B[k-1], B[k]);
        }
    }

    int128 max_dp_so_far = 0;

    for (int i = 1; i <= n; ++i) {
        // The DP transition can be optimized.
        // dp[i] = max_{0<=j<i} (dp[j] + rank(pa[i]-pa[j]) - c)
        // This can be rewritten as:
        // dp[i] = pa[i] - c + max_{j,k} (dp[j]-pa[j] + k-pb[k])
        // subject to pa[j]+pb[k] <= pa[i].
        // Let A_j = dp[j]-pa[j] and B_k = k-pb[k]. We want max(A_j + B_k).
        // This is max_j(A_j + max_{k s.t. pb[k]<=pa[i]-pa[j]} B_k).
        // The inner max is precomputed as premax_B.

        // Contribution from ranks 1..m
        int128 max_contrib = -3e18; 

        for (const auto& rec : records) {
            int128 p_j = rec.first;
            int128 A_j = rec.second;
            
            int128 current_exp = pa[i] - p_j;
            long long k_max = get_rank(current_exp, pb);

            if (k_max >= 1) {
                max_contrib = max(max_contrib, A_j + premax_B[k_max]);
            }
        }
        
        int128 max_val;
        if (max_contrib > -2.5e18) { // Check if any valid rank > 0 segment was found
            max_val = pa[i] - c_ll + max_contrib;
        } else {
            max_val = -3e18; // No valid segment with rank > 0 found
        }

        // Contribution from rank 0: value is dp[j] - c.
        // To maximize this, we take max(dp[j]) over all j < i.
        max_val = max(max_val, max_dp_so_far - c_ll);

        dp[i] = max_val;
        max_dp_so_far = max(max_dp_so_far, dp[i]);
        
        // Add dp[i] to records if it's a new non-dominated point
        int128 new_A_i = dp[i] - pa[i];
        if (records.empty() || new_A_i > records.back().second) {
            // Maintain the sorted and non-dominated properties of records
            while (!records.empty() && records.back().first >= pa[i]) {
                records.pop_back();
            }
            records.push_back({pa[i], new_A_i});
        }
    }
    cout << (long long)dp[n] << "\n";
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
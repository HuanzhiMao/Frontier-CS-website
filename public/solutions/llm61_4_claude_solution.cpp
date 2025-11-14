#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    
    while (T--) {
        int n, m, c;
        cin >> n >> m >> c;
        
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        
        vector<long long> b(m);
        for (int i = 0; i < m; i++) {
            cin >> b[i];
        }
        
        // Precompute prefix sums for b
        vector<long long> prefix_b(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            prefix_b[i] = prefix_b[i - 1] + b[i - 1];
        }
        
        // Function to get level from EXP
        auto getLevel = [&](long long exp) -> int {
            int level = 0;
            for (int i = 1; i <= m; i++) {
                if (exp >= prefix_b[i]) {
                    level = i;
                } else {
                    break;
                }
            }
            return level;
        };
        
        // Precompute all possible segment values
        vector<vector<int>> segmentLevel(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            long long sum = 0;
            for (int j = i; j < n; j++) {
                sum += a[j];
                segmentLevel[i][j] = getLevel(sum);
            }
        }
        
        // DP: dp[i] = maximum value using first i days
        vector<long long> dp(n + 1, LLONG_MIN);
        dp[0] = 0;
        
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] != LLONG_MIN) {
                    int level = segmentLevel[j][i - 1];
                    dp[i] = max(dp[i], dp[j] + level - c);
                }
            }
        }
        
        cout << dp[n] << "\n";
    }
    
    return 0;
}
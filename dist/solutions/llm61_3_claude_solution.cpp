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
        
        // Compute prefix sums for level requirements
        vector<long long> level_req(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            level_req[i] = level_req[i - 1] + b[i - 1];
        }
        
        // Function to get level from total EXP
        auto getLevel = [&](long long exp) -> int {
            int level = 0;
            for (int i = 1; i <= m; i++) {
                if (exp >= level_req[i]) {
                    level = i;
                } else {
                    break;
                }
            }
            return level;
        };
        
        // Precompute all possible segment values
        vector<int> segmentValue(n * (n + 1) / 2);
        int idx = 0;
        for (int l = 0; l < n; l++) {
            long long sum = 0;
            for (int r = l; r < n; r++) {
                sum += a[r];
                segmentValue[idx++] = getLevel(sum);
            }
        }
        
        // DP: dp[i][j] = maximum value using first i days with j characters
        vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, -1e18));
        dp[0][0] = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                if (dp[i][j] == -1e18) continue;
                
                // Try all possible segments starting from day i+1
                idx = i * n - i * (i - 1) / 2;
                long long sum = 0;
                for (int r = i; r < n; r++) {
                    sum += a[r];
                    int level = getLevel(sum);
                    dp[r + 1][j + 1] = max(dp[r + 1][j + 1], dp[i][j] + level);
                }
            }
        }
        
        long long ans = -1e18;
        for (int j = 0; j <= n; j++) {
            ans = max(ans, dp[n][j] - (long long)c * j);
        }
        
        cout << ans << "\n";
    }
    
    return 0;
}
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
        
        vector<long long> b(m + 1);
        b[0] = 0;
        for (int i = 1; i <= m; i++) {
            cin >> b[i];
            b[i] += b[i-1];
        }
        
        // Function to get level from exp
        auto getLevel = [&](long long exp) -> int {
            int level = 0;
            for (int i = 1; i <= m; i++) {
                if (exp >= b[i]) {
                    level = i;
                } else {
                    break;
                }
            }
            return level;
        };
        
        // Precompute prefix sums
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + a[i];
        }
        
        // dp[i] = maximum value using first i days
        vector<long long> dp(n + 1, LLONG_MIN);
        dp[0] = 0;
        
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] == LLONG_MIN) continue;
                
                long long exp = prefix[i] - prefix[j];
                int level = getLevel(exp);
                long long value = dp[j] + level - c;
                
                dp[i] = max(dp[i], value);
            }
        }
        
        cout << dp[n] << "\n";
    }
    
    return 0;
}
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
        
        // Precompute cumulative requirements
        vector<long long> req(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            req[i] = req[i - 1] + b[i - 1];
        }
        
        // Function to get level from EXP
        auto getLevel = [&](long long exp) -> int {
            int level = 0;
            for (int i = 1; i <= m; i++) {
                if (exp >= req[i]) {
                    level = i;
                } else {
                    break;
                }
            }
            return level;
        };
        
        // dp[i] = maximum value using first i days
        vector<long long> dp(n + 1, LLONG_MIN);
        dp[0] = 0;
        
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                // Play character from day j+1 to day i
                long long totalExp = 0;
                for (int k = j; k < i; k++) {
                    totalExp += a[k];
                }
                
                int level = getLevel(totalExp);
                long long value = level - c;
                
                if (dp[j] != LLONG_MIN) {
                    dp[i] = max(dp[i], dp[j] + value);
                }
            }
        }
        
        cout << dp[n] << "\n";
    }
    
    return 0;
}
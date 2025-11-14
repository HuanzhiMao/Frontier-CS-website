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
        
        // Compute prefix sums for b
        vector<long long> prefixB(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            prefixB[i] = prefixB[i - 1] + b[i - 1];
        }
        
        // Function to get level from EXP
        auto getLevel = [&](long long exp) -> int {
            int level = 0;
            for (int i = 1; i <= m; i++) {
                if (exp >= prefixB[i]) {
                    level = i;
                } else {
                    break;
                }
            }
            return level;
        };
        
        // DP
        vector<long long> dp(n + 1, LLONG_MIN);
        dp[0] = 0;
        
        for (int i = 0; i < n; i++) {
            if (dp[i] == LLONG_MIN) continue;
            
            long long sum = 0;
            for (int j = i; j < n; j++) {
                sum += a[j];
                int level = getLevel(sum);
                long long value = level - c;
                dp[j + 1] = max(dp[j + 1], dp[i] + value);
            }
        }
        
        cout << dp[n] << "\n";
    }
    
    return 0;
}
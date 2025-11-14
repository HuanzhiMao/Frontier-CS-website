#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    long long T;
    cin >> n >> T;
    
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    long long best_diff = LLONG_MAX;
    int best_mask = 0;
    
    // Try all possible subsets
    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += a[i];
            }
        }
        
        long long diff = abs(sum - T);
        if (diff < best_diff) {
            best_diff = diff;
            best_mask = mask;
        }
    }
    
    string result(n, '0');
    for (int i = 0; i < n; i++) {
        if (best_mask & (1 << i)) {
            result[i] = '1';
        }
    }
    
    cout << result << endl;
    
    return 0;
}
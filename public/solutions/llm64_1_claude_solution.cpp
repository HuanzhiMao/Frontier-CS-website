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
    
    string best_subset;
    long long best_error = LLONG_MAX;
    
    // Try all possible subsets using bit manipulation
    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += a[i];
            }
        }
        
        long long error = abs(sum - T);
        if (error < best_error) {
            best_error = error;
            best_subset = "";
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    best_subset += '1';
                } else {
                    best_subset += '0';
                }
            }
        }
    }
    
    cout << best_subset << endl;
    
    return 0;
}
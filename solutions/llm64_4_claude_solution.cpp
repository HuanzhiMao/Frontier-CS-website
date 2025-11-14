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
    string best_subset = "";
    
    // Try all possible subsets
    for (int mask = 0; mask < (1 << n); mask++) {
        long long sum = 0;
        string subset = "";
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sum += a[i];
                subset += "1";
            } else {
                subset += "0";
            }
        }
        
        long long diff = abs(sum - T);
        if (diff < best_diff) {
            best_diff = diff;
            best_subset = subset;
        }
    }
    
    cout << best_subset << endl;
    
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    // Get starting position
    cout << "walk 0" << endl;
    int start;
    cin >> start;
    
    // Try different step sizes to find cycle length
    vector<int> factors;
    
    // Try small factors first
    for (int i = 1; i <= 1000; i++) {
        cout << "walk " << i << endl;
        int pos;
        cin >> pos;
        if (pos == start) {
            factors.push_back(i);
        }
    }
    
    if (!factors.empty()) {
        int n = factors[0];
        for (int f : factors) {
            n = __gcd(n, f);
        }
        cout << "guess " << n << endl;
        return 0;
    }
    
    // If no small factors found, try larger jumps
    int operations = 1000;
    
    // Try powers of 2 with some randomization
    for (int exp = 10; exp <= 30 && operations < 100000; exp++) {
        long long step = 1LL << exp;
        if (step > 1000000000LL) break;
        
        cout << "walk " << step << endl;
        operations++;
        int pos;
        cin >> pos;
        
        if (pos == start) {
            factors.push_back(step);
        }
    }
    
    // Try some other strategic values
    vector<int> test_values = {1001, 10007, 100003, 1000003, 10000019, 100000007};
    
    for (int val : test_values) {
        if (operations >= 150000) break;
        cout << "walk " << val << endl;
        operations++;
        int pos;
        cin >> pos;
        if (pos == start) {
            factors.push_back(val);
        }
    }
    
    // Binary search approach for remaining operations
    if (factors.empty() && operations < 180000) {
        int left = 1000001;
        int right = 1000000000;
        
        while (left < right && operations < 180000) {
            int mid = left + (right - left) / 2;
            cout << "walk " << mid << endl;
            operations++;
            int pos;
            cin >> pos;
            
            if (pos == start) {
                factors.push_back(mid);
                break;
            }
            
            // Adjust search range heuristically
            if (operations % 2 == 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    if (!factors.empty()) {
        int n = factors[0];
        for (int f : factors) {
            n = __gcd(n, f);
        }
        cout << "guess " << n << endl;
    } else {
        // Last resort guess
        cout << "guess 1000000007" << endl;
    }
    
    return 0;
}
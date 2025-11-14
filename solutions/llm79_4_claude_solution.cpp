#include <bits/stdc++.h>
using namespace std;

long long bits(long long x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

long long modpow_time(long long a, long long d, long long n) {
    long long r = 1;
    long long total_time = 0;
    
    for (int i = 0; i < 60; i++) {
        if ((d & (1LL << i)) != 0) {
            total_time += (bits(r) + 1) * (bits(a) + 1);
            r = (r * a) % n;
        }
        total_time += (bits(a) + 1) * (bits(a) + 1);
        a = (a * a) % n;
    }
    
    return total_time;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    vector<pair<long long, long long>> queries;
    
    // Make some strategic queries
    for (int i = 0; i < 20; i++) {
        long long a = 2 + i;
        cout << "? " << a << endl;
        cout.flush();
        
        long long time;
        cin >> time;
        
        queries.push_back({a, time});
    }
    
    // Try to find d by checking possible values
    // Since d can be very large, we need to be smart about this
    
    // First, let's try small values of d to see if we can find a pattern
    for (long long d = 1; d < min(1000000LL, n); d++) {
        bool matches = true;
        
        for (auto& query : queries) {
            long long expected_time = modpow_time(query.first, d, n);
            if (expected_time != query.second) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            cout << "! " << d << endl;
            cout.flush();
            return 0;
        }
    }
    
    // If small d doesn't work, we need a different approach
    // Let's try to deduce d bit by bit using timing differences
    
    long long d = 0;
    
    // Try to reconstruct d bit by bit
    for (int bit = 0; bit < 60; bit++) {
        long long test_d1 = d;
        long long test_d2 = d | (1LL << bit);
        
        // Use a simple test case
        long long test_a = 2;
        long long time1 = modpow_time(test_a, test_d1, n);
        long long time2 = modpow_time(test_a, test_d2, n);
        
        // Find which matches better with our known query
        bool found = false;
        for (auto& query : queries) {
            if (query.first == test_a) {
                if (abs(time2 - query.second) < abs(time1 - query.second)) {
                    d = test_d2;
                }
                found = true;
                break;
            }
        }
        
        if (!found) {
            // Make a new query
            cout << "? " << test_a << endl;
            cout.flush();
            long long actual_time;
            cin >> actual_time;
            
            if (abs(time2 - actual_time) < abs(time1 - actual_time)) {
                d = test_d2;
            }
        }
    }
    
    cout << "! " << d << endl;
    cout.flush();
    
    return 0;
}
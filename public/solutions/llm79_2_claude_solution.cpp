#include <bits/stdc++.h>
using namespace std;

long long bits(long long x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

long long modpow_time(long long a, long long d, long long n) {
    long long r = 1;
    long long time = 0;
    
    for (int i = 0; i < 60; i++) {
        if ((d & (1LL << i)) != 0) {
            time += (bits(r) + 1) * (bits(a) + 1);
            r = (r * a) % n;
        }
        time += (bits(a) + 1) * (bits(a) + 1);
        a = (a * a) % n;
    }
    
    return time;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    map<long long, long long> queries;
    
    // Try different values of a and collect timing information
    vector<long long> test_values = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    
    for (long long a : test_values) {
        if (a >= n) continue;
        cout << "? " << a << endl;
        cout.flush();
        long long time;
        cin >> time;
        queries[a] = time;
    }
    
    // Try to find d by testing candidates
    long long found_d = 1;
    
    // Since d can be very large, we need a smarter approach
    // We'll use the fact that the timing depends on the bit pattern of d
    
    // Binary search approach on d
    long long low = 1, high = n;
    
    // Try a few more strategic values
    vector<long long> more_tests = {31, 37, 41, 43, 47};
    for (long long a : more_tests) {
        if (a >= n) continue;
        if (queries.count(a)) continue;
        cout << "? " << a << endl;
        cout.flush();
        long long time;
        cin >> time;
        queries[a] = time;
    }
    
    // Try to reconstruct d bit by bit using timing information
    long long candidate_d = 0;
    
    // Test powers of 2 and small values systematically
    for (long long d_test = 1; d_test < min(10000LL, n); d_test++) {
        bool matches = true;
        
        for (auto& p : queries) {
            long long a = p.first;
            long long expected_time = p.second;
            long long computed_time = modpow_time(a, d_test, n);
            
            if (computed_time != expected_time) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            found_d = d_test;
            break;
        }
    }
    
    // If not found in small range, try larger systematic search
    if (found_d == 1) {
        // Use more queries to narrow down
        for (int extra = 0; extra < 10 && queries.size() < 20; extra++) {
            long long a = 53 + extra * 10;
            if (a >= n) break;
            cout << "? " << a << endl;
            cout.flush();
            long long time;
            cin >> time;
            queries[a] = time;
        }
        
        // Try larger range
        for (long long d_test = 1; d_test < min(100000LL, n); d_test++) {
            bool matches = true;
            
            for (auto& p : queries) {
                long long a = p.first;
                long long expected_time = p.second;
                long long computed_time = modpow_time(a, d_test, n);
                
                if (computed_time != expected_time) {
                    matches = false;
                    break;
                }
            }
            
            if (matches) {
                found_d = d_test;
                break;
            }
        }
    }
    
    cout << "! " << found_d << endl;
    cout.flush();
    
    return 0;
}
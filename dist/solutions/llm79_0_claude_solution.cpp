#include <bits/stdc++.h>
using namespace std;

long long bits(long long x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

long long computeTime(long long a, long long d, long long n) {
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
    
    // Try different values of a to gather timing information
    vector<long long> test_values = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    
    for (long long a : test_values) {
        if (a >= n) continue;
        cout << "? " << a << endl;
        cout.flush();
        long long time;
        cin >> time;
        queries[a] = time;
    }
    
    // Binary search approach to find d
    long long left = 1, right = n - 1;
    long long best_d = 1;
    
    // Use binary search with multiple test values
    while (right - left > 1000) {
        long long mid = (left + right) / 2;
        
        bool matches = true;
        for (auto& p : queries) {
            long long expected_time = computeTime(p.first, mid, n);
            if (expected_time != p.second) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            best_d = mid;
            break;
        }
        
        // Try to narrow down based on timing differences
        long long test_a = 2;
        if (queries.count(test_a)) {
            long long expected_time = computeTime(test_a, mid, n);
            if (expected_time > queries[test_a]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {
            left = mid + 1;
        }
    }
    
    // Brute force in the remaining range
    for (long long d = left; d <= right && d < n; d++) {
        bool matches = true;
        for (auto& p : queries) {
            long long expected_time = computeTime(p.first, d, n);
            if (expected_time != p.second) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            best_d = d;
            break;
        }
    }
    
    cout << "! " << best_d << endl;
    cout.flush();
    
    return 0;
}
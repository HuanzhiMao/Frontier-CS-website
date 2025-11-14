#include <bits/stdc++.h>
using namespace std;

long long bits(long long x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

long long modpow(long long a, long long d, long long n) {
    long long r = 1;
    for (int i = 0; i < 60; i++) {
        if ((d & (1LL << i)) != 0) {
            r = (__int128)r * a % n;
        }
        a = (__int128)a * a % n;
    }
    return r;
}

long long simulate_time(long long a, long long d, long long n) {
    long long r = 1;
    long long total_time = 0;
    
    for (int i = 0; i < 60; i++) {
        if ((d & (1LL << i)) != 0) {
            total_time += (bits(r) + 1) * (bits(a) + 1);
            r = (__int128)r * a % n;
        }
        total_time += (bits(a) + 1) * (bits(a) + 1);
        a = (__int128)a * a % n;
    }
    return total_time;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n;
    cin >> n;
    
    // Try to find patterns by querying small values
    vector<pair<long long, long long>> queries;
    
    for (int a = 2; a <= 20; a++) {
        if (gcd(a, n) == 1) {
            cout << "? " << a << endl;
            cout.flush();
            long long time;
            cin >> time;
            queries.push_back({a, time});
        }
    }
    
    // Binary search approach for d
    long long low = 1, high = n;
    
    while (queries.size() < 25 && high - low > 1) {
        long long mid = (low + high) / 2;
        
        // Test with a = 2
        long long test_a = 2;
        while (gcd(test_a, n) != 1) test_a++;
        
        cout << "? " << test_a << endl;
        cout.flush();
        long long actual_time;
        cin >> actual_time;
        
        long long sim_time_low = simulate_time(test_a, low, n);
        long long sim_time_mid = simulate_time(test_a, mid, n);
        long long sim_time_high = simulate_time(test_a, high, n);
        
        if (abs(actual_time - sim_time_mid) < abs(actual_time - sim_time_high)) {
            high = mid;
        } else {
            low = mid;
        }
        
        queries.push_back({test_a, actual_time});
    }
    
    // Brute force in the remaining range
    long long best_d = low;
    long long min_error = LLONG_MAX;
    
    for (long long d = low; d <= high && d <= low + 10000; d++) {
        long long total_error = 0;
        bool valid = true;
        
        for (auto& q : queries) {
            long long expected = simulate_time(q.first, d, n);
            long long error = abs(expected - q.second);
            total_error += error;
            if (error > q.second / 10) { // Allow 10% error
                valid = false;
                break;
            }
        }
        
        if (valid && total_error < min_error) {
            min_error = total_error;
            best_d = d;
        }
    }
    
    cout << "! " << best_d << endl;
    cout.flush();
    
    return 0;
}
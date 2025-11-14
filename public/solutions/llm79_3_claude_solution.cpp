#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int bits(ll x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

ll simulate_time(ll a, ll d, ll n) {
    ll r = 1;
    ll total_time = 0;
    
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
    
    ll n;
    cin >> n;
    
    vector<pair<ll, ll>> queries;
    
    // Query with a few small values
    for (ll a = 2; a <= 10 && queries.size() < 20; a++) {
        cout << "? " << a << endl;
        cout.flush();
        ll time;
        cin >> time;
        queries.push_back({a, time});
    }
    
    // Try to determine d by binary search approach
    ll max_d = (1LL << 60) - 1;
    
    for (ll candidate_d = 1; candidate_d < max_d; candidate_d++) {
        bool matches = true;
        
        for (auto& query : queries) {
            ll expected_time = simulate_time(query.first, candidate_d, n);
            if (expected_time != query.second) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            cout << "! " << candidate_d << endl;
            cout.flush();
            return 0;
        }
        
        if (candidate_d > 1000000) break;
    }
    
    // If brute force doesn't work, try to reconstruct bit by bit
    ll d = 0;
    
    for (int bit = 0; bit < 60; bit++) {
        ll test_d = d | (1LL << bit);
        bool matches = true;
        
        for (auto& query : queries) {
            ll expected_time = simulate_time(query.first, test_d, n);
            if (expected_time != query.second) {
                matches = false;
                break;
            }
        }
        
        if (matches) {
            d = test_d;
        }
    }
    
    cout << "! " << d << endl;
    cout.flush();
    
    return 0;
}
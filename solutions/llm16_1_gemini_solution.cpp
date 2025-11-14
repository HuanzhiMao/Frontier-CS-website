#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

long long N_val;

// Function to perform a query
long long query(long long u, long long v) {
    if (u == v) return 0;
    cout << "? " << u << " " << v << endl;
    long long d;
    cin >> d;
    return d;
}

// Function to calculate distance in the original cycle
long long dist_cycle(long long u, long long v) {
    if (u > v) swap(u, v);
    return min(v - u, N_val - (v - u));
}

void solve() {
    cin >> N_val;

    long long u = -1;
    long long ref = 1;

    // --- Step 1: Find one endpoint 'u' using ref 1 ---
    long long low = 1, high = N_val;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (mid == ref) {
            low = mid + 1;
            continue;
        }
        if (query(ref, mid) < dist_cycle(ref, mid)) {
            u = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    // --- Step 2: Handle bad reference point ---
    if (u == -1) {
        ref = N_val / 2 + 1;
        low = 1, high = N_val;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (mid == ref) {
                low = mid + 1;
                continue;
            }
            if (query(ref, mid) < dist_cycle(ref, mid)) {
                u = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
    }
    
    // --- Step 3: Find the other endpoint 'v' ---
    long long v;
    long long dist_ref_u = query(ref, u);
    long long dist_ref_v = dist_ref_u - 1;

    long long v1 = ref + dist_ref_v;
    if (v1 > N_val) v1 -= N_val;

    long long v2 = ref - dist_ref_v;
    if (v2 <= 0) v2 += N_val;

    if (v1 > 0 && v1 <= N_val && v1 != u && query(u, v1) == 1) {
        v = v1;
    } else {
        v = v2;
    }

    if (u > v) swap(u, v);
    cout << "! " << u << " " << v << endl;

    int r;
    cin >> r;
    if (r == -1) {
        exit(0);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}
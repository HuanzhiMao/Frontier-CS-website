#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

long long n;
map<pair<long long, long long>, long long> memo;

long long query(long long x, long long y) {
    if (x == y) return 0;
    if (x > y) swap(x, y);
    if (memo.count({x, y})) {
        return memo[{x, y}];
    }
    cout << "? " << x << " " << y << endl;
    long long dist;
    cin >> dist;
    if (dist == -1) exit(0);
    return memo[{x, y}] = dist;
}

void answer(long long u, long long v) {
    if (u > v) swap(u, v);
    cout << "! " << u << " " << v << endl;
    int r;
    cin >> r;
    if (r == -1) {
        exit(0);
    }
}

long long cyc_add(long long base, long long offset) {
    long long res = base + offset;
    if (res > n) {
        res -= n;
    }
    return res;
}

long long cyc_sub(long long base, long long offset) {
    long long res = base - offset;
    if (res < 1) {
        res += n;
    }
    return res;
}

long long get_opposite_dist(long long x) {
    long long opp1 = cyc_add(x, n / 2);
    if (n % 2 == 0) {
        return query(x, opp1);
    } else {
        long long opp2 = cyc_add(opp1, 1);
        return min(query(x, opp1), query(x, opp2));
    }
}

void solve() {
    cin >> n;
    memo.clear();

    long long l, r, m1_cand, m2_cand;
    long long val1, val2;
    
    // Step 1: Ternary search for one of the midpoints of the chord arcs
    l = 1, r = n / 2 + 1;
    while (r - l >= 3) {
        m1_cand = l + (r - l) / 3;
        m2_cand = r - (r - l) / 3;
        val1 = get_opposite_dist(m1_cand);
        val2 = get_opposite_dist(m2_cand);
        if (val1 < val2) {
            r = m2_cand - 1;
        } else {
            l = m1_cand + 1;
        }
    }

    long long min_dist_mid = -1;
    long long axis_p = -1;
    for (long long i = l; i <= r; ++i) {
        long long d = get_opposite_dist(i);
        if (axis_p == -1 || d < min_dist_mid) {
            min_dist_mid = d;
            axis_p = i;
        }
    }

    // Step 2: Ternary search for k, the distance from midpoint to chord endpoints
    l = 1, r = n / 2;
    
    while (r - l >= 3) {
        m1_cand = l + (r - l) / 3;
        m2_cand = r - (r - l) / 3;
        
        val1 = query(cyc_add(axis_p, m1_cand), cyc_sub(axis_p, m1_cand));
        val2 = query(cyc_add(axis_p, m2_cand), cyc_sub(axis_p, m2_cand));

        if (val1 < val2) {
            r = m2_cand - 1;
        } else {
            l = m1_cand + 1;
        }
    }

    long long min_dist_k = -1;
    long long best_k = -1;
    for (long long k_cand = l; k_cand <= r; ++k_cand) {
        if (k_cand == 0) continue;
        long long u_cand = cyc_add(axis_p, k_cand);
        long long v_cand = cyc_sub(axis_p, k_cand);
        if (u_cand == v_cand) continue;
        long long d = query(u_cand, v_cand);
        if (best_k == -1 || d < min_dist_k) {
            min_dist_k = d;
            best_k = k_cand;
        }
    }
    
    answer(cyc_add(axis_p, best_k), cyc_sub(axis_p, best_k));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.flush();
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
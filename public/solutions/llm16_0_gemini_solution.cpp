#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

long long N;
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

void solve() {
    memo.clear();
    cin >> N;

    long long p = -1;

    // Binary search on derivative to find the vertex p furthest from vertex 1
    long long l = 1, r = N;
    while (l < r) {
        long long mid = l + (r - l) / 2;
        if (query(1, mid) < query(1, mid + 1)) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    p = l;
    
    long long k = query(1, p);

    long long u1 = -1, v1 = -1;

    // A vertex x is on a shortest path between 1 and p if query(1,x) + query(x,p) == k.
    // The chord (u,v) bypasses a segment of a cycle path between 1 and p.
    // Vertices on that segment will not be on any shortest path.
    // This creates two groups of vertices on the arc that are on shortest paths, separated by the bypassed segment.
    // u is the last vertex of the first group, v is the first vertex of the second group.
    
    // Check clockwise arc from 1 to p
    {
        // Binary search for u: last vertex on the arc [1..p] on a shortest path from 1's side.
        long long low = 1, high = p, ans = 1;
        while(low <= high) {
            long long mid = low + (high-low)/2;
            if (query(1, mid) + query(mid, p) == k) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        u1 = ans;

        // Binary search for v: first vertex on the arc [1..p] on a shortest path from p's side.
        low = u1, high = p, ans = p;
        while(low <= high) {
            long long mid = low + (high-low)/2;
            if (query(1, mid) + query(mid, p) == k) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        v1 = ans;
    }

    if (u1 < v1) {
        cout << "! " << u1 << " " << v1 << endl;
    } else {
        // If u1 >= v1, it means the whole arc is a shortest path, so the chord is on the other arc.
        // Search on counter-clockwise arc from 1 to p.
        long long dist_1_p_ccw = N - (p - 1);
        long long u2, v2;
        
        // BS for u on CCW arc.
        long long low = 0, high = dist_1_p_ccw, ans_idx = 0;
        while(low <= high) {
            long long mid_idx = low + (high - low) / 2;
            long long mid_node = (1 - 1 - mid_idx % N + N) % N + 1;
            
            if (query(1, mid_node) + query(mid_node, p) == k) {
                ans_idx = mid_idx;
                low = mid_idx + 1;
            } else {
                high = mid_idx - 1;
            }
        }
        u2 = (1 - 1 - ans_idx % N + N) % N + 1;
        
        // BS for v on CCW arc.
        low = ans_idx, high = dist_1_p_ccw;
        ans_idx = high;
        while(low <= high) {
            long long mid_idx = low + (high - low) / 2;
            long long mid_node = (1 - 1 - mid_idx % N + N) % N + 1;
            
            if (query(1, mid_node) + query(mid_node, p) == k) {
                ans_idx = mid_idx;
                high = mid_idx - 1;
            } else {
                low = mid_idx + 1;
            }
        }
        v2 = (1 - 1 - ans_idx % N + N) % N + 1;

        if (u2 > v2) swap(u2, v2);
        cout << "! " << u2 << " " << v2 << endl;
    }

    int result;
    cin >> result;
    if (result == -1) {
        exit(0);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }

    return 0;
}
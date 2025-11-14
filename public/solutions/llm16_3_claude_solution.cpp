#include <bits/stdc++.h>
using namespace std;

int query(int x, int y) {
    cout << "? " << x << " " << y << endl;
    cout.flush();
    int dist;
    cin >> dist;
    return dist;
}

void answer(int u, int v) {
    cout << "! " << u << " " << v << endl;
    cout.flush();
    int result;
    cin >> result;
}

int main() {
    int T;
    cin >> T;
    
    while (T--) {
        int n;
        cin >> n;
        
        // Find one endpoint of the chord using binary search
        int left = 1, right = n;
        int chord_endpoint = -1;
        
        while (left < right) {
            int mid = (left + right) / 2;
            int dist = query(1, mid + 1);
            
            // In original cycle, distance from 1 to mid+1 would be min(mid, n-mid)
            int expected_dist = min(mid, n - mid);
            
            if (dist < expected_dist) {
                // There's a chord affecting this path
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        // Now we need to find the exact chord
        // Try a different approach - check distances around the cycle
        vector<pair<int, int>> candidates;
        
        // Sample some points and check for anomalies
        for (int step = 1; step <= 20 && step < n; step++) {
            int pos = 1 + step;
            if (pos > n) pos -= n;
            
            int dist = query(1, pos);
            int expected = min(step, n - step);
            
            if (dist < expected) {
                // Found anomaly, the chord likely connects vertices that create shortcut
                // Binary search to find exact positions
                int a = 1, b = pos;
                
                // Try to find the chord endpoints more precisely
                for (int offset = 1; offset <= min(10, n/4); offset++) {
                    int test1 = 1 + offset;
                    int test2 = pos - offset;
                    if (test1 > n) test1 -= n;
                    if (test2 <= 0) test2 += n;
                    
                    int d1 = query(test1, pos);
                    int expected1 = min(abs(pos - test1), n - abs(pos - test1));
                    
                    if (d1 == 1) {
                        answer(test1, pos);
                        goto next_test;
                    }
                }
                
                // If direct approach doesn't work, try systematic search
                for (int u = 1; u <= min(n, 50); u++) {
                    for (int v = u + 2; v <= min(n, 50) && v <= u + n/2; v++) {
                        if (abs(v - u) == 1 || abs(v - u) == n - 1) continue;
                        
                        int dist_uv = query(u, v);
                        int cycle_dist = min(abs(v - u), n - abs(v - u));
                        
                        if (dist_uv == 1) {
                            answer(u, v);
                            goto next_test;
                        }
                    }
                }
                break;
            }
        }
        
        // Fallback: systematic search with larger steps
        bool found = false;
        for (int u = 1; u <= min(100, n) && !found; u++) {
            for (int v = u + 2; v <= min(100, n) && !found; v++) {
                if (abs(v - u) == 1 || abs(v - u) == n - 1) continue;
                if (abs(v - u) >= n - 1) continue;
                
                int dist_uv = query(u, v);
                if (dist_uv == 1) {
                    answer(u, v);
                    found = true;
                    break;
                }
            }
        }
        
        next_test:;
    }
    
    return 0;
}
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
    int r;
    cin >> r;
}

int main() {
    int T;
    cin >> T;
    
    while (T--) {
        int n;
        cin >> n;
        
        // Find one endpoint of the chord using binary search
        int left = 1, right = n;
        int chord_end1 = -1;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            int dist = query(1, mid);
            int expected = min(mid - 1, n - (mid - 1));
            
            if (dist < expected) {
                chord_end1 = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        if (chord_end1 == -1) {
            // Try the other direction
            left = 1;
            right = n;
            while (left <= right) {
                int mid = (left + right) / 2;
                int dist = query(mid, 1);
                int expected = min(mid - 1, n - (mid - 1));
                
                if (dist < expected) {
                    chord_end1 = mid;
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
        }
        
        // Now find the other endpoint
        // Check distances from chord_end1 to other vertices
        vector<int> candidates;
        
        // Sample some vertices around the cycle
        for (int offset = 3; offset <= min(n/2, 20); offset++) {
            int v1 = (chord_end1 + offset - 1) % n + 1;
            int v2 = (chord_end1 - offset + n - 1) % n + 1;
            
            int dist1 = query(chord_end1, v1);
            int expected1 = offset;
            if (dist1 == 1) {
                candidates.push_back(v1);
                break;
            }
            
            int dist2 = query(chord_end1, v2);
            int expected2 = offset;
            if (dist2 == 1) {
                candidates.push_back(v2);
                break;
            }
        }
        
        if (candidates.empty()) {
            // Binary search for the other endpoint
            left = 1;
            right = n;
            
            while (left <= right) {
                int mid = (left + right) / 2;
                if (mid == chord_end1) {
                    if (left == right) break;
                    if (mid == left) left++;
                    else right--;
                    continue;
                }
                
                int dist = query(chord_end1, mid);
                int cycle_dist = min(abs(mid - chord_end1), n - abs(mid - chord_end1));
                
                if (dist == 1) {
                    candidates.push_back(mid);
                    break;
                } else if (dist < cycle_dist) {
                    // There's a shortcut, search around this area
                    for (int delta = -2; delta <= 2; delta++) {
                        int test = mid + delta;
                        if (test < 1) test += n;
                        if (test > n) test -= n;
                        if (test == chord_end1) continue;
                        
                        int test_dist = query(chord_end1, test);
                        if (test_dist == 1) {
                            candidates.push_back(test);
                            goto found;
                        }
                    }
                    left = mid + 1;
                } else {
                    left = mid + 1;
                }
            }
            found:;
        }
        
        if (!candidates.empty()) {
            answer(chord_end1, candidates[0]);
        } else {
            // Fallback: try systematic search
            for (int i = 1; i <= n; i++) {
                if (i == chord_end1) continue;
                int dist = query(chord_end1, i);
                if (dist == 1) {
                    answer(chord_end1, i);
                    break;
                }
            }
        }
    }
    
    return 0;
}
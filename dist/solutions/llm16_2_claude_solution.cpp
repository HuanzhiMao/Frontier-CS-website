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
        
        // Find one endpoint of the chord
        int left = 1, right = n/2;
        int chord_end1 = -1;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            int dist = query(1, mid + 1);
            
            if (dist < mid) {
                // Chord affects this path
                chord_end1 = mid + 1;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        if (chord_end1 == -1) {
            // Check the other half
            left = n/2 + 1;
            right = n - 1;
            
            while (left <= right) {
                int mid = (left + right) / 2;
                int dist = query(1, mid + 1);
                int expected = (mid < n/2) ? mid : n - mid;
                
                if (dist < expected) {
                    chord_end1 = mid + 1;
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
        }
        
        // Find the other endpoint
        int chord_end2 = -1;
        
        // Binary search around the cycle to find the other end
        left = 1;
        right = n;
        
        while (left <= right && left != chord_end1 && right != chord_end1) {
            int mid = (left + right) / 2;
            if (mid == chord_end1) {
                if (mid == left) left++;
                else right--;
                continue;
            }
            
            int dist = query(chord_end1, mid);
            int cycle_dist1 = abs(chord_end1 - mid);
            int cycle_dist2 = n - cycle_dist1;
            int expected_cycle_dist = min(cycle_dist1, cycle_dist2);
            
            if (dist == 1) {
                chord_end2 = mid;
                break;
            } else if (dist < expected_cycle_dist) {
                // This point is affected by the chord
                chord_end2 = mid;
                break;
            } else {
                // Try other points
                if (mid < chord_end1) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        // If not found, try a different approach
        if (chord_end2 == -1) {
            for (int i = 1; i <= n && chord_end2 == -1; i++) {
                if (i == chord_end1) continue;
                if (abs(i - chord_end1) <= 1 || abs(i - chord_end1) >= n - 1) continue;
                
                int dist = query(chord_end1, i);
                if (dist == 1) {
                    chord_end2 = i;
                    break;
                }
            }
        }
        
        answer(chord_end1, chord_end2);
    }
    
    return 0;
}
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
                chord_end1 = mid + 1;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        if (chord_end1 == -1) {
            left = n/2 + 1;
            right = n - 1;
            
            while (left <= right) {
                int mid = (left + right) / 2;
                int dist = query(1, mid + 1);
                
                if (dist < n - mid) {
                    chord_end1 = mid + 1;
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        // Find the other endpoint
        int chord_end2 = -1;
        
        // Binary search around chord_end1 to find the other end
        left = 1;
        right = n;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            if (mid == chord_end1) {
                if (left == right) break;
                if (mid == left) mid = right;
                else mid = left;
            }
            
            int dist = query(chord_end1, mid);
            int expected_dist = min(abs(chord_end1 - mid), n - abs(chord_end1 - mid));
            
            if (dist == 1) {
                chord_end2 = mid;
                break;
            }
            
            if (dist < expected_dist) {
                // There's a shortcut, search in this direction
                if (mid < chord_end1) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } else {
                // No shortcut in this direction
                if (mid < chord_end1) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        // If we haven't found it yet, try a different approach
        if (chord_end2 == -1) {
            for (int i = 1; i <= n; i++) {
                if (i != chord_end1) {
                    int dist = query(chord_end1, i);
                    if (dist == 1) {
                        chord_end2 = i;
                        break;
                    }
                }
            }
        }
        
        answer(chord_end1, chord_end2);
    }
    
    return 0;
}
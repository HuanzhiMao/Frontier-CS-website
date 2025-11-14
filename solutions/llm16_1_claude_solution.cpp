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
        int left = 1, right = n/2 + 1;
        int chord_endpoint = -1;
        
        while (left < right) {
            int mid = (left + right) / 2;
            int dist = query(1, mid);
            
            if (dist < mid - 1) {
                // Chord affects this path
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        // Now we need to find which vertex in the range creates the shortcut
        int start = left;
        for (int i = max(1, start - 2); i <= min(n, start + 2); i++) {
            if (i == 1) continue;
            int dist = query(1, i);
            int expected = min(i - 1, n - i + 1);
            if (dist < expected) {
                chord_endpoint = i;
                break;
            }
        }
        
        if (chord_endpoint == -1) {
            chord_endpoint = start;
        }
        
        // Find the other endpoint
        int other_endpoint = -1;
        
        // Check vertices around the opposite side of the cycle
        int opposite = (chord_endpoint + n/2 - 1) % n + 1;
        
        for (int offset = -3; offset <= 3; offset++) {
            int candidate = ((opposite - 1 + offset + n) % n) + 1;
            if (candidate == chord_endpoint) continue;
            
            int dist = query(chord_endpoint, candidate);
            int cycle_dist = min(abs(candidate - chord_endpoint), n - abs(candidate - chord_endpoint));
            
            if (dist == 1 && cycle_dist > 2) {
                other_endpoint = candidate;
                break;
            }
        }
        
        if (other_endpoint == -1) {
            // Try binary search on the other half
            left = n/2 + 1;
            right = n;
            
            while (left < right) {
                int mid = (left + right) / 2;
                int dist = query(1, mid);
                int expected = n - mid + 1;
                
                if (dist < expected) {
                    right = mid;
                } else {
                    left = mid + 1;
                }
            }
            other_endpoint = left;
        }
        
        answer(chord_endpoint, other_endpoint);
    }
    
    return 0;
}
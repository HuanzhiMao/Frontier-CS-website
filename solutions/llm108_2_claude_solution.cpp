#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<int> pos(n, 0);
    
    // For each ring i > 0, find its relative position to ring 0
    for (int i = 1; i < n; i++) {
        int best_pos = 0;
        int max_count = -1;
        
        // Try all possible relative positions
        for (int offset = 0; offset < n * m; offset++) {
            // Move ring i to position offset relative to ring 0
            int moves = (offset - pos[i] + n * m) % (n * m);
            if (moves > n * m / 2) {
                moves = moves - n * m;
            }
            
            // Perform the moves
            while (moves > 0) {
                cout << "? " << i << " 1" << endl;
                cout.flush();
                int response;
                cin >> response;
                pos[i] = (pos[i] + 1) % (n * m);
                moves--;
            }
            while (moves < 0) {
                cout << "? " << i << " -1" << endl;
                cout.flush();
                int response;
                cin >> response;
                pos[i] = (pos[i] - 1 + n * m) % (n * m);
                moves++;
            }
            
            // Get current count
            cout << "? " << 0 << " 1" << endl;
            cout.flush();
            int count1;
            cin >> count1;
            pos[0] = (pos[0] + 1) % (n * m);
            
            cout << "? " << 0 << " -1" << endl;
            cout.flush();
            int count2;
            cin >> count2;
            pos[0] = (pos[0] - 1 + n * m) % (n * m);
            
            // Check if rings are aligned by rotating ring 0
            for (int j = 0; j < n * m; j++) {
                cout << "? " << 0 << " 1" << endl;
                cout.flush();
                int curr_count;
                cin >> curr_count;
                pos[0] = (pos[0] + 1) % (n * m);
                
                if (curr_count < count2) {
                    // Found alignment
                    best_pos = (pos[0] - pos[i] + n * m) % (n * m);
                    goto next_ring;
                }
            }
        }
        
        next_ring:;
    }
    
    // Calculate final relative positions
    vector<int> result(n - 1);
    for (int i = 1; i < n; i++) {
        result[i - 1] = (pos[i] - pos[0] + n * m) % (n * m);
    }
    
    cout << "!";
    for (int i = 0; i < n - 1; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    int total = n * m;
    vector<int> pos(n, 0); // current positions of rings
    
    // Get initial count
    cout << "? 0 1" << endl;
    cout.flush();
    int initial;
    cin >> initial;
    pos[0] = 1;
    
    cout << "? 0 -1" << endl;
    cout.flush();
    cin >> initial;
    pos[0] = 0;
    
    vector<int> result(n-1);
    
    for (int i = 1; i < n; i++) {
        int best_overlap = -1;
        int best_pos = -1;
        
        // Try all positions by rotating ring i
        for (int rot = 0; rot < total; rot++) {
            cout << "? " << i << " 1" << endl;
            cout.flush();
            int count;
            cin >> count;
            pos[i] = (pos[i] + 1) % total;
            
            // Calculate how many sections ring 0 and ring i share
            int shared = 0;
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < m; k++) {
                    if ((pos[0] + j) % total == (pos[i] + k) % total) {
                        shared++;
                    }
                }
            }
            
            if (shared > best_overlap) {
                best_overlap = shared;
                best_pos = rot;
            }
            
            if (shared == m) {
                // Perfect alignment found
                result[i-1] = (pos[i] - pos[0] + total) % total;
                break;
            }
        }
        
        if (best_overlap < m) {
            // Find the position that maximizes overlap
            int current_pos = pos[i];
            int target_pos = (pos[0] + best_pos) % total;
            
            while (pos[i] != target_pos) {
                cout << "? " << i << " 1" << endl;
                cout.flush();
                int count;
                cin >> count;
                pos[i] = (pos[i] + 1) % total;
            }
            
            result[i-1] = (pos[i] - pos[0] + total) % total;
        }
    }
    
    cout << "!";
    for (int i = 0; i < n-1; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}
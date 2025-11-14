#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    int total = n * m;
    vector<int> position(n, 0);  // current position of each ring
    
    // Get initial count
    cout << "? 0 1" << endl;
    cout.flush();
    int initial;
    cin >> initial;
    position[0] = 1;
    
    // Restore ring 0
    cout << "? 0 -1" << endl;
    cout.flush();
    cin >> initial;
    position[0] = 0;
    
    vector<int> result(n - 1);
    
    for (int ring = 1; ring < n; ring++) {
        int best_pos = 0;
        int min_blocked = initial;
        
        // Try all positions for this ring
        for (int pos = 0; pos < total; pos++) {
            cout << "? " << ring << " 1" << endl;
            cout.flush();
            int blocked;
            cin >> blocked;
            position[ring] = (position[ring] + 1) % total;
            
            if (blocked < min_blocked) {
                min_blocked = blocked;
                best_pos = position[ring];
            }
        }
        
        // Calculate relative position
        result[ring - 1] = (best_pos - position[0] + total) % total;
    }
    
    cout << "!";
    for (int i = 0; i < n - 1; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}
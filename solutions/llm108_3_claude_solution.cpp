#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    int total = n * m;
    vector<int> pos(n, 0);  // current positions of each ring
    
    // Get initial count
    cout << "? 0 1" << endl;
    cout.flush();
    int initial;
    cin >> initial;
    pos[0] = 1;
    
    // Reset ring 0
    cout << "? 0 -1" << endl;
    cout.flush();
    cin >> initial;
    pos[0] = 0;
    
    vector<int> result(n-1);
    
    // For each ring i (1 to n-1), find when it aligns with ring 0
    for (int i = 1; i < n; i++) {
        int best_alignment = -1;
        int max_blocked = -1;
        
        // Try all possible positions for ring i
        for (int shift = 0; shift < total; shift++) {
            cout << "? " << i << " 1" << endl;
            cout.flush();
            int count;
            cin >> count;
            pos[i] = (pos[i] + 1) % total;
            
            // Check if this creates maximum overlap (minimum unblocked)
            if (max_blocked == -1 || count < max_blocked) {
                max_blocked = count;
                best_alignment = shift + 1;
            }
        }
        
        // The relative position is how much we need to rotate ring 0 to align with ring i
        result[i-1] = (pos[i] - pos[0] + total) % total;
    }
    
    cout << "!";
    for (int i = 0; i < n-1; i++) {
        cout << " " << result[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}
#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    vector<int> pair_of(2*N + 1, -1);
    set<int> in_device;
    
    for (int i = 1; i <= 2*N; i++) {
        if (pair_of[i] != -1) continue;
        
        // Insert slice i
        cout << "? " << i << endl;
        cout.flush();
        int r1;
        cin >> r1;
        in_device.insert(i);
        
        // Try each remaining slice to find its pair
        for (int j = i + 1; j <= 2*N; j++) {
            if (pair_of[j] != -1) continue;
            
            // Insert slice j
            cout << "? " << j << endl;
            cout.flush();
            int r2;
            cin >> r2;
            
            if (r2 == r1) {
                // j is the pair of i
                pair_of[i] = j;
                pair_of[j] = i;
                
                // Remove j from device
                cout << "? " << j << endl;
                cout.flush();
                int r3;
                cin >> r3;
                
                break;
            } else {
                // j is not the pair of i, remove it
                cout << "? " << j << endl;
                cout.flush();
                int r3;
                cin >> r3;
            }
        }
        
        // Remove i from device
        cout << "? " << i << endl;
        cout.flush();
        int r4;
        cin >> r4;
        in_device.erase(i);
    }
    
    // Output all pairs
    for (int i = 1; i <= 2*N; i++) {
        if (pair_of[i] > i) {
            cout << "! " << i << " " << pair_of[i] << endl;
            cout.flush();
        }
    }
    
    return 0;
}
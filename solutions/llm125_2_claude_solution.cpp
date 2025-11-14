#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    vector<int> pairs(2*N + 1, -1);
    set<int> in_device;
    
    for (int i = 1; i <= 2*N; i++) {
        if (pairs[i] != -1) continue;
        
        // Add slice i to device
        cout << "? " << i << endl;
        cout.flush();
        int r1;
        cin >> r1;
        in_device.insert(i);
        
        // Try each remaining slice to find its pair
        for (int j = i + 1; j <= 2*N; j++) {
            if (pairs[j] != -1) continue;
            
            // Add slice j to device
            cout << "? " << j << endl;
            cout.flush();
            int r2;
            cin >> r2;
            
            if (r2 == r1) {
                // j is the pair of i (same kind, so count didn't increase)
                pairs[i] = j;
                pairs[j] = i;
                cout << "! " << i << " " << j << endl;
                cout.flush();
                
                // Remove both from device
                cout << "? " << i << endl;
                cout.flush();
                int dummy1;
                cin >> dummy1;
                cout << "? " << j << endl;
                cout.flush();
                int dummy2;
                cin >> dummy2;
                
                in_device.clear();
                break;
            } else {
                // j is not the pair of i, remove j from device
                cout << "? " << j << endl;
                cout.flush();
                int dummy;
                cin >> dummy;
            }
        }
    }
    
    return 0;
}
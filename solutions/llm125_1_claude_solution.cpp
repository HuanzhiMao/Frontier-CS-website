#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    vector<int> partner(2*N + 1, -1);
    set<int> in_device;
    
    for (int i = 1; i <= 2*N; i++) {
        if (partner[i] != -1) continue;
        
        // Insert slice i
        cout << "? " << i << endl;
        cout.flush();
        int r1;
        cin >> r1;
        in_device.insert(i);
        
        // Try each remaining slice to find its partner
        for (int j = i + 1; j <= 2*N; j++) {
            if (partner[j] != -1) continue;
            
            // Insert slice j
            cout << "? " << j << endl;
            cout.flush();
            int r2;
            cin >> r2;
            
            if (r2 == r1) {
                // j is partner of i
                partner[i] = j;
                partner[j] = i;
                cout << "! " << i << " " << j << endl;
                cout.flush();
                
                // Remove j from device
                cout << "? " << j << endl;
                cout.flush();
                int r3;
                cin >> r3;
                break;
            } else {
                // j is not partner of i, remove j from device
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
    
    return 0;
}
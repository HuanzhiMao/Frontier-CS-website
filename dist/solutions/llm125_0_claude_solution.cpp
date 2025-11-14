#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    vector<bool> used(2*N + 1, false);
    vector<pair<int, int>> pairs;
    set<int> in_device;
    
    for (int i = 1; i <= 2*N; i++) {
        if (used[i]) continue;
        
        // Add element i to device
        cout << "? " << i << endl;
        cout.flush();
        int r1;
        cin >> r1;
        in_device.insert(i);
        
        // Try to find its pair
        for (int j = i + 1; j <= 2*N; j++) {
            if (used[j]) continue;
            
            // Add element j to device
            cout << "? " << j << endl;
            cout.flush();
            int r2;
            cin >> r2;
            
            if (r2 == r1) {
                // j is the pair of i (same mineral type)
                pairs.push_back({i, j});
                used[i] = used[j] = true;
                
                // Remove j from device
                cout << "? " << j << endl;
                cout.flush();
                int r3;
                cin >> r3;
                
                break;
            } else {
                // j is not the pair of i, remove j from device
                cout << "? " << j << endl;
                cout.flush();
                int r3;
                cin >> r3;
            }
        }
        
        // Remove i from device for next iteration
        if (!used[i]) {
            cout << "? " << i << endl;
            cout.flush();
            int r;
            cin >> r;
            in_device.erase(i);
        } else {
            cout << "? " << i << endl;
            cout.flush();
            int r;
            cin >> r;
            in_device.erase(i);
        }
    }
    
    // Output all pairs
    for (auto& p : pairs) {
        cout << "! " << p.first << " " << p.second << endl;
        cout.flush();
    }
    
    return 0;
}
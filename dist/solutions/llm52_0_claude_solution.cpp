#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n, l1, l2;
    cin >> n >> l1 >> l2;
    
    vector<int> p(n + 1);
    vector<bool> known(n + 1, false);
    
    // First, try to identify elements by querying single positions
    for (int i = 1; i <= n; i++) {
        cout << "1 " << i << " " << i << endl;
        cout.flush();
        int x;
        cin >> x;
        // Single element is always a contiguous segment, so x = 1
        // This doesn't give us direct information about the value
    }
    
    // Try to find patterns by querying pairs and small ranges
    vector<vector<int>> segments(n + 1);
    
    for (int len = 2; len <= n && len <= 5; len++) {
        for (int i = 1; i + len - 1 <= n; i++) {
            cout << "1 " << i << " " << i + len - 1 << endl;
            cout.flush();
            int x;
            cin >> x;
            segments[i].push_back(x);
        }
    }
    
    // Use a simple heuristic approach
    // Query the entire array to get total segments
    cout << "1 1 " << n << endl;
    cout.flush();
    int total_segments;
    cin >> total_segments;
    
    // Initialize with a guess
    for (int i = 1; i <= n; i++) {
        p[i] = i;
    }
    
    // Try some swaps to see if we can improve
    int swaps_used = 0;
    for (int i = 1; i <= n - 1 && swaps_used < l2; i++) {
        // Query current state
        cout << "1 1 " << n << endl;
        cout.flush();
        int current_segments;
        cin >> current_segments;
        
        // Try swapping adjacent elements
        cout << "2 " << i << " " << i + 1 << endl;
        cout.flush();
        int confirm;
        cin >> confirm;
        swaps_used++;
        
        swap(p[i], p[i + 1]);
        
        // Check if this improves
        cout << "1 1 " << n << endl;
        cout.flush();
        int new_segments;
        cin >> new_segments;
        
        if (new_segments < current_segments && swaps_used < l2) {
            // This was good, keep it
        } else if (swaps_used < l2) {
            // Swap back
            cout << "2 " << i << " " << i + 1 << endl;
            cout.flush();
            cin >> confirm;
            swaps_used++;
            swap(p[i], p[i + 1]);
        }
    }
    
    cout << "3";
    for (int i = 1; i <= n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
    cout.flush();
    
    return 0;
}
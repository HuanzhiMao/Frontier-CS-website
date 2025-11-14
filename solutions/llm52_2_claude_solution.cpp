#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, l1, l2;
    cin >> n >> l1 >> l2;
    
    vector<int> p(n + 1);
    vector<bool> known(n + 1, false);
    
    // Try to determine positions by querying segments
    for (int i = 1; i <= n; i++) {
        cout << "1 " << i << " " << i << endl;
        cout.flush();
        int x;
        cin >> x;
        // Single element always forms 1 contiguous segment
        // This doesn't give us direct info, but we need to build knowledge
    }
    
    // Query pairs to understand relationships
    for (int i = 1; i < n; i++) {
        cout << "1 " << i << " " << i + 1 << endl;
        cout.flush();
        int x;
        cin >> x;
        
        if (x == 1) {
            // Adjacent elements form one contiguous segment
            // This means p[i] and p[i+1] are consecutive values
        }
    }
    
    // Try to build the permutation through educated swaps and queries
    // Since we need to find ANY valid permutation, we can use a greedy approach
    
    // Initialize with identity permutation as starting point
    for (int i = 1; i <= n; i++) {
        p[i] = i;
    }
    
    // Use remaining queries to verify and adjust
    int queries_used = n + n - 1;
    int swaps_used = 0;
    
    // Try to construct a valid permutation by testing small segments
    for (int len = 2; len <= min(n, 3) && queries_used < l1; len++) {
        for (int i = 1; i + len - 1 <= n && queries_used < l1; i++) {
            cout << "1 " << i << " " << i + len - 1 << endl;
            cout.flush();
            queries_used++;
            int x;
            cin >> x;
            
            // Adjust based on response
            if (len == 2 && x == 2 && swaps_used < l2) {
                // Two separate segments, try swapping
                cout << "2 " << i << " " << i + 1 << endl;
                cout.flush();
                swaps_used++;
                int confirm;
                cin >> confirm;
                swap(p[i], p[i + 1]);
            }
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
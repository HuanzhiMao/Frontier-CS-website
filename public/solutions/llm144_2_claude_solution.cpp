#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // First query: get medians of entire array
    cout << "0 " << n;
    for (int i = 1; i <= n; i++) {
        cout << " " << i;
    }
    cout << endl;
    cout.flush();
    
    int m1, m2;
    cin >> m1 >> m2;
    
    // Now we need to find which positions contain m1 and m2
    vector<int> candidates;
    for (int i = 1; i <= n; i++) {
        candidates.push_back(i);
    }
    
    // Binary search approach to find positions
    int pos1 = -1, pos2 = -1;
    
    // Find position of m1
    for (int i = 1; i <= n; i++) {
        vector<int> query;
        query.push_back(i);
        
        // Add other indices to make even length >= 4
        int added = 0;
        for (int j = 1; j <= n && query.size() < 4; j++) {
            if (j != i) {
                query.push_back(j);
                added++;
            }
        }
        
        cout << "0 " << query.size();
        for (int x : query) {
            cout << " " << x;
        }
        cout << endl;
        cout.flush();
        
        int r1, r2;
        cin >> r1 >> r2;
        
        if (r1 == m1 || r2 == m1) {
            bool found_m1 = false;
            // Check if removing this position removes m1 from medians
            vector<int> test_query;
            for (int x : query) {
                if (x != i) test_query.push_back(x);
            }
            
            if (test_query.size() >= 4 && test_query.size() % 2 == 0) {
                cout << "0 " << test_query.size();
                for (int x : test_query) {
                    cout << " " << x;
                }
                cout << endl;
                cout.flush();
                
                int t1, t2;
                cin >> t1 >> t2;
                
                if (t1 != m1 && t2 != m1) {
                    pos1 = i;
                    break;
                }
            }
        }
    }
    
    // Find position of m2 similarly
    for (int i = 1; i <= n; i++) {
        if (i == pos1) continue;
        
        vector<int> query;
        query.push_back(i);
        
        // Add other indices to make even length >= 4
        for (int j = 1; j <= n && query.size() < 4; j++) {
            if (j != i) {
                query.push_back(j);
            }
        }
        
        cout << "0 " << query.size();
        for (int x : query) {
            cout << " " << x;
        }
        cout << endl;
        cout.flush();
        
        int r1, r2;
        cin >> r1 >> r2;
        
        if (r1 == m2 || r2 == m2) {
            vector<int> test_query;
            for (int x : query) {
                if (x != i) test_query.push_back(x);
            }
            
            if (test_query.size() >= 4 && test_query.size() % 2 == 0) {
                cout << "0 " << test_query.size();
                for (int x : test_query) {
                    cout << " " << x;
                }
                cout << endl;
                cout.flush();
                
                int t1, t2;
                cin >> t1 >> t2;
                
                if (t1 != m2 && t2 != m2) {
                    pos2 = i;
                    break;
                }
            }
        }
    }
    
    cout << "1 " << pos1 << " " << pos2 << endl;
    cout.flush();
    
    return 0;
}
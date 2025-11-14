#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // Query all elements to get the two medians of the entire permutation
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
    
    int pos1 = -1, pos2 = -1;
    
    // Binary search approach to find positions
    while (candidates.size() > 2) {
        int mid = candidates.size() / 2;
        vector<int> left, right;
        
        for (int i = 0; i < mid; i++) {
            left.push_back(candidates[i]);
        }
        for (int i = mid; i < candidates.size(); i++) {
            right.push_back(candidates[i]);
        }
        
        // Make left even size if needed
        while (left.size() < 4 || left.size() % 2 != 0) {
            if (right.size() > 0) {
                left.push_back(right[0]);
                right.erase(right.begin());
            }
        }
        
        if (left.size() >= 4) {
            cout << "0 " << left.size();
            for (int x : left) {
                cout << " " << x;
            }
            cout << endl;
            cout.flush();
            
            int q1, q2;
            cin >> q1 >> q2;
            
            if ((q1 == m1 && q2 == m2) || (q1 == m1 || q2 == m1 || q1 == m2 || q2 == m2)) {
                candidates = left;
            } else {
                candidates = right;
            }
        } else {
            break;
        }
    }
    
    // Try different combinations to narrow down
    for (int i = 1; i <= n && (pos1 == -1 || pos2 == -1); i++) {
        for (int j = i + 1; j <= n && (pos1 == -1 || pos2 == -1); j++) {
            vector<int> test = {i, j};
            
            // Add more elements to make it even and >= 4
            for (int k = 1; k <= n && test.size() < 4; k++) {
                if (k != i && k != j) {
                    test.push_back(k);
                }
            }
            
            if (test.size() >= 4 && test.size() % 2 == 0) {
                cout << "0 " << test.size();
                for (int x : test) {
                    cout << " " << x;
                }
                cout << endl;
                cout.flush();
                
                int q1, q2;
                cin >> q1 >> q2;
                
                if (q1 == m1 && q2 == m2) {
                    pos1 = i;
                    pos2 = j;
                    break;
                }
            }
        }
    }
    
    if (pos1 == -1 || pos2 == -1) {
        // Fallback: try systematic approach
        for (int i = 1; i <= n; i++) {
            for (int j = i + 1; j <= n; j++) {
                // Create a query with positions i, j and two other positions
                vector<int> others;
                for (int k = 1; k <= n; k++) {
                    if (k != i && k != j) {
                        others.push_back(k);
                    }
                }
                
                if (others.size() >= 2) {
                    vector<int> query = {i, j, others[0], others[1]};
                    
                    cout << "0 4";
                    for (int x : query) {
                        cout << " " << x;
                    }
                    cout << endl;
                    cout.flush();
                    
                    int q1, q2;
                    cin >> q1 >> q2;
                    
                    if (q1 == m1 && q2 == m2) {
                        pos1 = i;
                        pos2 = j;
                        goto found;
                    }
                }
            }
        }
    }
    
    found:
    cout << "1 " << pos1 << " " << pos2 << endl;
    cout.flush();
    
    return 0;
}
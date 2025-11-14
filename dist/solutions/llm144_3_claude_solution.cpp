#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    // First query all positions to get global medians
    cout << "0 " << n;
    for (int i = 1; i <= n; i++) {
        cout << " " << i;
    }
    cout << endl;
    cout.flush();
    
    int m1, m2;
    cin >> m1 >> m2;
    
    // Find positions of m1 and m2
    vector<int> candidates;
    for (int i = 1; i <= n; i++) {
        candidates.push_back(i);
    }
    
    // Binary search approach to find positions
    int pos1 = -1, pos2 = -1;
    
    // For each candidate position, check if removing it changes the medians
    for (int pos : candidates) {
        vector<int> test_pos;
        for (int i = 1; i <= n; i++) {
            if (i != pos) {
                test_pos.push_back(i);
            }
        }
        
        if (test_pos.size() >= 4 && test_pos.size() % 2 == 0) {
            cout << "0 " << test_pos.size();
            for (int x : test_pos) {
                cout << " " << x;
            }
            cout << endl;
            cout.flush();
            
            int tm1, tm2;
            cin >> tm1 >> tm2;
            
            if (tm1 != m1 || tm2 != m2) {
                if (pos1 == -1) {
                    pos1 = pos;
                } else {
                    pos2 = pos;
                    break;
                }
            }
        }
    }
    
    // If we haven't found both positions, use a different strategy
    if (pos1 == -1 || pos2 == -1) {
        // Try smaller subsets to isolate the median positions
        for (int i = 1; i <= n && (pos1 == -1 || pos2 == -1); i++) {
            for (int j = i + 1; j <= n && (pos1 == -1 || pos2 == -1); j++) {
                vector<int> others;
                for (int k = 1; k <= n; k++) {
                    if (k != i && k != j) {
                        others.push_back(k);
                    }
                }
                
                if (others.size() >= 2 && (others.size() + 2) % 2 == 0) {
                    others.push_back(i);
                    others.push_back(j);
                    
                    cout << "0 " << others.size();
                    for (int x : others) {
                        cout << " " << x;
                    }
                    cout << endl;
                    cout.flush();
                    
                    int tm1, tm2;
                    cin >> tm1 >> tm2;
                    
                    if (tm1 == m1 && tm2 == m2) {
                        pos1 = i;
                        pos2 = j;
                        break;
                    }
                }
            }
        }
    }
    
    cout << "1 " << pos1 << " " << pos2 << endl;
    cout.flush();
    
    return 0;
}
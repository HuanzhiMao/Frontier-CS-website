#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    cin >> n >> k;
    
    set<int> distinct_types;
    map<int, int> type_map;
    int next_type = 1;
    
    // Query all bakeries once to get initial mapping
    for (int i = 1; i <= n; i++) {
        cout << "? " << i << endl;
        cout.flush();
        
        char response;
        cin >> response;
        
        if (response == 'N') {
            type_map[i] = next_type;
            distinct_types.insert(next_type);
            next_type++;
        } else {
            // Find which type this bakery has by checking recent queries
            // This is tricky in the first pass, so we'll use a different approach
        }
    }
    
    // Reset and use a more systematic approach
    cout << "R" << endl;
    cout.flush();
    
    vector<int> bakery_type(n + 1, 0);
    set<int> seen_types;
    int type_counter = 1;
    
    // Process bakeries in groups of size k
    for (int start = 1; start <= n; start += k) {
        if (start > 1) {
            cout << "R" << endl;
            cout.flush();
        }
        
        for (int i = start; i < min(start + k, n + 1); i++) {
            cout << "? " << i << endl;
            cout.flush();
            
            char response;
            cin >> response;
            
            if (response == 'N') {
                bakery_type[i] = type_counter++;
                seen_types.insert(bakery_type[i]);
            } else {
                // Find which previous bakery in current group has same type
                for (int j = start; j < i; j++) {
                    cout << "? " << j << endl;
                    cout.flush();
                    cin >> response;
                    
                    if (response == 'Y') {
                        bakery_type[i] = bakery_type[j];
                        break;
                    }
                }
            }
        }
    }
    
    cout << "! " << seen_types.size() << endl;
    cout.flush();
    
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, R;
    cin >> N >> R;
    
    vector<int> U(N), V(N);
    for (int i = 0; i < N; i++) {
        cin >> U[i] >> V[i];
    }
    
    string result(N, '?');
    
    // For each gate, we need to determine if it's AND or OR
    for (int gate = 0; gate < N; gate++) {
        // Test with different combinations of inputs to the gate
        string query(2*N+1, '0');
        
        // Set all switches N to 2N to ON initially
        for (int j = N; j <= 2*N; j++) {
            query[j] = '1';
        }
        
        // Test case 1: both inputs to gate are 1
        cout << "? " << query << endl;
        int out1;
        cin >> out1;
        
        // Test case 2: first input is 0, second is 1
        query[U[gate]] = '0';
        query[V[gate]] = '1';
        cout << "? " << query << endl;
        int out2;
        cin >> out2;
        
        // Test case 3: first input is 1, second is 0
        query[U[gate]] = '1';
        query[V[gate]] = '0';
        cout << "? " << query << endl;
        int out3;
        cin >> out3;
        
        // Test case 4: both inputs are 0
        query[U[gate]] = '0';
        query[V[gate]] = '0';
        cout << "? " << query << endl;
        int out4;
        cin >> out4;
        
        // Analyze the pattern
        // For AND gate: 1,1->1, 1,0->0, 0,1->0, 0,0->0
        // For OR gate:  1,1->1, 1,0->1, 0,1->1, 0,0->0
        
        // We need to trace back what the actual gate output would be
        // This is complex due to the switch inversion logic
        
        // Simplified approach: check if output changes when one input changes
        if (out2 == out3 && out2 != out4) {
            result[gate] = '|'; // OR gate behavior
        } else {
            result[gate] = '&'; // AND gate behavior
        }
    }
    
    cout << "! " << result << endl;
    
    return 0;
}
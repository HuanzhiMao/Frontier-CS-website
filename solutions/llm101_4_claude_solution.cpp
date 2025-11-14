#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, R;
    cin >> N >> R;
    
    vector<int> U(N), V(N);
    for (int i = 0; i < N; i++) {
        cin >> U[i] >> V[i];
    }
    
    string result(N, '?');
    
    // For each slot, we need to determine if it's AND or OR
    for (int slot = 0; slot < N; slot++) {
        int u = U[slot];
        int v = V[slot];
        
        // Test case 1: u=0, v=0
        string query1(2*N+1, '0');
        cout << "? " << query1 << endl;
        int output1;
        cin >> output1;
        
        // Test case 2: u=0, v=1
        string query2(2*N+1, '0');
        query2[v] = '1';
        cout << "? " << query2 << endl;
        int output2;
        cin >> output2;
        
        // Test case 3: u=1, v=0
        string query3(2*N+1, '0');
        query3[u] = '1';
        cout << "? " << query3 << endl;
        int output3;
        cin >> output3;
        
        // Test case 4: u=1, v=1
        string query4(2*N+1, '0');
        query4[u] = '1';
        query4[v] = '1';
        cout << "? " << query4 << endl;
        int output4;
        cin >> output4;
        
        // Analyze the outputs to determine gate type
        // For AND gate: 0&0=0, 0&1=0, 1&0=0, 1&1=1
        // For OR gate: 0|0=0, 0|1=1, 1|0=1, 1|1=1
        
        vector<int> outputs = {output1, output2, output3, output4};
        
        // Check if it matches AND pattern
        bool isAND = true;
        bool isOR = true;
        
        // We need to consider that the output might be inverted by switches 0 to slot-1
        // Try both possibilities (inverted and non-inverted)
        for (int inv = 0; inv < 2; inv++) {
            vector<int> expected_and = {0, 0, 0, 1};
            vector<int> expected_or = {0, 1, 1, 1};
            
            if (inv) {
                for (int& x : expected_and) x = 1 - x;
                for (int& x : expected_or) x = 1 - x;
            }
            
            bool matches_and = true, matches_or = true;
            for (int i = 0; i < 4; i++) {
                if (outputs[i] != expected_and[i]) matches_and = false;
                if (outputs[i] != expected_or[i]) matches_or = false;
            }
            
            if (matches_and && !matches_or) {
                result[slot] = '&';
                break;
            }
            if (matches_or && !matches_and) {
                result[slot] = '|';
                break;
            }
        }
        
        // If we couldn't determine from the pattern, use heuristic
        if (result[slot] == '?') {
            // If output2 or output3 is 1, more likely to be OR
            if (output2 == 1 || output3 == 1) {
                result[slot] = '|';
            } else {
                result[slot] = '&';
            }
        }
    }
    
    cout << "! " << result << endl;
    
    return 0;
}
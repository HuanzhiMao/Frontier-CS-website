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
    
    // For each slot, determine if it's AND or OR
    for (int slot = 0; slot < N; slot++) {
        // Test with all switches OFF except the ones connected to this slot
        string query1(2*N + 1, '0');
        query1[U[slot]] = '1';
        query1[V[slot]] = '0';
        cout << "? " << query1 << endl;
        int out1;
        cin >> out1;
        
        string query2(2*N + 1, '0');
        query2[U[slot]] = '0';
        query2[V[slot]] = '1';
        cout << "? " << query2 << endl;
        int out2;
        cin >> out2;
        
        string query3(2*N + 1, '0');
        query3[U[slot]] = '1';
        query3[V[slot]] = '1';
        cout << "? " << query3 << endl;
        int out3;
        cin >> out3;
        
        string query4(2*N + 1, '0');
        query4[U[slot]] = '0';
        query4[V[slot]] = '0';
        cout << "? " << query4 << endl;
        int out4;
        cin >> out4;
        
        // Analyze the outputs to determine gate type
        // For AND gate: 00->0, 01->0, 10->0, 11->1
        // For OR gate: 00->0, 01->1, 10->1, 11->1
        
        if (out1 == 0 && out2 == 0 && out3 == 1 && out4 == 0) {
            result[slot] = '&';
        } else if (out1 == 1 && out2 == 1 && out3 == 1 && out4 == 0) {
            result[slot] = '|';
        } else {
            // Need to consider the effect of other gates and switches
            // Try a simpler approach: test specific patterns
            if (out3 == 1 && (out1 == 0 || out2 == 0)) {
                result[slot] = '&';
            } else {
                result[slot] = '|';
            }
        }
    }
    
    cout << "! " << result << endl;
    
    return 0;
}
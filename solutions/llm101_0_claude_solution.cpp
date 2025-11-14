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
        // Test with both inputs 0
        string query1(2*N+1, '0');
        query1[U[slot]] = '0';
        query1[V[slot]] = '0';
        cout << "? " << query1 << endl;
        int out00;
        cin >> out00;
        
        // Test with first input 1, second input 0
        string query2(2*N+1, '0');
        query2[U[slot]] = '1';
        query2[V[slot]] = '0';
        cout << "? " << query2 << endl;
        int out10;
        cin >> out10;
        
        // Test with first input 0, second input 1
        string query3(2*N+1, '0');
        query3[U[slot]] = '0';
        query3[V[slot]] = '1';
        cout << "? " << query3 << endl;
        int out01;
        cin >> out01;
        
        // Test with both inputs 1
        string query4(2*N+1, '0');
        query4[U[slot]] = '1';
        query4[V[slot]] = '1';
        cout << "? " << query4 << endl;
        int out11;
        cin >> out11;
        
        // Analyze the truth table
        // AND: 00->0, 01->0, 10->0, 11->1
        // OR:  00->0, 01->1, 10->1, 11->1
        
        if (out00 == 0 && out01 == 0 && out10 == 0 && out11 == 1) {
            result[slot] = '&';
        } else if (out00 == 0 && out01 == 1 && out10 == 1 && out11 == 1) {
            result[slot] = '|';
        } else {
            // Need to consider the effect of other switches
            // Try a different approach - use minimal queries
            string test1(2*N+1, '1');
            test1[U[slot]] = '0';
            test1[V[slot]] = '0';
            cout << "? " << test1 << endl;
            int t1;
            cin >> t1;
            
            string test2(2*N+1, '1');
            test2[U[slot]] = '1';
            test2[V[slot]] = '1';
            cout << "? " << test2 << endl;
            int t2;
            cin >> t2;
            
            if (t1 == t2) {
                // Both give same result, try different combination
                string test3(2*N+1, '1');
                test3[U[slot]] = '1';
                test3[V[slot]] = '0';
                cout << "? " << test3 << endl;
                int t3;
                cin >> t3;
                
                if (t3 == t1) {
                    result[slot] = '|';
                } else {
                    result[slot] = '&';
                }
            } else {
                // Different results
                if (t1 < t2) {
                    result[slot] = '|';
                } else {
                    result[slot] = '&';
                }
            }
        }
    }
    
    cout << "! " << result << endl;
    
    return 0;
}
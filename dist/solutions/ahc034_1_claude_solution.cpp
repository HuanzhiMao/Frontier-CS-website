#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<vector<int>> h(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> h[i][j];
        }
    }
    
    vector<string> operations;
    int load = 0;
    int r = 0, c = 0;
    
    // Simple greedy approach: traverse grid and balance heights
    for (int i = 0; i < N; i++) {
        // Move to start of row
        while (r > i) {
            operations.push_back("U");
            r--;
        }
        while (r < i) {
            operations.push_back("D");
            r++;
        }
        while (c > 0) {
            operations.push_back("L");
            c--;
        }
        
        // Traverse the row
        for (int j = 0; j < N; j++) {
            // Move to column j
            while (c < j) {
                operations.push_back("R");
                c++;
            }
            
            // Handle current cell
            if (h[i][j] > 0) {
                // Load excess soil
                operations.push_back("+" + to_string(h[i][j]));
                load += h[i][j];
                h[i][j] = 0;
            } else if (h[i][j] < 0 && load > 0) {
                // Unload soil to fill deficit
                int unload_amount = min(load, -h[i][j]);
                operations.push_back("-" + to_string(unload_amount));
                load -= unload_amount;
                h[i][j] += unload_amount;
            }
        }
    }
    
    // Second pass to handle remaining deficits
    for (int i = 0; i < N; i++) {
        // Move to start of row
        while (r > i) {
            operations.push_back("U");
            r--;
        }
        while (r < i) {
            operations.push_back("D");
            r++;
        }
        while (c > 0) {
            operations.push_back("L");
            c--;
        }
        
        for (int j = 0; j < N; j++) {
            // Move to column j
            while (c < j) {
                operations.push_back("R");
                c++;
            }
            
            // Handle remaining deficit
            if (h[i][j] < 0 && load > 0) {
                int unload_amount = min(load, -h[i][j]);
                operations.push_back("-" + to_string(unload_amount));
                load -= unload_amount;
                h[i][j] += unload_amount;
            }
        }
    }
    
    for (const string& op : operations) {
        cout << op << "\n";
    }
    
    return 0;
}
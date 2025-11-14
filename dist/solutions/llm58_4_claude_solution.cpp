#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long x;
    cin >> x;
    
    // Convert x to binary representation
    vector<int> bits;
    long long temp = x;
    while (temp > 0) {
        bits.push_back(temp & 1);
        temp >>= 1;
    }
    
    int n = bits.size() + 1;
    vector<vector<int>> grid(n, vector<int>(n, 0));
    
    // Set the main diagonal and first row/column to 1
    for (int i = 0; i < n; i++) {
        grid[0][i] = 1;
        grid[i][0] = 1;
    }
    
    // For each bit position, create a path that contributes 2^i ways
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i] == 1) {
            // Create a path that adds 2^i ways
            for (int j = 1; j <= i + 1; j++) {
                grid[j][i + 1] = 1;
            }
        }
    }
    
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grid[i][j];
            if (j < n - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
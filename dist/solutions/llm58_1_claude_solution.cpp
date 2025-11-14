#include <bits/stdc++.h>
using namespace std;

int main() {
    long long x;
    cin >> x;
    
    // Find the binary representation of x
    vector<int> bits;
    long long temp = x;
    while (temp > 0) {
        bits.push_back(temp & 1);
        temp >>= 1;
    }
    
    int n = bits.size() + 1;
    vector<vector<int>> grid(n, vector<int>(n, 0));
    
    // Initialize the main diagonal path
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            grid[i][j] = 1;
        }
    }
    
    // Add paths based on binary representation
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i] == 1) {
            // Add a path that contributes 2^i ways
            for (int j = i + 1; j < n; j++) {
                grid[i][j] = 1;
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
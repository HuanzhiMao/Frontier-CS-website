#include <bits/stdc++.h>
using namespace std;

int main() {
    long long x;
    cin >> x;
    
    // Convert x to binary representation
    vector<int> binary;
    long long temp = x;
    while (temp > 0) {
        binary.push_back(temp % 2);
        temp /= 2;
    }
    
    int n = binary.size() + 1;
    vector<vector<int>> grid(n, vector<int>(n, 0));
    
    // Set the main diagonal to 1
    for (int i = 0; i < n; i++) {
        grid[i][i] = 1;
    }
    
    // Set (0,0) to 1
    grid[0][0] = 1;
    
    // For each bit in binary representation
    for (int i = 0; i < binary.size(); i++) {
        if (binary[i] == 1) {
            // Enable the path that contributes 2^i ways
            grid[0][i + 1] = 1;
        }
    }
    
    // Fill the grid to enable paths
    for (int i = 1; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            grid[i][j] = 1;
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
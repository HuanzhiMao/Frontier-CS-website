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
    
    // Set the main diagonal to 1
    for (int i = 0; i < n; i++) {
        grid[i][i] = 1;
    }
    
    // Set (0,0) to (n-1,n-1) path
    grid[0][0] = 1;
    grid[n-1][n-1] = 1;
    
    // For each bit, decide whether to add the corresponding path
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i] == 1) {
            // Add a path that contributes 2^i ways
            if (i + 1 < n) {
                grid[0][i + 1] = 1;
            }
        }
    }
    
    // Fill the grid to ensure connectivity
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i == j) {
                grid[i][j] = 1;
            }
        }
    }
    
    // Create a specific pattern based on binary representation
    n = 60; // Use a reasonable size
    grid = vector<vector<int>>(n, vector<int>(n, 0));
    
    // Set first row and first column
    grid[0][0] = 1;
    
    // Create paths based on binary representation of x
    temp = x;
    int pos = 1;
    while (temp > 1 && pos < n) {
        if (temp & 1) {
            grid[0][pos] = 1;
            for (int i = 1; i <= pos; i++) {
                grid[i][pos] = 1;
            }
        }
        grid[pos][pos] = 1;
        temp >>= 1;
        pos++;
    }
    
    // Ensure path to destination
    for (int i = 0; i < pos; i++) {
        grid[i][pos-1] = 1;
        grid[pos-1][i] = 1;
    }
    grid[pos-1][pos-1] = 1;
    
    cout << pos << "\n";
    for (int i = 0; i < pos; i++) {
        for (int j = 0; j < pos; j++) {
            cout << grid[i][j];
            if (j < pos - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
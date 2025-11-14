#include <bits/stdc++.h>
using namespace std;

int main() {
    long long x;
    cin >> x;
    
    // Find binary representation of x
    vector<int> bits;
    long long temp = x;
    while (temp > 0) {
        bits.push_back(temp & 1);
        temp >>= 1;
    }
    
    int n = bits.size() + 1;
    vector<vector<int>> grid(n, vector<int>(n, 0));
    
    // Set the main diagonal and the path below it
    for (int i = 0; i < n; i++) {
        grid[i][0] = 1; // First column
        grid[0][i] = 1; // First row
    }
    
    // For each bit in the binary representation
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i] == 1) {
            // Add a path that contributes 2^i ways
            grid[i + 1][i + 1] = 1;
        }
    }
    
    // Fill the rest to ensure connectivity
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            if (i == j && j <= bits.size()) {
                continue; // Already handled above
            }
            if (i < j) {
                grid[i][j] = 1;
            } else if (i == j + 1 && j < bits.size() && bits[j] == 0) {
                grid[i][j] = 0;
            } else if (i > j) {
                grid[i][j] = 1;
            }
        }
    }
    
    // Reconstruct using a different approach - build path step by step
    n = 64; // Use a reasonable size
    vector<vector<int>> result(n, vector<int>(n, 0));
    
    // Initialize first row and column
    for (int i = 0; i < n; i++) {
        result[0][i] = 1;
        result[i][0] = 1;
    }
    
    // Use binary representation approach
    temp = x;
    int pos = 1;
    while (temp > 1 && pos < n - 1) {
        if (temp & 1) {
            // This bit is set, create a bypass
            for (int i = pos; i < n; i++) {
                result[pos][i] = 1;
                result[i][pos] = 1;
            }
            temp = (temp - 1) / 2;
        } else {
            // This bit is not set, block the direct path
            for (int i = pos + 1; i < n; i++) {
                result[pos][i] = 0;
            }
            temp = temp / 2;
        }
        pos++;
    }
    
    // Ensure (n-1, n-1) is reachable
    result[n-1][n-1] = 1;
    
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << result[i][j];
            if (j < n - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
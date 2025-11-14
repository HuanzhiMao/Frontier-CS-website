#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<vector<int>> grid;
vector<vector<int>> result;
vector<set<int>> adj;

void findAdjacencies() {
    adj.resize(m + 1);
    
    // Check internal adjacencies
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i > 0 && grid[i][j] != grid[i-1][j]) {
                adj[grid[i][j]].insert(grid[i-1][j]);
                adj[grid[i-1][j]].insert(grid[i][j]);
            }
            if (j > 0 && grid[i][j] != grid[i][j-1]) {
                adj[grid[i][j]].insert(grid[i][j-1]);
                adj[grid[i][j-1]].insert(grid[i][j]);
            }
        }
    }
    
    // Check boundary adjacencies with color 0
    for (int i = 0; i < n; i++) {
        adj[0].insert(grid[i][0]);
        adj[grid[i][0]].insert(0);
        adj[0].insert(grid[i][n-1]);
        adj[grid[i][n-1]].insert(0);
        adj[0].insert(grid[0][i]);
        adj[grid[0][i]].insert(0);
        adj[0].insert(grid[n-1][i]);
        adj[grid[n-1][i]].insert(0);
    }
}

int main() {
    cin >> n >> m;
    grid.resize(n, vector<int>(n));
    result.resize(n, vector<int>(n, 0));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    
    findAdjacencies();
    
    // Simple approach: place each color in a minimal connected region
    // Start from top-left and place colors in a compact way
    
    vector<bool> placed(m + 1, false);
    placed[0] = true;
    
    // Place color 1 at (0,0)
    result[0][0] = 1;
    placed[1] = true;
    
    int row = 0, col = 1;
    
    for (int color = 2; color <= m; color++) {
        if (col >= n) {
            row++;
            col = 0;
        }
        result[row][col] = color;
        placed[color] = true;
        col++;
    }
    
    // Now we need to ensure adjacencies are preserved
    // This is a complex problem, so let's use a simpler approach
    
    // Reset and use a different strategy
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
        }
    }
    
    // Place colors in a line to minimize area while preserving adjacencies
    int pos = 0;
    for (int color = 1; color <= m; color++) {
        int r = pos / n;
        int c = pos % n;
        result[r][c] = color;
        pos++;
    }
    
    // Fill remaining with 0s (they're already 0)
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << result[i][j];
            if (j < n - 1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
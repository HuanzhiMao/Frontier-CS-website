#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    
    // Find adjacency relationships in original map
    set<pair<int, int>> adj;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Check adjacency with boundary (color 0)
            if (i == 0 || i == n-1 || j == 0 || j == n-1) {
                int c1 = min(0, grid[i][j]);
                int c2 = max(0, grid[i][j]);
                if (c1 != c2) adj.insert({c1, c2});
            }
            
            // Check adjacency with neighboring cells
            for (int d = 0; d < 4; d++) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                    if (grid[i][j] != grid[ni][nj]) {
                        int c1 = min(grid[i][j], grid[ni][nj]);
                        int c2 = max(grid[i][j], grid[ni][nj]);
                        adj.insert({c1, c2});
                    }
                }
            }
        }
    }
    
    // Create result grid
    vector<vector<int>> result(n, vector<int>(n, 0));
    
    // Place each color in a single cell, arranged in a compact way
    int pos = 0;
    for (int color = 1; color <= m; color++) {
        int row = pos / n;
        int col = pos % n;
        if (row < n) {
            result[row][col] = color;
        }
        pos++;
    }
    
    // Ensure adjacencies are preserved by expanding regions as needed
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Check each required adjacency
        for (auto& p : adj) {
            int c1 = p.first, c2 = p.second;
            bool found = false;
            
            // Check if this adjacency exists in current result
            for (int i = 0; i < n && !found; i++) {
                for (int j = 0; j < n && !found; j++) {
                    if (result[i][j] == c1 || result[i][j] == c2) {
                        // Check adjacency with boundary
                        if (c1 == 0 || c2 == 0) {
                            if ((i == 0 || i == n-1 || j == 0 || j == n-1) && 
                                (result[i][j] == c1 || result[i][j] == c2)) {
                                found = true;
                            }
                        }
                        
                        // Check adjacency with neighbors
                        for (int d = 0; d < 4; d++) {
                            int ni = i + dx[d];
                            int nj = j + dy[d];
                            if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                                if ((result[i][j] == c1 && result[ni][nj] == c2) ||
                                    (result[i][j] == c2 && result[ni][nj] == c1)) {
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            // If adjacency not found, try to create it
            if (!found) {
                for (int i = 0; i < n && !found; i++) {
                    for (int j = 0; j < n && !found; j++) {
                        if (result[i][j] == 0) {
                            for (int d = 0; d < 4; d++) {
                                int ni = i + dx[d];
                                int nj = j + dy[d];
                                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                                    if (result[ni][nj] == c1 || result[ni][nj] == c2) {
                                        result[i][j] = (result[ni][nj] == c1) ? c2 : c1;
                                        changed = true;
                                        found = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Output result
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << result[i][j];
            if (j < n-1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
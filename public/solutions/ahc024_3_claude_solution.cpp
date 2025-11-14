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
    
    // Find adjacencies in original map
    set<pair<int, int>> adj;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c1 = grid[i][j];
            
            // Check boundary (adjacent to color 0)
            if (i == 0 || i == n-1 || j == 0 || j == n-1) {
                int a = min(0, c1), b = max(0, c1);
                adj.insert({a, b});
            }
            
            // Check adjacent cells
            for (int d = 0; d < 4; d++) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                    int c2 = grid[ni][nj];
                    if (c1 != c2) {
                        int a = min(c1, c2), b = max(c1, c2);
                        adj.insert({a, b});
                    }
                }
            }
        }
    }
    
    // Create result grid
    vector<vector<int>> result(n, vector<int>(n, 0));
    
    // Place each color at specific positions to maintain connectivity
    // Use a simple layout: place colors in a spiral or grid pattern
    int pos = 0;
    for (int color = 1; color <= m; color++) {
        int row = pos / n;
        int col = pos % n;
        if (row < n && col < n) {
            result[row][col] = color;
            pos++;
        }
    }
    
    // Check and fix adjacencies
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (auto& p : adj) {
            int c1 = p.first, c2 = p.second;
            
            // Check if adjacency exists in current result
            bool exists = false;
            
            for (int i = 0; i < n && !exists; i++) {
                for (int j = 0; j < n && !exists; j++) {
                    if (result[i][j] == c1) {
                        // Check boundary for color 0
                        if (c2 == 0 && (i == 0 || i == n-1 || j == 0 || j == n-1)) {
                            exists = true;
                        }
                        
                        // Check adjacent cells
                        for (int d = 0; d < 4 && !exists; d++) {
                            int ni = i + dx[d];
                            int nj = j + dy[d];
                            if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                                if (result[ni][nj] == c2) {
                                    exists = true;
                                }
                            }
                        }
                    }
                }
            }
            
            if (!exists) {
                // Find positions of both colors and try to make them adjacent
                vector<pair<int, int>> pos1, pos2;
                
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        if (result[i][j] == c1) pos1.push_back({i, j});
                        if (result[i][j] == c2) pos2.push_back({i, j});
                    }
                }
                
                // If c2 is 0, try to place c1 on boundary
                if (c2 == 0 && !pos1.empty()) {
                    for (auto& pos : pos1) {
                        if (pos.first == 0 || pos.first == n-1 || pos.second == 0 || pos.second == n-1) {
                            exists = true;
                            break;
                        }
                    }
                    
                    if (!exists) {
                        // Move c1 to boundary
                        for (int i = 0; i < n; i++) {
                            if (result[0][i] == 0) {
                                result[0][i] = c1;
                                if (!pos1.empty()) {
                                    result[pos1[0].first][pos1[0].second] = 0;
                                }
                                changed = true;
                                break;
                            }
                        }
                    }
                } else if (!pos1.empty() && !pos2.empty()) {
                    // Try to make c1 and c2 adjacent
                    bool made_adjacent = false;
                    
                    for (auto& p1 : pos1) {
                        for (int d = 0; d < 4 && !made_adjacent; d++) {
                            int ni = p1.first + dx[d];
                            int nj = p1.second + dy[d];
                            if (ni >= 0 && ni < n && nj >= 0 && nj < n && result[ni][nj] == 0) {
                                result[ni][nj] = c2;
                                if (!pos2.empty()) {
                                    result[pos2[0].first][pos2[0].second] = 0;
                                }
                                made_adjacent = true;
                                changed = true;
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
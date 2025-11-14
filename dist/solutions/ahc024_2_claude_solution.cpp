#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> grid(n, vector<int>(n));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    
    // Find adjacency graph
    set<pair<int, int>> adj;
    vector<set<int>> neighbors(m + 1);
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c1 = grid[i][j];
            
            // Check boundary adjacency to color 0
            if (i == 0 || i == n-1 || j == 0 || j == n-1) {
                int a = min(c1, 0), b = max(c1, 0);
                if (a != b) {
                    adj.insert({a, b});
                    neighbors[c1].insert(0);
                    neighbors[0].insert(c1);
                }
            }
            
            for (int d = 0; d < 4; d++) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                
                if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                    int c2 = grid[ni][nj];
                    if (c1 != c2) {
                        int a = min(c1, c2), b = max(c1, c2);
                        adj.insert({a, b});
                        neighbors[c1].insert(c2);
                        neighbors[c2].insert(c1);
                    }
                }
            }
        }
    }
    
    // Create minimal map
    vector<vector<int>> result(n, vector<int>(n, 0));
    
    // Place one cell per color
    vector<pair<int, int>> pos(m + 1);
    int center = n / 2;
    
    // Place colors in a compact arrangement
    pos[0] = {-1, -1}; // Color 0 is outside
    
    int idx = 0;
    for (int color = 1; color <= m; color++) {
        int r = center + (idx / 10 - 5);
        int c = center + (idx % 10 - 5);
        r = max(0, min(n-1, r));
        c = max(0, min(n-1, c));
        pos[color] = {r, c};
        result[r][c] = color;
        idx++;
    }
    
    // Ensure connectivity for adjacency requirements
    for (auto& edge : adj) {
        int c1 = edge.first;
        int c2 = edge.second;
        
        if (c1 == 0) {
            // Ensure color c2 touches boundary
            int r = pos[c2].first;
            int c = pos[c2].second;
            
            // Move to boundary if not already there
            if (r != 0 && r != n-1 && c != 0 && c != n-1) {
                // Find closest boundary
                int dist_to_boundary = min({r, n-1-r, c, n-1-c});
                if (r == dist_to_boundary) {
                    // Move to top
                    result[r][c] = 0;
                    result[0][c] = c2;
                    pos[c2] = {0, c};
                } else if (n-1-r == dist_to_boundary) {
                    // Move to bottom
                    result[r][c] = 0;
                    result[n-1][c] = c2;
                    pos[c2] = {n-1, c};
                } else if (c == dist_to_boundary) {
                    // Move to left
                    result[r][c] = 0;
                    result[r][0] = c2;
                    pos[c2] = {r, 0};
                } else {
                    // Move to right
                    result[r][c] = 0;
                    result[r][n-1] = c2;
                    pos[c2] = {r, n-1};
                }
            }
        } else {
            // Ensure c1 and c2 are adjacent
            int r1 = pos[c1].first, c1_col = pos[c1].second;
            int r2 = pos[c2].first, c2_col = pos[c2].second;
            
            // If not adjacent, make them adjacent
            if (abs(r1 - r2) + abs(c1_col - c2_col) != 1) {
                // Move c2 next to c1
                for (int d = 0; d < 4; d++) {
                    int nr = r1 + dx[d];
                    int nc = c1_col + dy[d];
                    
                    if (nr >= 0 && nr < n && nc >= 0 && nc < n && 
                        result[nr][nc] == 0) {
                        result[r2][c2_col] = 0;
                        result[nr][nc] = c2;
                        pos[c2] = {nr, nc};
                        break;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << result[i][j];
            if (j < n-1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
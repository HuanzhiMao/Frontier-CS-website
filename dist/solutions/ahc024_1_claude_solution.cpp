#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 50, m;
    cin >> n >> m;
    
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    
    // Build adjacency graph
    set<pair<int, int>> adj;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int c1 = grid[i][j];
            
            // Check adjacency with boundary (color 0)
            if (i == 0 || i == n-1 || j == 0 || j == n-1) {
                int a = min(0, c1), b = max(0, c1);
                adj.insert({a, b});
            }
            
            // Check adjacency with neighbors
            for (int d = 0; d < 4; d++) {
                int ni = i + dx[d], nj = j + dy[d];
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
    
    // Place each color at center positions
    vector<pair<int, int>> centers;
    int rows = (int)sqrt(m) + 1;
    int cols = (m + rows - 1) / rows;
    
    for (int k = 1; k <= m; k++) {
        int idx = k - 1;
        int r = idx / cols;
        int c = idx % cols;
        
        int ci = 5 + r * 4;
        int cj = 5 + c * 4;
        
        if (ci < n && cj < n) {
            result[ci][cj] = k;
            centers.push_back({ci, cj});
        }
    }
    
    // Connect adjacent colors
    for (auto& edge : adj) {
        int c1 = edge.first, c2 = edge.second;
        
        if (c1 == 0) {
            // Connect c2 to boundary
            pair<int, int> pos2 = {-1, -1};
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (result[i][j] == c2) {
                        pos2 = {i, j};
                        break;
                    }
                }
                if (pos2.first != -1) break;
            }
            
            if (pos2.first != -1) {
                // Connect to closest boundary
                int dist_to_boundary = min({pos2.first, pos2.second, n-1-pos2.first, n-1-pos2.second});
                
                if (pos2.first == dist_to_boundary) {
                    // Connect to top
                    for (int i = pos2.first - 1; i >= 0; i--) {
                        if (result[i][pos2.second] == 0) {
                            result[i][pos2.second] = c2;
                        }
                    }
                } else if (pos2.second == dist_to_boundary) {
                    // Connect to left
                    for (int j = pos2.second - 1; j >= 0; j--) {
                        if (result[pos2.first][j] == 0) {
                            result[pos2.first][j] = c2;
                        }
                    }
                } else if (n-1-pos2.first == dist_to_boundary) {
                    // Connect to bottom
                    for (int i = pos2.first + 1; i < n; i++) {
                        if (result[i][pos2.second] == 0) {
                            result[i][pos2.second] = c2;
                        }
                    }
                } else {
                    // Connect to right
                    for (int j = pos2.second + 1; j < n; j++) {
                        if (result[pos2.first][j] == 0) {
                            result[pos2.first][j] = c2;
                        }
                    }
                }
            }
        } else {
            // Connect c1 and c2
            pair<int, int> pos1 = {-1, -1}, pos2 = {-1, -1};
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (result[i][j] == c1 && pos1.first == -1) {
                        pos1 = {i, j};
                    }
                    if (result[i][j] == c2 && pos2.first == -1) {
                        pos2 = {i, j};
                    }
                }
            }
            
            if (pos1.first != -1 && pos2.first != -1) {
                // Simple path connection
                int i1 = pos1.first, j1 = pos1.second;
                int i2 = pos2.first, j2 = pos2.second;
                
                // Create path
                int mi = (i1 + i2) / 2;
                int mj = (j1 + j2) / 2;
                
                // Connect pos1 to middle
                while (i1 != mi || j1 != mj) {
                    if (i1 < mi) i1++;
                    else if (i1 > mi) i1--;
                    else if (j1 < mj) j1++;
                    else if (j1 > mj) j1--;
                    
                    if (result[i1][j1] == 0) {
                        result[i1][j1] = c1;
                    }
                }
                
                // Connect middle to pos2
                while (i1 != i2 || j1 != j2) {
                    if (i1 < i2) i1++;
                    else if (i1 > i2) i1--;
                    else if (j1 < j2) j1++;
                    else if (j1 > j2) j1--;
                    
                    if (result[i1][j1] == 0) {
                        result[i1][j1] = c2;
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
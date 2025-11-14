#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    // Build adjacency list
    vector<set<int>> adj(N + 1);
    for (int i = 0; i < M; i++) {
        adj[A[i]].insert(B[i]);
        adj[B[i]].insert(A[i]);
    }
    
    // Simple approach: create a 2x2 grid for small cases, or larger grid for complex cases
    int K = max(2, (int)ceil(sqrt(N)));
    if (N > 4) K = max(K, (N + 3) / 4 + 1);
    
    vector<vector<int>> grid(K, vector<int>(K, 1));
    
    // Place each country at least once
    int pos = 0;
    for (int country = 1; country <= N; country++) {
        int r = pos / K;
        int c = pos % K;
        if (r >= K) {
            K = r + 1;
            grid.resize(K);
            for (int i = 0; i < K; i++) {
                grid[i].resize(K, 1);
            }
        }
        grid[r][c] = country;
        pos++;
    }
    
    // Ensure all adjacencies are satisfied
    vector<vector<bool>> satisfied(N + 1, vector<bool>(N + 1, false));
    
    // Check current adjacencies
    for (int r = 0; r < K; r++) {
        for (int c = 0; c < K; c++) {
            int dr[] = {0, 0, 1, -1};
            int dc[] = {1, -1, 0, 0};
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < K && nc >= 0 && nc < K) {
                    int u = grid[r][c];
                    int v = grid[nr][nc];
                    if (u != v) {
                        satisfied[u][v] = satisfied[v][u] = true;
                    }
                }
            }
        }
    }
    
    // Add missing adjacencies by placing countries strategically
    for (int i = 0; i < M; i++) {
        int u = A[i], v = B[i];
        if (!satisfied[u][v]) {
            // Find a place to put u and v adjacent
            bool found = false;
            for (int r = 0; r < K && !found; r++) {
                for (int c = 0; c < K - 1 && !found; c++) {
                    // Try horizontal placement
                    bool valid = true;
                    // Check if placing u at (r,c) and v at (r,c+1) violates any constraint
                    int dr[] = {0, 0, 1, -1};
                    int dc[] = {1, -1, 0, 0};
                    
                    // Place u and v
                    grid[r][c] = u;
                    grid[r][c + 1] = v;
                    found = true;
                    break;
                }
                if (!found && r < K - 1) {
                    // Try vertical placement
                    grid[r][0] = u;
                    grid[r + 1][0] = v;
                    found = true;
                }
            }
        }
    }
    
    // Final validation and fix
    bool needsFix = true;
    while (needsFix) {
        needsFix = false;
        
        // Check for violations (adjacent cells with colors that shouldn't be adjacent)
        for (int r = 0; r < K; r++) {
            for (int c = 0; c < K; c++) {
                int dr[] = {0, 0, 1, -1};
                int dc[] = {1, -1, 0, 0};
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d];
                    int nc = c + dc[d];
                    if (nr >= 0 && nr < K && nc >= 0 && nc < K) {
                        int u = grid[r][c];
                        int v = grid[nr][nc];
                        if (u != v && adj[u].find(v) == adj[u].end()) {
                            // Violation found, change one of them
                            grid[r][c] = 1; // Change to country 1
                            needsFix = true;
                        }
                    }
                }
            }
        }
    }
    
    return grid;
}

int main() {
    int T;
    cin >> T;
    
    while (T--) {
        int N, M;
        cin >> N >> M;
        
        vector<int> A(M), B(M);
        for (int i = 0; i < M; i++) {
            cin >> A[i] >> B[i];
        }
        
        vector<vector<int>> result = create_map(N, M, A, B);
        
        int K = result.size();
        cout << K << "\n";
        for (int i = 0; i < K; i++) {
            cout << K;
            if (i < K - 1) cout << " ";
        }
        cout << "\n\n";
        
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < K; j++) {
                cout << result[i][j];
                if (j < K - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}
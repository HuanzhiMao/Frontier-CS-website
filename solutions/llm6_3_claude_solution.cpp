#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    // Build adjacency list
    vector<set<int>> adj(N + 1);
    for (int i = 0; i < M; i++) {
        adj[A[i]].insert(B[i]);
        adj[B[i]].insert(A[i]);
    }
    
    // Simple approach: create a grid where we place countries strategically
    int K = max(2, (int)ceil(sqrt(N * 2)));
    
    vector<vector<int>> grid(K, vector<int>(K, 1));
    
    // Place each country at least once
    for (int i = 1; i <= N; i++) {
        int row = (i - 1) / K;
        int col = (i - 1) % K;
        if (row < K) {
            grid[row][col] = i;
        }
    }
    
    // Fill remaining cells to ensure all adjacencies are satisfied
    for (int iter = 0; iter < 10; iter++) {
        vector<pair<int, int>> missing_edges;
        
        // Check which edges are missing
        for (int i = 0; i < M; i++) {
            int u = A[i], v = B[i];
            bool found = false;
            
            for (int r = 0; r < K && !found; r++) {
                for (int c = 0; c < K && !found; c++) {
                    if (grid[r][c] == u) {
                        // Check adjacent cells
                        int dr[] = {-1, 1, 0, 0};
                        int dc[] = {0, 0, -1, 1};
                        for (int d = 0; d < 4; d++) {
                            int nr = r + dr[d];
                            int nc = c + dc[d];
                            if (nr >= 0 && nr < K && nc >= 0 && nc < K) {
                                if (grid[nr][nc] == v) {
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            if (!found) {
                missing_edges.push_back({u, v});
            }
        }
        
        // Try to fix missing edges
        for (auto edge : missing_edges) {
            int u = edge.first, v = edge.second;
            bool fixed = false;
            
            for (int r = 0; r < K && !fixed; r++) {
                for (int c = 0; c < K && !fixed; c++) {
                    if (grid[r][c] == u) {
                        int dr[] = {-1, 1, 0, 0};
                        int dc[] = {0, 0, -1, 1};
                        for (int d = 0; d < 4; d++) {
                            int nr = r + dr[d];
                            int nc = c + dc[d];
                            if (nr >= 0 && nr < K && nc >= 0 && nc < K) {
                                // Check if we can place v here
                                bool can_place = true;
                                int old_val = grid[nr][nc];
                                
                                // Check if placing v here would create invalid adjacencies
                                for (int dd = 0; dd < 4; dd++) {
                                    int nnr = nr + dr[dd];
                                    int nnc = nc + dc[dd];
                                    if (nnr >= 0 && nnr < K && nnc >= 0 && nnc < K) {
                                        int neighbor = grid[nnr][nnc];
                                        if (neighbor != v && adj[v].find(neighbor) == adj[v].end()) {
                                            can_place = false;
                                            break;
                                        }
                                    }
                                }
                                
                                if (can_place) {
                                    grid[nr][nc] = v;
                                    fixed = true;
                                    break;
                                }
                            }
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
        
        auto result = create_map(N, M, A, B);
        
        cout << result.size() << "\n";
        for (int i = 0; i < result.size(); i++) {
            cout << result[i].size();
            if (i < result.size() - 1) cout << " ";
        }
        cout << "\n\n";
        
        for (int i = 0; i < result.size(); i++) {
            for (int j = 0; j < result[i].size(); j++) {
                cout << result[i][j];
                if (j < result[i].size() - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}
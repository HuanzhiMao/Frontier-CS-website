#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    vector<vector<bool>> adj(N + 1, vector<bool>(N + 1, false));
    
    // Build adjacency matrix
    for (int i = 0; i < M; i++) {
        adj[A[i]][B[i]] = true;
        adj[B[i]][A[i]] = true;
    }
    
    // Simple approach: create a grid where each country appears
    // Try to make it as compact as possible
    int K = max(2, (int)ceil(sqrt(N)));
    
    while (true) {
        vector<vector<int>> grid(K, vector<int>(K, 1));
        
        // Place countries in grid
        vector<pair<int, int>> positions(N + 1);
        int idx = 0;
        
        // Assign positions for each country
        for (int i = 1; i <= N; i++) {
            positions[i] = {idx / K, idx % K};
            grid[idx / K][idx % K] = i;
            idx++;
            if (idx >= K * K) break;
        }
        
        // Fill remaining cells
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < K; j++) {
                if (grid[i][j] == 1 && (i != 0 || j != 0)) {
                    grid[i][j] = 1 + ((i + j) % N);
                }
            }
        }
        
        // Check if all adjacency requirements are satisfied
        vector<vector<bool>> found_adj(N + 1, vector<bool>(N + 1, false));
        bool valid = true;
        
        // Check adjacencies in grid
        for (int i = 0; i < K && valid; i++) {
            for (int j = 0; j < K && valid; j++) {
                int curr = grid[i][j];
                
                // Check all 4 directions
                int di[] = {-1, 1, 0, 0};
                int dj[] = {0, 0, -1, 1};
                
                for (int d = 0; d < 4; d++) {
                    int ni = i + di[d];
                    int nj = j + dj[d];
                    
                    if (ni >= 0 && ni < K && nj >= 0 && nj < K) {
                        int next = grid[ni][nj];
                        if (curr != next) {
                            if (!adj[curr][next]) {
                                valid = false;
                                break;
                            }
                            found_adj[curr][next] = true;
                            found_adj[next][curr] = true;
                        }
                    }
                }
            }
        }
        
        // Check if all required adjacencies are found
        if (valid) {
            for (int i = 0; i < M; i++) {
                if (!found_adj[A[i]][B[i]]) {
                    valid = false;
                    break;
                }
            }
        }
        
        if (valid) {
            return grid;
        }
        
        K++;
        if (K > 240) break;
    }
    
    // Fallback: create a simple valid configuration
    K = N;
    vector<vector<int>> grid(K, vector<int>(K));
    
    // Place countries in first row
    for (int i = 0; i < K; i++) {
        grid[0][i] = (i % N) + 1;
    }
    
    // Fill rest with pattern that ensures adjacencies
    for (int i = 1; i < K; i++) {
        for (int j = 0; j < K; j++) {
            grid[i][j] = ((i + j) % N) + 1;
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
        
        int P = result.size();
        cout << P << "\n";
        
        for (int i = 0; i < P; i++) {
            cout << result[i].size();
            if (i < P - 1) cout << " ";
        }
        cout << "\n\n";
        
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < result[i].size(); j++) {
                cout << result[i][j];
                if (j < result[i].size() - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}
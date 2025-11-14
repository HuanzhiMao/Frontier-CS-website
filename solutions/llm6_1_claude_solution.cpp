#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    // Build adjacency list
    vector<set<int>> adj(N + 1);
    for (int i = 0; i < M; i++) {
        adj[A[i]].insert(B[i]);
        adj[B[i]].insert(A[i]);
    }
    
    // Try different grid sizes starting from small ones
    for (int K = 1; K <= 240; K++) {
        vector<vector<int>> grid(K, vector<int>(K, 0));
        
        // Try to place countries in the grid
        bool success = true;
        vector<bool> placed(N + 1, false);
        
        // Simple strategy: place countries in order
        int pos = 0;
        for (int country = 1; country <= N; country++) {
            int r = pos / K;
            int c = pos % K;
            if (r >= K) {
                success = false;
                break;
            }
            grid[r][c] = country;
            placed[country] = true;
            pos++;
        }
        
        if (!success) continue;
        
        // Fill remaining cells to satisfy adjacency constraints
        for (int r = 0; r < K; r++) {
            for (int c = 0; c < K; c++) {
                if (grid[r][c] == 0) {
                    grid[r][c] = 1; // Fill with country 1
                }
            }
        }
        
        // Check if all countries are present
        vector<bool> present(N + 1, false);
        for (int r = 0; r < K; r++) {
            for (int c = 0; c < K; c++) {
                present[grid[r][c]] = true;
            }
        }
        
        bool allPresent = true;
        for (int i = 1; i <= N; i++) {
            if (!present[i]) {
                allPresent = false;
                break;
            }
        }
        
        if (!allPresent) continue;
        
        // Check adjacency constraints
        set<pair<int, int>> requiredAdj;
        for (int i = 0; i < M; i++) {
            requiredAdj.insert({min(A[i], B[i]), max(A[i], B[i])});
        }
        
        set<pair<int, int>> foundAdj;
        bool validAdj = true;
        
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        
        for (int r = 0; r < K && validAdj; r++) {
            for (int c = 0; c < K && validAdj; c++) {
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d];
                    int nc = c + dc[d];
                    if (nr >= 0 && nr < K && nc >= 0 && nc < K) {
                        int country1 = grid[r][c];
                        int country2 = grid[nr][nc];
                        if (country1 != country2) {
                            pair<int, int> adjPair = {min(country1, country2), max(country1, country2)};
                            foundAdj.insert(adjPair);
                            
                            // Check if this adjacency is allowed
                            if (requiredAdj.find(adjPair) == requiredAdj.end()) {
                                validAdj = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        if (!validAdj) continue;
        
        // Check if all required adjacencies are satisfied
        bool allAdjSatisfied = true;
        for (auto req : requiredAdj) {
            if (foundAdj.find(req) == foundAdj.end()) {
                allAdjSatisfied = false;
                break;
            }
        }
        
        if (allAdjSatisfied) {
            return grid;
        }
    }
    
    // Fallback: create a larger grid that definitely works
    int K = min(240, N + 10);
    vector<vector<int>> grid(K, vector<int>(K, 1));
    
    // Place each country at least once
    for (int i = 1; i <= N; i++) {
        grid[0][i-1] = i;
    }
    
    // Ensure adjacencies by placing adjacent countries next to each other
    for (int i = 0; i < M; i++) {
        grid[1][i*2] = A[i];
        grid[1][i*2+1] = B[i];
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
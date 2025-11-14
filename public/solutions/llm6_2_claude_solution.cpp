#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> create_map(int N, int M, vector<int> A, vector<int> B) {
    // Build adjacency list
    vector<set<int>> adj(N + 1);
    for (int i = 0; i < M; i++) {
        adj[A[i]].insert(B[i]);
        adj[B[i]].insert(A[i]);
    }
    
    // Try small grid sizes first
    for (int K = 1; K <= 240; K++) {
        vector<vector<int>> grid(K, vector<int>(K, 0));
        vector<bool> used(N + 1, false);
        
        // Try to fill the grid
        bool valid = true;
        
        // Place countries in a simple pattern
        for (int i = 0; i < K && valid; i++) {
            for (int j = 0; j < K && valid; j++) {
                int country = ((i * K + j) % N) + 1;
                grid[i][j] = country;
                used[country] = true;
            }
        }
        
        // Check if all countries are used
        bool allUsed = true;
        for (int i = 1; i <= N; i++) {
            if (!used[i]) {
                allUsed = false;
                break;
            }
        }
        if (!allUsed) continue;
        
        // Check adjacency requirements
        set<pair<int, int>> requiredAdj;
        for (int i = 0; i < M; i++) {
            requiredAdj.insert({min(A[i], B[i]), max(A[i], B[i])});
        }
        
        set<pair<int, int>> foundAdj;
        bool invalidAdj = false;
        
        for (int i = 0; i < K && !invalidAdj; i++) {
            for (int j = 0; j < K && !invalidAdj; j++) {
                // Check right neighbor
                if (j + 1 < K && grid[i][j] != grid[i][j + 1]) {
                    int c1 = grid[i][j], c2 = grid[i][j + 1];
                    if (adj[c1].find(c2) == adj[c1].end()) {
                        invalidAdj = true;
                        break;
                    }
                    foundAdj.insert({min(c1, c2), max(c1, c2)});
                }
                // Check bottom neighbor
                if (i + 1 < K && grid[i][j] != grid[i + 1][j]) {
                    int c1 = grid[i][j], c2 = grid[i + 1][j];
                    if (adj[c1].find(c2) == adj[c1].end()) {
                        invalidAdj = true;
                        break;
                    }
                    foundAdj.insert({min(c1, c2), max(c1, c2)});
                }
            }
        }
        
        if (invalidAdj) continue;
        
        // Check if all required adjacencies are found
        bool allFound = true;
        for (const auto& req : requiredAdj) {
            if (foundAdj.find(req) == foundAdj.end()) {
                allFound = false;
                break;
            }
        }
        
        if (allFound) {
            return grid;
        }
    }
    
    // Fallback: create a larger grid that should work
    int K = min(240, N * 2);
    vector<vector<int>> grid(K, vector<int>(K));
    
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            grid[i][j] = ((i * K + j) % N) + 1;
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
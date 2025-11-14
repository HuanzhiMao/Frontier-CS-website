#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<string> strings(M);
    for (int i = 0; i < M; i++) {
        cin >> strings[i];
    }
    
    vector<string> matrix(N, string(N, '.'));
    vector<bool> used(M, false);
    
    // Try to place each string
    for (int iter = 0; iter < M; iter++) {
        int best_idx = -1;
        int best_conflicts = INT_MAX;
        int best_row = -1, best_col = -1, best_dir = -1;
        
        for (int i = 0; i < M; i++) {
            if (used[i]) continue;
            
            string& s = strings[i];
            int len = s.length();
            
            // Try horizontal placement
            for (int r = 0; r < N; r++) {
                for (int c = 0; c < N; c++) {
                    int conflicts = 0;
                    for (int p = 0; p < len; p++) {
                        int cc = (c + p) % N;
                        if (matrix[r][cc] != '.' && matrix[r][cc] != s[p]) {
                            conflicts++;
                        }
                    }
                    if (conflicts < best_conflicts) {
                        best_conflicts = conflicts;
                        best_idx = i;
                        best_row = r;
                        best_col = c;
                        best_dir = 0; // horizontal
                    }
                }
            }
            
            // Try vertical placement
            for (int r = 0; r < N; r++) {
                for (int c = 0; c < N; c++) {
                    int conflicts = 0;
                    for (int p = 0; p < len; p++) {
                        int rr = (r + p) % N;
                        if (matrix[rr][c] != '.' && matrix[rr][c] != s[p]) {
                            conflicts++;
                        }
                    }
                    if (conflicts < best_conflicts) {
                        best_conflicts = conflicts;
                        best_idx = i;
                        best_row = r;
                        best_col = c;
                        best_dir = 1; // vertical
                    }
                }
            }
        }
        
        if (best_idx == -1) break;
        
        // Place the best string
        string& s = strings[best_idx];
        int len = s.length();
        used[best_idx] = true;
        
        if (best_dir == 0) { // horizontal
            for (int p = 0; p < len; p++) {
                int cc = (best_col + p) % N;
                matrix[best_row][cc] = s[p];
            }
        } else { // vertical
            for (int p = 0; p < len; p++) {
                int rr = (best_row + p) % N;
                matrix[rr][best_col] = s[p];
            }
        }
    }
    
    // Output the matrix
    for (int i = 0; i < N; i++) {
        cout << matrix[i] << "\n";
    }
    
    return 0;
}
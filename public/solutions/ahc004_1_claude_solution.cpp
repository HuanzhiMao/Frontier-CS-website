#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<string> strings(M);
    for (int i = 0; i < M; i++) {
        cin >> strings[i];
    }
    
    // Initialize grid with dots
    vector<string> grid(N, string(N, '.'));
    
    // Try to place strings greedily
    vector<bool> used(M, false);
    
    // For each position and direction, try to place the longest possible string
    for (int iter = 0; iter < 10; iter++) {
        bool changed = false;
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                // Try horizontal placement
                for (int s = 0; s < M; s++) {
                    if (used[s]) continue;
                    
                    string& str = strings[s];
                    bool canPlace = true;
                    
                    // Check if we can place horizontally
                    for (int k = 0; k < str.length(); k++) {
                        int col = (j + k) % N;
                        if (grid[i][col] != '.' && grid[i][col] != str[k]) {
                            canPlace = false;
                            break;
                        }
                    }
                    
                    if (canPlace) {
                        // Place the string
                        for (int k = 0; k < str.length(); k++) {
                            int col = (j + k) % N;
                            grid[i][col] = str[k];
                        }
                        used[s] = true;
                        changed = true;
                        break;
                    }
                }
                
                // Try vertical placement
                for (int s = 0; s < M; s++) {
                    if (used[s]) continue;
                    
                    string& str = strings[s];
                    bool canPlace = true;
                    
                    // Check if we can place vertically
                    for (int k = 0; k < str.length(); k++) {
                        int row = (i + k) % N;
                        if (grid[row][j] != '.' && grid[row][j] != str[k]) {
                            canPlace = false;
                            break;
                        }
                    }
                    
                    if (canPlace) {
                        // Place the string
                        for (int k = 0; k < str.length(); k++) {
                            int row = (i + k) % N;
                            grid[row][j] = str[k];
                        }
                        used[s] = true;
                        changed = true;
                        break;
                    }
                }
            }
        }
        
        if (!changed) break;
    }
    
    // Output the grid
    for (int i = 0; i < N; i++) {
        cout << grid[i] << "\n";
    }
    
    return 0;
}
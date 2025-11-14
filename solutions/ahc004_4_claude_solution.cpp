#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
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
    for (const string& s : strings) {
        bool placed = false;
        
        // Try horizontal placements
        for (int i = 0; i < N && !placed; i++) {
            for (int j = 0; j < N && !placed; j++) {
                bool canPlace = true;
                
                // Check if we can place horizontally at (i,j)
                for (int k = 0; k < s.length(); k++) {
                    int col = (j + k) % N;
                    if (grid[i][col] != '.' && grid[i][col] != s[k]) {
                        canPlace = false;
                        break;
                    }
                }
                
                if (canPlace) {
                    // Place the string horizontally
                    for (int k = 0; k < s.length(); k++) {
                        int col = (j + k) % N;
                        grid[i][col] = s[k];
                    }
                    placed = true;
                }
            }
        }
        
        // Try vertical placements if horizontal didn't work
        if (!placed) {
            for (int i = 0; i < N && !placed; i++) {
                for (int j = 0; j < N && !placed; j++) {
                    bool canPlace = true;
                    
                    // Check if we can place vertically at (i,j)
                    for (int k = 0; k < s.length(); k++) {
                        int row = (i + k) % N;
                        if (grid[row][j] != '.' && grid[row][j] != s[k]) {
                            canPlace = false;
                            break;
                        }
                    }
                    
                    if (canPlace) {
                        // Place the string vertically
                        for (int k = 0; k < s.length(); k++) {
                            int row = (i + k) % N;
                            grid[row][j] = s[k];
                        }
                        placed = true;
                    }
                }
            }
        }
    }
    
    // Output the grid
    for (int i = 0; i < N; i++) {
        cout << grid[i] << "\n";
    }
    
    return 0;
}
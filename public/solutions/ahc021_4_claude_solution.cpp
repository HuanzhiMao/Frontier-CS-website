#include <bits/stdc++.h>
using namespace std;

struct Move {
    int x1, y1, x2, y2;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int N = 30;
    vector<vector<int>> grid(N);
    
    for (int i = 0; i < N; i++) {
        grid[i].resize(i + 1);
        for (int j = 0; j <= i; j++) {
            cin >> grid[i][j];
        }
    }
    
    vector<Move> moves;
    
    // Simple bubble sort approach for each row
    for (int iter = 0; iter < 100 && moves.size() < 9000; iter++) {
        bool changed = false;
        
        // For each position except bottom row
        for (int x = 0; x < N - 1; x++) {
            for (int y = 0; y <= x; y++) {
                // Check if current ball violates condition with children
                int current = grid[x][y];
                int left_child = grid[x + 1][y];
                int right_child = grid[x + 1][y + 1];
                
                if (current > left_child || current > right_child) {
                    // Try to swap with adjacent positions to fix violation
                    vector<pair<int, int>> adjacent = {
                        {x - 1, y - 1}, {x - 1, y},
                        {x, y - 1}, {x, y + 1},
                        {x + 1, y}, {x + 1, y + 1}
                    };
                    
                    for (auto [nx, ny] : adjacent) {
                        if (nx >= 0 && nx < N && ny >= 0 && ny <= nx) {
                            int neighbor = grid[nx][ny];
                            
                            // Check if swapping would help
                            if (neighbor < current) {
                                // Perform swap
                                swap(grid[x][y], grid[nx][ny]);
                                moves.push_back({x, y, nx, ny});
                                changed = true;
                                break;
                            }
                        }
                    }
                    
                    if (changed) break;
                }
            }
            if (changed) break;
        }
        
        if (!changed) break;
    }
    
    // Additional passes to minimize violations
    for (int iter = 0; iter < 50 && moves.size() < 9500; iter++) {
        bool improved = false;
        
        for (int x = 0; x < N - 1; x++) {
            for (int y = 0; y <= x; y++) {
                int current = grid[x][y];
                int left_child = grid[x + 1][y];
                int right_child = grid[x + 1][y + 1];
                
                if (current > left_child || current > right_child) {
                    vector<pair<int, int>> adjacent = {
                        {x - 1, y - 1}, {x - 1, y},
                        {x, y - 1}, {x, y + 1},
                        {x + 1, y}, {x + 1, y + 1}
                    };
                    
                    int best_nx = -1, best_ny = -1;
                    int best_improvement = 0;
                    
                    for (auto [nx, ny] : adjacent) {
                        if (nx >= 0 && nx < N && ny >= 0 && ny <= nx) {
                            int neighbor = grid[nx][ny];
                            
                            if (neighbor < current) {
                                int improvement = 0;
                                if (current > left_child && neighbor <= left_child) improvement++;
                                if (current > right_child && neighbor <= right_child) improvement++;
                                
                                if (improvement > best_improvement) {
                                    best_improvement = improvement;
                                    best_nx = nx;
                                    best_ny = ny;
                                }
                            }
                        }
                    }
                    
                    if (best_nx != -1) {
                        swap(grid[x][y], grid[best_nx][best_ny]);
                        moves.push_back({x, y, best_nx, best_ny});
                        improved = true;
                    }
                }
            }
        }
        
        if (!improved) break;
    }
    
    cout << moves.size() << "\n";
    for (auto move : moves) {
        cout << move.x1 << " " << move.y1 << " " << move.x2 << " " << move.y2 << "\n";
    }
    
    return 0;
}
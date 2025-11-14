#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> f(100);
    for (int i = 0; i < 100; i++) {
        cin >> f[i];
    }
    
    int grid[10][10];
    memset(grid, 0, sizeof(grid));
    
    for (int t = 0; t < 100; t++) {
        int p;
        cin >> p;
        
        // Find the p-th empty cell
        int count = 0;
        int row = -1, col = -1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (grid[i][j] == 0) {
                    count++;
                    if (count == p) {
                        row = i;
                        col = j;
                        break;
                    }
                }
            }
            if (row != -1) break;
        }
        
        // Place the candy
        grid[row][col] = f[t];
        
        // Simple strategy: try to group similar candies
        char move = 'F';
        
        // Count candies of each type in different regions
        int top_count[4] = {0}, bottom_count[4] = {0};
        int left_count[4] = {0}, right_count[4] = {0};
        
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (grid[i][j] != 0) {
                    if (i < 5) top_count[grid[i][j]]++;
                    else bottom_count[grid[i][j]]++;
                    
                    if (j < 5) left_count[grid[i][j]]++;
                    else right_count[grid[i][j]]++;
                }
            }
        }
        
        // Simple heuristic: move towards the region with more of the same type
        int candy_type = f[t];
        
        int best_score = -1;
        char best_move = 'F';
        
        // Try each direction and pick based on clustering potential
        if (top_count[candy_type] > bottom_count[candy_type]) {
            if (best_score < top_count[candy_type]) {
                best_score = top_count[candy_type];
                best_move = 'F';
            }
        } else {
            if (best_score < bottom_count[candy_type]) {
                best_score = bottom_count[candy_type];
                best_move = 'B';
            }
        }
        
        if (left_count[candy_type] > right_count[candy_type]) {
            if (best_score < left_count[candy_type]) {
                best_score = left_count[candy_type];
                best_move = 'L';
            }
        } else {
            if (best_score < right_count[candy_type]) {
                best_score = right_count[candy_type];
                best_move = 'R';
            }
        }
        
        cout << best_move << endl;
        cout.flush();
        
        // Simulate the move to update our grid state
        if (best_move == 'F') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 0;
                for (int i = 0; i < 10; i++) {
                    if (grid[i][j] != 0) {
                        if (write_pos != i) {
                            grid[write_pos][j] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos++;
                    }
                }
            }
        } else if (best_move == 'B') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 9;
                for (int i = 9; i >= 0; i--) {
                    if (grid[i][j] != 0) {
                        if (write_pos != i) {
                            grid[write_pos][j] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos--;
                    }
                }
            }
        } else if (best_move == 'L') {
            for (int i = 0; i < 10; i++) {
                int write_pos = 0;
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j] != 0) {
                        if (write_pos != j) {
                            grid[i][write_pos] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos++;
                    }
                }
            }
        } else if (best_move == 'R') {
            for (int i = 0; i < 10; i++) {
                int write_pos = 9;
                for (int j = 9; j >= 0; j--) {
                    if (grid[i][j] != 0) {
                        if (write_pos != j) {
                            grid[i][write_pos] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos--;
                    }
                }
            }
        }
    }
    
    return 0;
}
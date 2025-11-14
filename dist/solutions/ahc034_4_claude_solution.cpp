#include <bits/stdc++.h>
using namespace std;

struct State {
    int r, c, load;
    vector<vector<int>> grid;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<vector<int>> grid(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];
        }
    }
    
    vector<string> operations;
    int r = 0, c = 0, load = 0;
    
    // Simple greedy approach: visit all cells in row-major order
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Move to cell (i, j)
            while (r != i || c != j) {
                if (r < i) {
                    operations.push_back("D");
                    r++;
                } else if (r > i) {
                    operations.push_back("U");
                    r--;
                } else if (c < j) {
                    operations.push_back("R");
                    c++;
                } else {
                    operations.push_back("L");
                    c--;
                }
            }
            
            // Handle current cell
            if (grid[i][j] > 0) {
                // Load soil from this cell
                int amount = grid[i][j];
                operations.push_back("+" + to_string(amount));
                load += amount;
                grid[i][j] = 0;
            } else if (grid[i][j] < 0) {
                // Unload soil to this cell
                int amount = min(load, -grid[i][j]);
                if (amount > 0) {
                    operations.push_back("-" + to_string(amount));
                    load -= amount;
                    grid[i][j] += amount;
                }
            }
        }
    }
    
    // Second pass to handle remaining negative heights
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] < 0 && load > 0) {
                // Move to cell (i, j)
                while (r != i || c != j) {
                    if (r < i) {
                        operations.push_back("D");
                        r++;
                    } else if (r > i) {
                        operations.push_back("U");
                        r--;
                    } else if (c < j) {
                        operations.push_back("R");
                        c++;
                    } else {
                        operations.push_back("L");
                        c--;
                    }
                }
                
                // Unload soil to this cell
                int amount = min(load, -grid[i][j]);
                if (amount > 0) {
                    operations.push_back("-" + to_string(amount));
                    load -= amount;
                    grid[i][j] += amount;
                }
            }
        }
    }
    
    for (const string& op : operations) {
        cout << op << "\n";
    }
    
    return 0;
}
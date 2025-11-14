#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

struct State {
    int r, c, load;
};

int main() {
    int N;
    cin >> N;
    
    vector<vector<int>> h(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> h[i][j];
        }
    }
    
    vector<string> operations;
    State state = {0, 0, 0};
    
    // Simple greedy approach: visit all cells in row-major order
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Move to cell (i, j)
            while (state.r < i) {
                operations.push_back("D");
                state.r++;
            }
            while (state.r > i) {
                operations.push_back("U");
                state.r--;
            }
            while (state.c < j) {
                operations.push_back("R");
                state.c++;
            }
            while (state.c > j) {
                operations.push_back("L");
                state.c--;
            }
            
            // Handle the current cell
            if (h[i][j] > 0) {
                // Load soil from this cell
                int amount = h[i][j];
                operations.push_back("+" + to_string(amount));
                h[i][j] = 0;
                state.load += amount;
            } else if (h[i][j] < 0 && state.load > 0) {
                // Unload soil to this cell
                int needed = -h[i][j];
                int amount = min(needed, state.load);
                operations.push_back("-" + to_string(amount));
                h[i][j] += amount;
                state.load -= amount;
            }
        }
    }
    
    // Second pass to handle remaining negative heights
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (h[i][j] < 0 && state.load > 0) {
                // Move to cell (i, j)
                while (state.r < i) {
                    operations.push_back("D");
                    state.r++;
                }
                while (state.r > i) {
                    operations.push_back("U");
                    state.r--;
                }
                while (state.c < j) {
                    operations.push_back("R");
                    state.c++;
                }
                while (state.c > j) {
                    operations.push_back("L");
                    state.c--;
                }
                
                int needed = -h[i][j];
                int amount = min(needed, state.load);
                operations.push_back("-" + to_string(amount));
                h[i][j] += amount;
                state.load -= amount;
            }
        }
    }
    
    for (const string& op : operations) {
        cout << op << "\n";
    }
    
    return 0;
}
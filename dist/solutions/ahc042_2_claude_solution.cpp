#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = 20;
    vector<string> board(N);
    
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }
    
    vector<pair<char, int>> operations;
    
    // Find all oni positions
    vector<pair<int, int>> onis;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'x') {
                onis.push_back({i, j});
            }
        }
    }
    
    // For each oni, find a safe direction and remove it
    for (auto oni : onis) {
        int r = oni.first;
        int c = oni.second;
        
        // Check upward direction
        bool canUp = true;
        for (int i = 0; i < r; i++) {
            if (board[i][c] == 'o') {
                canUp = false;
                break;
            }
        }
        
        if (canUp) {
            // Move column up r+1 times, then down r+1 times
            for (int i = 0; i <= r; i++) {
                operations.push_back({'U', c});
            }
            for (int i = 0; i <= r; i++) {
                operations.push_back({'D', c});
            }
            continue;
        }
        
        // Check downward direction
        bool canDown = true;
        for (int i = r + 1; i < N; i++) {
            if (board[i][c] == 'o') {
                canDown = false;
                break;
            }
        }
        
        if (canDown) {
            // Move column down (N-1-r) times, then up (N-1-r) times
            for (int i = 0; i < N - r; i++) {
                operations.push_back({'D', c});
            }
            for (int i = 0; i < N - r; i++) {
                operations.push_back({'U', c});
            }
            continue;
        }
        
        // Check leftward direction
        bool canLeft = true;
        for (int j = 0; j < c; j++) {
            if (board[r][j] == 'o') {
                canLeft = false;
                break;
            }
        }
        
        if (canLeft) {
            // Move row left c+1 times, then right c+1 times
            for (int i = 0; i <= c; i++) {
                operations.push_back({'L', r});
            }
            for (int i = 0; i <= c; i++) {
                operations.push_back({'R', r});
            }
            continue;
        }
        
        // Check rightward direction (must be valid by problem guarantee)
        // Move row right (N-1-c) times, then left (N-1-c) times
        for (int i = 0; i < N - c; i++) {
            operations.push_back({'R', r});
        }
        for (int i = 0; i < N - c; i++) {
            operations.push_back({'L', r});
        }
    }
    
    for (auto op : operations) {
        cout << op.first << " " << op.second << "\n";
    }
    
    return 0;
}
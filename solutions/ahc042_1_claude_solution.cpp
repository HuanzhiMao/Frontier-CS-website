#include <bits/stdc++.h>
using namespace std;

int main() {
    int N = 20;
    vector<string> board(N);
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }
    
    vector<pair<char, int>> moves;
    
    // Find all oni positions
    vector<pair<int, int>> oni_pos;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'x') {
                oni_pos.push_back({i, j});
            }
        }
    }
    
    // For each oni, find a safe direction and remove it
    for (auto [r, c] : oni_pos) {
        bool up_safe = true, down_safe = true, left_safe = true, right_safe = true;
        
        // Check up direction
        for (int i = 0; i < r; i++) {
            if (board[i][c] == 'o') {
                up_safe = false;
                break;
            }
        }
        
        // Check down direction
        for (int i = r + 1; i < N; i++) {
            if (board[i][c] == 'o') {
                down_safe = false;
                break;
            }
        }
        
        // Check left direction
        for (int j = 0; j < c; j++) {
            if (board[r][j] == 'o') {
                left_safe = false;
                break;
            }
        }
        
        // Check right direction
        for (int j = c + 1; j < N; j++) {
            if (board[r][j] == 'o') {
                right_safe = false;
                break;
            }
        }
        
        // Choose a safe direction and perform moves
        if (up_safe) {
            // Move up r+1 times, then down r+1 times
            for (int i = 0; i <= r; i++) {
                moves.push_back({'U', c});
            }
            for (int i = 0; i <= r; i++) {
                moves.push_back({'D', c});
            }
        } else if (down_safe) {
            // Move down N-1-r times, then up N-1-r times
            for (int i = 0; i < N - 1 - r; i++) {
                moves.push_back({'D', c});
            }
            for (int i = 0; i < N - 1 - r; i++) {
                moves.push_back({'U', c});
            }
        } else if (left_safe) {
            // Move left c+1 times, then right c+1 times
            for (int i = 0; i <= c; i++) {
                moves.push_back({'L', r});
            }
            for (int i = 0; i <= c; i++) {
                moves.push_back({'R', r});
            }
        } else if (right_safe) {
            // Move right N-1-c times, then left N-1-c times
            for (int i = 0; i < N - 1 - c; i++) {
                moves.push_back({'R', r});
            }
            for (int i = 0; i < N - 1 - c; i++) {
                moves.push_back({'L', r});
            }
        }
    }
    
    // Output moves
    for (auto [dir, pos] : moves) {
        cout << dir << " " << pos << "\n";
    }
    
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    vector<string> board(N);
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }
    
    vector<pair<char, int>> moves;
    
    // Find all oni positions
    vector<pair<int, int>> oni_positions;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'x') {
                oni_positions.push_back({i, j});
            }
        }
    }
    
    // For each oni, find a safe direction and remove it
    for (auto [r, c] : oni_positions) {
        // Check upward direction
        bool up_safe = true;
        for (int i = 0; i < r; i++) {
            if (board[i][c] == 'o') {
                up_safe = false;
                break;
            }
        }
        
        if (up_safe) {
            // Shift up r+1 times, then down r+1 times
            for (int i = 0; i <= r; i++) {
                moves.push_back({'U', c});
            }
            for (int i = 0; i <= r; i++) {
                moves.push_back({'D', c});
            }
            continue;
        }
        
        // Check downward direction
        bool down_safe = true;
        for (int i = r + 1; i < N; i++) {
            if (board[i][c] == 'o') {
                down_safe = false;
                break;
            }
        }
        
        if (down_safe) {
            // Shift down (N-1-r)+1 times, then up (N-1-r)+1 times
            int shifts = N - r;
            for (int i = 0; i < shifts; i++) {
                moves.push_back({'D', c});
            }
            for (int i = 0; i < shifts; i++) {
                moves.push_back({'U', c});
            }
            continue;
        }
        
        // Check leftward direction
        bool left_safe = true;
        for (int j = 0; j < c; j++) {
            if (board[r][j] == 'o') {
                left_safe = false;
                break;
            }
        }
        
        if (left_safe) {
            // Shift left c+1 times, then right c+1 times
            for (int i = 0; i <= c; i++) {
                moves.push_back({'L', r});
            }
            for (int i = 0; i <= c; i++) {
                moves.push_back({'R', r});
            }
            continue;
        }
        
        // Check rightward direction
        bool right_safe = true;
        for (int j = c + 1; j < N; j++) {
            if (board[r][j] == 'o') {
                right_safe = false;
                break;
            }
        }
        
        if (right_safe) {
            // Shift right (N-1-c)+1 times, then left (N-1-c)+1 times
            int shifts = N - c;
            for (int i = 0; i < shifts; i++) {
                moves.push_back({'R', r});
            }
            for (int i = 0; i < shifts; i++) {
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
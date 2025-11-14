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
    vector<pair<int, int>> oni_positions;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 'x') {
                oni_positions.push_back({i, j});
            }
        }
    }
    
    // Process each oni
    for (auto [oni_r, oni_c] : oni_positions) {
        // Check upward direction
        bool can_go_up = true;
        for (int i = 0; i < oni_r; i++) {
            if (board[i][oni_c] == 'o') {
                can_go_up = false;
                break;
            }
        }
        
        if (can_go_up) {
            // Move column up oni_r+1 times, then down oni_r+1 times
            for (int t = 0; t <= oni_r; t++) {
                operations.push_back({'U', oni_c});
            }
            for (int t = 0; t <= oni_r; t++) {
                operations.push_back({'D', oni_c});
            }
            continue;
        }
        
        // Check downward direction
        bool can_go_down = true;
        for (int i = oni_r + 1; i < N; i++) {
            if (board[i][oni_c] == 'o') {
                can_go_down = false;
                break;
            }
        }
        
        if (can_go_down) {
            // Move column down (N-1-oni_r)+1 times, then up (N-1-oni_r)+1 times
            int moves = N - oni_r;
            for (int t = 0; t < moves; t++) {
                operations.push_back({'D', oni_c});
            }
            for (int t = 0; t < moves; t++) {
                operations.push_back({'U', oni_c});
            }
            continue;
        }
        
        // Check leftward direction
        bool can_go_left = true;
        for (int j = 0; j < oni_c; j++) {
            if (board[oni_r][j] == 'o') {
                can_go_left = false;
                break;
            }
        }
        
        if (can_go_left) {
            // Move row left oni_c+1 times, then right oni_c+1 times
            for (int t = 0; t <= oni_c; t++) {
                operations.push_back({'L', oni_r});
            }
            for (int t = 0; t <= oni_c; t++) {
                operations.push_back({'R', oni_r});
            }
            continue;
        }
        
        // Check rightward direction
        bool can_go_right = true;
        for (int j = oni_c + 1; j < N; j++) {
            if (board[oni_r][j] == 'o') {
                can_go_right = false;
                break;
            }
        }
        
        if (can_go_right) {
            // Move row right (N-1-oni_c)+1 times, then left (N-1-oni_c)+1 times
            int moves = N - oni_c;
            for (int t = 0; t < moves; t++) {
                operations.push_back({'R', oni_r});
            }
            for (int t = 0; t < moves; t++) {
                operations.push_back({'L', oni_r});
            }
        }
    }
    
    for (auto [dir, pos] : operations) {
        cout << dir << " " << pos << "\n";
    }
    
    return 0;
}
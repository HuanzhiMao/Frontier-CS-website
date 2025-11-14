#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<int> flavors(100);
    for (int i = 0; i < 100; i++) {
        cin >> flavors[i];
    }
    
    int board[10][10];
    memset(board, 0, sizeof(board));
    
    for (int t = 0; t < 100; t++) {
        int p;
        cin >> p;
        p--;
        
        // Find the p-th empty cell
        int empty_count = 0;
        int row = -1, col = -1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (board[i][j] == 0) {
                    if (empty_count == p) {
                        row = i;
                        col = j;
                        break;
                    }
                    empty_count++;
                }
            }
            if (row != -1) break;
        }
        
        // Place the candy
        board[row][col] = flavors[t];
        
        // Simulate all 4 directions and choose the best one
        char best_dir = 'F';
        int best_score = -1;
        
        for (char dir : {'F', 'B', 'L', 'R'}) {
            // Create a copy of the board
            int temp_board[10][10];
            memcpy(temp_board, board, sizeof(board));
            
            // Apply the tilt
            if (dir == 'F') {
                for (int j = 0; j < 10; j++) {
                    int write_pos = 0;
                    for (int i = 0; i < 10; i++) {
                        if (temp_board[i][j] != 0) {
                            temp_board[write_pos][j] = temp_board[i][j];
                            if (write_pos != i) temp_board[i][j] = 0;
                            write_pos++;
                        }
                    }
                }
            } else if (dir == 'B') {
                for (int j = 0; j < 10; j++) {
                    int write_pos = 9;
                    for (int i = 9; i >= 0; i--) {
                        if (temp_board[i][j] != 0) {
                            temp_board[write_pos][j] = temp_board[i][j];
                            if (write_pos != i) temp_board[i][j] = 0;
                            write_pos--;
                        }
                    }
                }
            } else if (dir == 'L') {
                for (int i = 0; i < 10; i++) {
                    int write_pos = 0;
                    for (int j = 0; j < 10; j++) {
                        if (temp_board[i][j] != 0) {
                            temp_board[i][write_pos] = temp_board[i][j];
                            if (write_pos != j) temp_board[i][j] = 0;
                            write_pos++;
                        }
                    }
                }
            } else if (dir == 'R') {
                for (int i = 0; i < 10; i++) {
                    int write_pos = 9;
                    for (int j = 9; j >= 0; j--) {
                        if (temp_board[i][j] != 0) {
                            temp_board[i][write_pos] = temp_board[i][j];
                            if (write_pos != j) temp_board[i][j] = 0;
                            write_pos--;
                        }
                    }
                }
            }
            
            // Calculate connected components score
            bool visited[10][10];
            memset(visited, false, sizeof(visited));
            int total_score = 0;
            
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    if (temp_board[i][j] != 0 && !visited[i][j]) {
                        int component_size = 0;
                        queue<pair<int, int>> q;
                        q.push({i, j});
                        visited[i][j] = true;
                        int flavor = temp_board[i][j];
                        
                        while (!q.empty()) {
                            auto [x, y] = q.front();
                            q.pop();
                            component_size++;
                            
                            int dx[] = {-1, 1, 0, 0};
                            int dy[] = {0, 0, -1, 1};
                            for (int d = 0; d < 4; d++) {
                                int nx = x + dx[d];
                                int ny = y + dy[d];
                                if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && 
                                    !visited[nx][ny] && temp_board[nx][ny] == flavor) {
                                    visited[nx][ny] = true;
                                    q.push({nx, ny});
                                }
                            }
                        }
                        total_score += component_size * component_size;
                    }
                }
            }
            
            if (total_score > best_score) {
                best_score = total_score;
                best_dir = dir;
            }
        }
        
        // Apply the best move to the actual board
        if (best_dir == 'F') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 0;
                for (int i = 0; i < 10; i++) {
                    if (board[i][j] != 0) {
                        board[write_pos][j] = board[i][j];
                        if (write_pos != i) board[i][j] = 0;
                        write_pos++;
                    }
                }
            }
        } else if (best_dir == 'B') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 9;
                for (int i = 9; i >= 0; i--) {
                    if (board[i][j] != 0) {
                        board[write_pos][j] = board[i][j];
                        if (write_pos != i) board[i][j] = 0;
                        write_pos--;
                    }
                }
            }
        } else if (best_dir == 'L') {
            for (int i = 0; i < 10; i++) {
                int write_pos = 0;
                for (int j = 0; j < 10; j++) {
                    if (board[i][j] != 0) {
                        board[i][write_pos] = board[i][j];
                        if (write_pos != j) board[i][j] = 0;
                        write_pos++;
                    }
                }
            }
        } else if (best_dir == 'R') {
            for (int i = 0; i < 10; i++) {
                int write_pos = 9;
                for (int j = 9; j >= 0; j--) {
                    if (board[i][j] != 0) {
                        board[i][write_pos] = board[i][j];
                        if (write_pos != j) board[i][j] = 0;
                        write_pos--;
                    }
                }
            }
        }
        
        cout << best_dir << "\n" << flush;
    }
    
    return 0;
}
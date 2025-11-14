#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<int> flavors(100);
    for (int i = 0; i < 100; i++) {
        cin >> flavors[i];
    }
    
    vector<vector<int>> board(10, vector<int>(10, 0));
    vector<char> directions = {'F', 'B', 'L', 'R'};
    
    for (int t = 0; t < 100; t++) {
        int p;
        cin >> p;
        
        // Find the p-th empty cell
        int count = 0;
        int row = -1, col = -1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (board[i][j] == 0) {
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
        board[row][col] = flavors[t];
        
        // Try all 4 directions and pick the best one
        char best_dir = 'F';
        double best_score = -1;
        
        for (char dir : directions) {
            // Simulate the tilt
            vector<vector<int>> temp_board = board;
            
            if (dir == 'F') {
                for (int j = 0; j < 10; j++) {
                    int write_pos = 0;
                    for (int i = 0; i < 10; i++) {
                        if (temp_board[i][j] != 0) {
                            temp_board[write_pos][j] = temp_board[i][j];
                            if (write_pos != i) {
                                temp_board[i][j] = 0;
                            }
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
                            if (write_pos != i) {
                                temp_board[i][j] = 0;
                            }
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
                            if (write_pos != j) {
                                temp_board[i][j] = 0;
                            }
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
                            if (write_pos != j) {
                                temp_board[i][j] = 0;
                            }
                            write_pos--;
                        }
                    }
                }
            }
            
            // Calculate score for this direction
            vector<vector<bool>> visited(10, vector<bool>(10, false));
            double score = 0;
            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};
            
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    if (temp_board[i][j] != 0 && !visited[i][j]) {
                        int component_size = 0;
                        queue<pair<int, int>> q;
                        q.push({i, j});
                        visited[i][j] = true;
                        
                        while (!q.empty()) {
                            auto [x, y] = q.front();
                            q.pop();
                            component_size++;
                            
                            for (int k = 0; k < 4; k++) {
                                int nx = x + dx[k];
                                int ny = y + dy[k];
                                if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && 
                                    !visited[nx][ny] && 
                                    temp_board[nx][ny] == temp_board[i][j]) {
                                    visited[nx][ny] = true;
                                    q.push({nx, ny});
                                }
                            }
                        }
                        score += component_size * component_size;
                    }
                }
            }
            
            if (score > best_score) {
                best_score = score;
                best_dir = dir;
            }
        }
        
        cout << best_dir << "\n";
        cout.flush();
        
        // Apply the chosen direction to the actual board
        if (best_dir == 'F') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 0;
                for (int i = 0; i < 10; i++) {
                    if (board[i][j] != 0) {
                        board[write_pos][j] = board[i][j];
                        if (write_pos != i) {
                            board[i][j] = 0;
                        }
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
                        if (write_pos != i) {
                            board[i][j] = 0;
                        }
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
                        if (write_pos != j) {
                            board[i][j] = 0;
                        }
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
                        if (write_pos != j) {
                            board[i][j] = 0;
                        }
                        write_pos--;
                    }
                }
            }
        }
    }
    
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int N, T;
vector<string> board;
int empty_r, empty_c;
string moves = "";

int dx[] = {-1, 1, 0, 0}; // U, D, L, R
int dy[] = {0, 0, -1, 1};
char move_chars[] = {'U', 'D', 'L', 'R'};

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void find_empty() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '0') {
                empty_r = i;
                empty_c = j;
                return;
            }
        }
    }
}

void make_move(int dir) {
    int nr = empty_r + dx[dir];
    int nc = empty_c + dy[dir];
    
    if (is_valid(nr, nc)) {
        swap(board[empty_r][empty_c], board[nr][nc]);
        empty_r = nr;
        empty_c = nc;
        moves += move_chars[dir];
    }
}

int get_tile_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

bool has_connection(int r1, int c1, int r2, int c2) {
    if (!is_valid(r1, c1) || !is_valid(r2, c2)) return false;
    if (board[r1][c1] == '0' || board[r2][c2] == '0') return false;
    
    int val1 = get_tile_value(board[r1][c1]);
    int val2 = get_tile_value(board[r2][c2]);
    
    if (r1 == r2 - 1 && c1 == c2) {
        return (val1 & 8) && (val2 & 2);
    }
    if (r1 == r2 && c1 == c2 - 1) {
        return (val1 & 4) && (val2 & 1);
    }
    return false;
}

int calculate_largest_tree() {
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    int max_size = 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] != '0' && !visited[i][j]) {
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int size = 1;
                
                while (!q.empty()) {
                    auto [r, c] = q.front();
                    q.pop();
                    
                    if (has_connection(r, c, r+1, c) && !visited[r+1][c]) {
                        visited[r+1][c] = true;
                        q.push({r+1, c});
                        size++;
                    }
                    if (has_connection(r, c, r, c+1) && !visited[r][c+1]) {
                        visited[r][c+1] = true;
                        q.push({r, c+1});
                        size++;
                    }
                    if (has_connection(r-1, c, r, c) && !visited[r-1][c]) {
                        visited[r-1][c] = true;
                        q.push({r-1, c});
                        size++;
                    }
                    if (has_connection(r, c-1, r, c) && !visited[r][c-1]) {
                        visited[r][c-1] = true;
                        q.push({r, c-1});
                        size++;
                    }
                }
                
                max_size = max(max_size, size);
            }
        }
    }
    
    return max_size;
}

int main() {
    cin >> N >> T;
    board.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }
    
    find_empty();
    
    // Simple greedy approach: try to move empty space to bottom-right
    while (moves.size() < T && (empty_r != N-1 || empty_c != N-1)) {
        if (empty_r < N-1) {
            make_move(1); // Down
        } else if (empty_c < N-1) {
            make_move(3); // Right
        } else {
            break;
        }
    }
    
    cout << moves << endl;
    
    return 0;
}
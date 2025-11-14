#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<string> board;
    int empty_r, empty_c;
    string moves;
    
    State(vector<string> b, int er, int ec, string m) : board(b), empty_r(er), empty_c(ec), moves(m) {}
};

int N, T;
vector<string> initial_board;
int initial_empty_r, initial_empty_c;

int dx[] = {-1, 1, 0, 0}; // U, D, L, R
int dy[] = {0, 0, -1, 1};
string dir_chars = "UDLR";

int char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

bool is_valid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

int count_tree_size(const vector<string>& board) {
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    int max_tree_size = 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '0' || visited[i][j]) continue;
            
            // BFS to find connected component
            queue<pair<int, int>> q;
            q.push({i, j});
            visited[i][j] = true;
            int component_size = 1;
            bool has_cycle = false;
            vector<pair<int, int>> edges;
            
            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                
                int tile = char_to_int(board[r][c]);
                
                // Check connections
                for (int d = 0; d < 4; d++) {
                    if (!(tile & (1 << d))) continue;
                    
                    int nr = r + dx[d];
                    int nc = c + dy[d];
                    
                    if (!is_valid(nr, nc) || board[nr][nc] == '0') continue;
                    
                    int neighbor_tile = char_to_int(board[nr][nc]);
                    int opposite_dir = d ^ 1;
                    
                    if (!(neighbor_tile & (1 << opposite_dir))) continue;
                    
                    edges.push_back({min(r * N + c, nr * N + nc), max(r * N + c, nr * N + nc)});
                    
                    if (!visited[nr][nc]) {
                        visited[nr][nc] = true;
                        q.push({nr, nc});
                        component_size++;
                    }
                }
            }
            
            // Check for cycles
            sort(edges.begin(), edges.end());
            edges.erase(unique(edges.begin(), edges.end()), edges.end());
            
            if (edges.size() >= component_size) {
                has_cycle = true;
            }
            
            if (!has_cycle) {
                max_tree_size = max(max_tree_size, component_size);
            }
        }
    }
    
    return max_tree_size;
}

State make_move(const State& state, int dir) {
    int new_r = state.empty_r + dx[dir];
    int new_c = state.empty_c + dy[dir];
    
    if (!is_valid(new_r, new_c)) {
        return state; // Invalid move
    }
    
    vector<string> new_board = state.board;
    new_board[state.empty_r][state.empty_c] = new_board[new_r][new_c];
    new_board[new_r][new_c] = '0';
    
    return State(new_board, new_r, new_c, state.moves + dir_chars[dir]);
}

int main() {
    cin >> N >> T;
    
    initial_board.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> initial_board[i];
        for (int j = 0; j < N; j++) {
            if (initial_board[i][j] == '0') {
                initial_empty_r = i;
                initial_empty_c = j;
            }
        }
    }
    
    // Simple greedy approach with random moves
    State current(initial_board, initial_empty_r, initial_empty_c, "");
    int best_score = count_tree_size(current.board);
    State best_state = current;
    
    srand(42);
    
    for (int iter = 0; iter < T && current.moves.length() < T; iter++) {
        vector<int> valid_dirs;
        
        for (int d = 0; d < 4; d++) {
            int new_r = current.empty_r + dx[d];
            int new_c = current.empty_c + dy[d];
            if (is_valid(new_r, new_c)) {
                valid_dirs.push_back(d);
            }
        }
        
        if (valid_dirs.empty()) break;
        
        // Try all valid directions and pick the best
        State next_state = current;
        int next_score = best_score;
        
        for (int d : valid_dirs) {
            State temp = make_move(current, d);
            int temp_score = count_tree_size(temp.board);
            
            if (temp_score > next_score || (temp_score == next_score && rand() % 2)) {
                next_state = temp;
                next_score = temp_score;
            }
        }
        
        // If no improvement, make a random move
        if (next_state.moves == current.moves) {
            int random_dir = valid_dirs[rand() % valid_dirs.size()];
            next_state = make_move(current, random_dir);
            next_score = count_tree_size(next_state.board);
        }
        
        current = next_state;
        
        if (next_score > best_score) {
            best_score = next_score;
            best_state = current;
        }
        
        // Early termination if perfect score achieved
        if (best_score == N * N - 1) {
            break;
        }
    }
    
    cout << best_state.moves << endl;
    
    return 0;
}
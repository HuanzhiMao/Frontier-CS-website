#include <bits/stdc++.h>
using namespace std;

int N, T;
vector<string> board;
int empty_r, empty_c;
string result;

vector<int> dr = {-1, 1, 0, 0};
vector<int> dc = {0, 0, -1, 1};
string moves = "UDLR";

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void move(char dir) {
    int idx = moves.find(dir);
    int nr = empty_r + dr[idx];
    int nc = empty_c + dc[idx];
    
    if (isValid(nr, nc)) {
        board[empty_r][empty_c] = board[nr][nc];
        board[nr][nc] = '0';
        empty_r = nr;
        empty_c = nc;
        result += dir;
    }
}

bool hasConnection(int r, int c, int dir) {
    int val = 0;
    if (board[r][c] >= '0' && board[r][c] <= '9') {
        val = board[r][c] - '0';
    } else {
        val = board[r][c] - 'a' + 10;
    }
    return (val & (1 << dir)) != 0;
}

int countTreeSize() {
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    vector<vector<int>> component(N, vector<int>(N, -1));
    int maxTreeSize = 0;
    int compId = 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '0' || visited[i][j]) continue;
            
            queue<pair<int, int>> q;
            q.push({i, j});
            visited[i][j] = true;
            component[i][j] = compId;
            int size = 1;
            bool hasCycle = false;
            set<pair<pair<int,int>, pair<int,int>>> edges;
            
            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                
                // Check down
                if (r + 1 < N && board[r+1][c] != '0') {
                    if (hasConnection(r, c, 3) && hasConnection(r+1, c, 1)) {
                        pair<int,int> p1 = {r, c}, p2 = {r+1, c};
                        if (p1 > p2) swap(p1, p2);
                        edges.insert({p1, p2});
                        
                        if (!visited[r+1][c]) {
                            visited[r+1][c] = true;
                            component[r+1][c] = compId;
                            q.push({r+1, c});
                            size++;
                        }
                    }
                }
                
                // Check right
                if (c + 1 < N && board[r][c+1] != '0') {
                    if (hasConnection(r, c, 2) && hasConnection(r, c+1, 0)) {
                        pair<int,int> p1 = {r, c}, p2 = {r, c+1};
                        if (p1 > p2) swap(p1, p2);
                        edges.insert({p1, p2});
                        
                        if (!visited[r][c+1]) {
                            visited[r][c+1] = true;
                            component[r][c+1] = compId;
                            q.push({r, c+1});
                            size++;
                        }
                    }
                }
            }
            
            if (edges.size() >= size) hasCycle = true;
            
            if (!hasCycle) {
                maxTreeSize = max(maxTreeSize, size);
            }
            compId++;
        }
    }
    
    return maxTreeSize;
}

int main() {
    cin >> N >> T;
    board.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> board[i];
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '0') {
                empty_r = i;
                empty_c = j;
            }
        }
    }
    
    // Simple strategy: try to move empty space to bottom-right corner
    // while maximizing tree size
    
    while (result.length() < T) {
        int bestTreeSize = countTreeSize();
        char bestMove = 0;
        
        for (char dir : moves) {
            int idx = moves.find(dir);
            int nr = empty_r + dr[idx];
            int nc = empty_c + dc[idx];
            
            if (isValid(nr, nc)) {
                // Try this move
                char temp = board[nr][nc];
                board[empty_r][empty_c] = temp;
                board[nr][nc] = '0';
                int old_r = empty_r, old_c = empty_c;
                empty_r = nr;
                empty_c = nc;
                
                int treeSize = countTreeSize();
                if (treeSize > bestTreeSize) {
                    bestTreeSize = treeSize;
                    bestMove = dir;
                }
                
                // Revert
                board[nr][nc] = temp;
                board[old_r][old_c] = '0';
                empty_r = old_r;
                empty_c = old_c;
            }
        }
        
        if (bestMove) {
            move(bestMove);
        } else {
            // No improvement found, try moving towards bottom-right
            if (empty_r < N-1 && isValid(empty_r + 1, empty_c)) {
                move('D');
            } else if (empty_c < N-1 && isValid(empty_r, empty_c + 1)) {
                move('R');
            } else {
                // Random move
                vector<char> validMoves;
                for (char dir : moves) {
                    int idx = moves.find(dir);
                    int nr = empty_r + dr[idx];
                    int nc = empty_c + dc[idx];
                    if (isValid(nr, nc)) {
                        validMoves.push_back(dir);
                    }
                }
                if (!validMoves.empty()) {
                    move(validMoves[0]);
                } else {
                    break;
                }
            }
        }
        
        if (countTreeSize() == N*N - 1) break;
    }
    
    cout << result << endl;
    return 0;
}
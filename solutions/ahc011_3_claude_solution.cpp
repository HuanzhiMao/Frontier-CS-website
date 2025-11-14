#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<string> board;
    int empty_r, empty_c;
    int N;
    
    State(int n) : N(n), board(n, string(n, '0')) {}
    
    bool canMove(char dir) {
        int nr = empty_r, nc = empty_c;
        if (dir == 'U') nr--;
        else if (dir == 'D') nr++;
        else if (dir == 'L') nc--;
        else if (dir == 'R') nc++;
        return nr >= 0 && nr < N && nc >= 0 && nc < N;
    }
    
    void move(char dir) {
        int nr = empty_r, nc = empty_c;
        if (dir == 'U') nr--;
        else if (dir == 'D') nr++;
        else if (dir == 'L') nc--;
        else if (dir == 'R') nc++;
        
        swap(board[empty_r][empty_c], board[nr][nc]);
        empty_r = nr;
        empty_c = nc;
    }
    
    int getValue(int r, int c) {
        char ch = board[r][c];
        if (ch >= '0' && ch <= '9') return ch - '0';
        return ch - 'a' + 10;
    }
    
    bool hasConnection(int r1, int c1, int r2, int c2) {
        if (board[r1][c1] == '0' || board[r2][c2] == '0') return false;
        
        int val1 = getValue(r1, c1);
        int val2 = getValue(r2, c2);
        
        if (r1 == r2 - 1 && c1 == c2) { // r1 above r2
            return (val1 & 8) && (val2 & 2); // r1 has down, r2 has up
        }
        if (r1 == r2 && c1 == c2 - 1) { // r1 left of r2
            return (val1 & 4) && (val2 & 1); // r1 has right, r2 has left
        }
        return false;
    }
    
    int getTreeSize() {
        vector<vector<bool>> visited(N, vector<bool>(N, false));
        vector<vector<int>> component(N, vector<int>(N, -1));
        vector<int> compSize;
        int compCount = 0;
        
        // Find connected components
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] != '0' && !visited[i][j]) {
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    component[i][j] = compCount;
                    int size = 0;
                    
                    while (!q.empty()) {
                        auto [r, c] = q.front();
                        q.pop();
                        size++;
                        
                        // Check 4 directions
                        vector<pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                        for (auto [dr, dc] : dirs) {
                            int nr = r + dr, nc = c + dc;
                            if (nr >= 0 && nr < N && nc >= 0 && nc < N && 
                                !visited[nr][nc] && board[nr][nc] != '0') {
                                if (hasConnection(r, c, nr, nc) || hasConnection(nr, nc, r, c)) {
                                    visited[nr][nc] = true;
                                    component[nr][nc] = compCount;
                                    q.push({nr, nc});
                                }
                            }
                        }
                    }
                    compSize.push_back(size);
                    compCount++;
                }
            }
        }
        
        // Check if components are trees (no cycles)
        vector<bool> isTree(compCount, true);
        vector<int> edgeCount(compCount, 0);
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[i][j] != '0') {
                    // Check down
                    if (i + 1 < N && hasConnection(i, j, i + 1, j)) {
                        edgeCount[component[i][j]]++;
                    }
                    // Check right
                    if (j + 1 < N && hasConnection(i, j, i, j + 1)) {
                        edgeCount[component[i][j]]++;
                    }
                }
            }
        }
        
        for (int i = 0; i < compCount; i++) {
            if (edgeCount[i] != compSize[i] - 1) {
                isTree[i] = false;
            }
        }
        
        int maxTreeSize = 0;
        for (int i = 0; i < compCount; i++) {
            if (isTree[i]) {
                maxTreeSize = max(maxTreeSize, compSize[i]);
            }
        }
        
        return maxTreeSize;
    }
};

int main() {
    int N, T;
    cin >> N >> T;
    
    State state(N);
    for (int i = 0; i < N; i++) {
        cin >> state.board[i];
        for (int j = 0; j < N; j++) {
            if (state.board[i][j] == '0') {
                state.empty_r = i;
                state.empty_c = j;
            }
        }
    }
    
    string result = "";
    vector<char> moves = {'U', 'D', 'L', 'R'};
    
    // Simple greedy approach
    for (int iter = 0; iter < T && result.length() < T; iter++) {
        int bestScore = state.getTreeSize();
        char bestMove = 0;
        
        for (char move : moves) {
            if (state.canMove(move)) {
                State temp = state;
                temp.move(move);
                int score = temp.getTreeSize();
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
        }
        
        if (bestMove != 0) {
            state.move(bestMove);
            result += bestMove;
        } else {
            // Random move if no improvement
            vector<char> validMoves;
            for (char move : moves) {
                if (state.canMove(move)) {
                    validMoves.push_back(move);
                }
            }
            if (!validMoves.empty()) {
                char randomMove = validMoves[rand() % validMoves.size()];
                state.move(randomMove);
                result += randomMove;
            } else {
                break;
            }
        }
        
        if (state.getTreeSize() == N * N - 1) break;
    }
    
    cout << result << endl;
    return 0;
}
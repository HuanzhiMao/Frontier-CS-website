#include <bits/stdc++.h>
using namespace std;

int N, T;
vector<string> board;
int emptyR, emptyC;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
char moves[] = {'U', 'D', 'L', 'R'};

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

void findEmpty() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '0') {
                emptyR = i;
                emptyC = j;
                return;
            }
        }
    }
}

void makeMove(char move) {
    int newR = emptyR, newC = emptyC;
    if (move == 'U') newR--;
    else if (move == 'D') newR++;
    else if (move == 'L') newC--;
    else if (move == 'R') newC++;
    
    swap(board[emptyR][emptyC], board[newR][newC]);
    emptyR = newR;
    emptyC = newC;
}

int hexToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

bool hasConnection(int r1, int c1, int r2, int c2) {
    if (board[r1][c1] == '0' || board[r2][c2] == '0') return false;
    
    int val1 = hexToInt(board[r1][c1]);
    int val2 = hexToInt(board[r2][c2]);
    
    if (r1 == r2 - 1) { // r1 above r2
        return (val1 & 8) && (val2 & 2); // down & up
    }
    if (r1 == r2 + 1) { // r1 below r2
        return (val1 & 2) && (val2 & 8); // up & down
    }
    if (c1 == c2 - 1) { // r1 left of r2
        return (val1 & 4) && (val2 & 1); // right & left
    }
    if (c1 == c2 + 1) { // r1 right of r2
        return (val1 & 1) && (val2 & 4); // left & right
    }
    return false;
}

int getLargestTreeSize() {
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    int maxSize = 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == '0' || visited[i][j]) continue;
            
            queue<pair<int, int>> q;
            q.push({i, j});
            visited[i][j] = true;
            int size = 1;
            
            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                
                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d];
                    int nc = c + dy[d];
                    
                    if (!isValid(nr, nc) || visited[nr][nc]) continue;
                    if (hasConnection(r, c, nr, nc)) {
                        visited[nr][nc] = true;
                        q.push({nr, nc});
                        size++;
                    }
                }
            }
            maxSize = max(maxSize, size);
        }
    }
    return maxSize;
}

int main() {
    cin >> N >> T;
    board.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> board[i];
    }
    
    findEmpty();
    string result = "";
    
    // Try to move empty to bottom right
    while (emptyR < N - 1) {
        result += 'D';
        makeMove('D');
    }
    while (emptyC < N - 1) {
        result += 'R';
        makeMove('R');
    }
    
    cout << result << endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<bool>> visited;
vector<pair<int, int>> path;
vector<pair<int, int>> bestPath;
int maxLength = 0;

int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};

bool isValid(int x, int y) {
    return x >= 1 && x <= n && y >= 1 && y <= n && !visited[x][y];
}

int countMoves(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
            count++;
        }
    }
    return count;
}

void dfs(int x, int y, int length) {
    if (length > maxLength) {
        maxLength = length;
        bestPath = path;
    }
    
    vector<pair<int, pair<int, int>>> moves;
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
            int count = countMoves(nx, ny);
            moves.push_back({count, {nx, ny}});
        }
    }
    
    sort(moves.begin(), moves.end());
    
    for (auto move : moves) {
        int nx = move.second.first;
        int ny = move.second.second;
        
        visited[nx][ny] = true;
        path.push_back({nx, ny});
        
        dfs(nx, ny, length + 1);
        
        visited[nx][ny] = false;
        path.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int r, c;
    cin >> n >> r >> c;
    
    visited.assign(n + 1, vector<bool>(n + 1, false));
    
    visited[r][c] = true;
    path.push_back({r, c});
    
    dfs(r, c, 1);
    
    cout << maxLength << "\n";
    for (auto pos : bestPath) {
        cout << pos.first << " " << pos.second << "\n";
    }
    
    return 0;
}
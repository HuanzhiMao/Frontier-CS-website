#include <bits/stdc++.h>
using namespace std;

int N;
bool visited[667][667];
vector<pair<int, int>> path;
vector<pair<int, int>> bestPath;
int maxLength = 0;

int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};

bool isValid(int x, int y) {
    return x >= 1 && x <= N && y >= 1 && y <= N && !visited[x][y];
}

int countMoves(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) count++;
    }
    return count;
}

void dfs(int x, int y, int depth) {
    if (depth > maxLength) {
        maxLength = depth;
        bestPath = path;
    }
    
    vector<pair<int, pair<int, int>>> moves;
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
            moves.push_back({countMoves(nx, ny), {nx, ny}});
        }
    }
    
    sort(moves.begin(), moves.end());
    
    for (auto move : moves) {
        int nx = move.second.first;
        int ny = move.second.second;
        
        visited[nx][ny] = true;
        path.push_back({nx, ny});
        dfs(nx, ny, depth + 1);
        path.pop_back();
        visited[nx][ny] = false;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int r, c;
    cin >> N >> r >> c;
    
    memset(visited, false, sizeof(visited));
    visited[r][c] = true;
    path.push_back({r, c});
    bestPath.push_back({r, c});
    maxLength = 1;
    
    dfs(r, c, 1);
    
    cout << maxLength << "\n";
    for (auto pos : bestPath) {
        cout << pos.first << " " << pos.second << "\n";
    }
    
    return 0;
}
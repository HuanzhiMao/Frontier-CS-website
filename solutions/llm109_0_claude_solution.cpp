#include <bits/stdc++.h>
using namespace std;

int n;
int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};
bool visited[667][667];
vector<pair<int, int>> path;
vector<pair<int, int>> best_path;
int max_length = 0;

bool isValid(int x, int y) {
    return x >= 1 && x <= n && y >= 1 && y <= n && !visited[x][y];
}

int getDegree(int x, int y) {
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
    if (length > max_length) {
        max_length = length;
        best_path = path;
    }
    
    if (length == n * n) {
        return;
    }
    
    vector<pair<int, pair<int, int>>> moves;
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
            int degree = getDegree(nx, ny);
            moves.push_back({degree, {nx, ny}});
        }
    }
    
    sort(moves.begin(), moves.end());
    
    for (auto move : moves) {
        int nx = move.second.first;
        int ny = move.second.second;
        
        visited[nx][ny] = true;
        path.push_back({nx, ny});
        dfs(nx, ny, length + 1);
        path.pop_back();
        visited[nx][ny] = false;
        
        if (max_length == n * n) return;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int start_r, start_c;
    cin >> n >> start_r >> start_c;
    
    memset(visited, false, sizeof(visited));
    visited[start_r][start_c] = true;
    path.push_back({start_r, start_c});
    
    dfs(start_r, start_c, 1);
    
    cout << max_length << "\n";
    for (auto pos : best_path) {
        cout << pos.first << " " << pos.second << "\n";
    }
    
    return 0;
}
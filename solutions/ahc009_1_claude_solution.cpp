#include <bits/stdc++.h>
using namespace std;

int main() {
    int si, sj, ti, tj;
    double p;
    cin >> si >> sj >> ti >> tj >> p;
    
    vector<string> h(20), v(19);
    for (int i = 0; i < 20; i++) {
        cin >> h[i];
    }
    for (int i = 0; i < 19; i++) {
        cin >> v[i];
    }
    
    // Simple approach: find shortest path and repeat it multiple times
    vector<vector<int>> dist(20, vector<int>(20, -1));
    vector<vector<pair<int, int>>> parent(20, vector<pair<int, int>>(20, {-1, -1}));
    
    queue<pair<int, int>> q;
    q.push({si, sj});
    dist[si][sj] = 0;
    
    int dx[] = {-1, 1, 0, 0}; // U, D, L, R
    int dy[] = {0, 0, -1, 1};
    string dirs = "UDLR";
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            if (nx < 0 || nx >= 20 || ny < 0 || ny >= 20) continue;
            if (dist[nx][ny] != -1) continue;
            
            // Check wall
            bool blocked = false;
            if (d == 0 && x > 0) blocked = (v[x-1][y] == '1'); // Up
            if (d == 1 && x < 19) blocked = (v[x][y] == '1'); // Down
            if (d == 2 && y > 0) blocked = (h[x][y-1] == '1'); // Left
            if (d == 3 && y < 19) blocked = (h[x][y] == '1'); // Right
            
            if (blocked) continue;
            
            dist[nx][ny] = dist[x][y] + 1;
            parent[nx][ny] = {x, y};
            q.push({nx, ny});
        }
    }
    
    // Reconstruct path
    string path = "";
    int cx = ti, cy = tj;
    while (cx != si || cy != sj) {
        int px = parent[cx][cy].first;
        int py = parent[cx][cy].second;
        
        if (cx == px - 1) path += "U";
        else if (cx == px + 1) path += "D";
        else if (cy == py - 1) path += "L";
        else if (cy == py + 1) path += "R";
        
        cx = px;
        cy = py;
    }
    
    reverse(path.begin(), path.end());
    
    // Repeat the path multiple times to increase robustness
    string result = "";
    int repetitions = min(200 / (int)path.length(), 10);
    if (repetitions == 0) repetitions = 1;
    
    for (int i = 0; i < repetitions && result.length() + path.length() <= 200; i++) {
        result += path;
    }
    
    cout << result << endl;
    
    return 0;
}
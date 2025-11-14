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
    
    // BFS to find shortest path
    queue<pair<int, int>> q;
    vector<vector<int>> dist(20, vector<int>(20, -1));
    vector<vector<pair<int, int>>> parent(20, vector<pair<int, int>>(20, {-1, -1}));
    
    q.push({si, sj});
    dist[si][sj] = 0;
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    char dir[] = {'U', 'D', 'L', 'R'};
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        if (x == ti && y == tj) break;
        
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            if (nx < 0 || nx >= 20 || ny < 0 || ny >= 20) continue;
            if (dist[nx][ny] != -1) continue;
            
            bool wall = false;
            if (d == 0 && x > 0 && v[x-1][y] == '1') wall = true;
            if (d == 1 && x < 19 && v[x][y] == '1') wall = true;
            if (d == 2 && y > 0 && h[x][y-1] == '1') wall = true;
            if (d == 3 && y < 19 && h[x][y] == '1') wall = true;
            
            if (wall) continue;
            
            dist[nx][ny] = dist[x][y] + 1;
            parent[nx][ny] = {x, y};
            q.push({nx, ny});
        }
    }
    
    // Reconstruct path
    vector<char> path;
    int cx = ti, cy = tj;
    while (cx != si || cy != sj) {
        int px = parent[cx][cy].first;
        int py = parent[cx][cy].second;
        
        for (int d = 0; d < 4; d++) {
            if (px + dx[d] == cx && py + dy[d] == cy) {
                path.push_back(dir[d]);
                break;
            }
        }
        cx = px;
        cy = py;
    }
    
    reverse(path.begin(), path.end());
    
    // Add redundancy to make it more robust
    string result;
    double redundancy_factor = min(3.0, 200.0 / path.size());
    int repeat_count = max(1, (int)(redundancy_factor));
    
    for (int i = 0; i < repeat_count && result.size() + path.size() <= 200; i++) {
        for (char c : path) {
            if (result.size() < 200) {
                result += c;
            }
        }
    }
    
    cout << result << endl;
    
    return 0;
}
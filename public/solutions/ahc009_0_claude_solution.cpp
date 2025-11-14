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
    vector<vector<int>> dist(20, vector<int>(20, -1));
    vector<vector<pair<int,int>>> parent(20, vector<pair<int,int>>(20, {-1, -1}));
    queue<pair<int,int>> q;
    
    q.push({si, sj});
    dist[si][sj] = 0;
    
    int dx[] = {-1, 1, 0, 0}; // U, D, L, R
    int dy[] = {0, 0, -1, 1};
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        if (x == ti && y == tj) break;
        
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            if (nx < 0 || nx >= 20 || ny < 0 || ny >= 20) continue;
            if (dist[nx][ny] != -1) continue;
            
            bool blocked = false;
            if (d == 0 && x > 0 && v[x-1][y] == '1') blocked = true; // Up
            if (d == 1 && x < 19 && v[x][y] == '1') blocked = true;   // Down
            if (d == 2 && y > 0 && h[x][y-1] == '1') blocked = true; // Left
            if (d == 3 && y < 19 && h[x][y] == '1') blocked = true;  // Right
            
            if (!blocked) {
                dist[nx][ny] = dist[x][y] + 1;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
            }
        }
    }
    
    // Reconstruct path
    string path = "";
    int cx = ti, cy = tj;
    vector<char> moves;
    
    while (parent[cx][cy].first != -1) {
        int px = parent[cx][cy].first;
        int py = parent[cx][cy].second;
        
        if (cx > px) moves.push_back('D');
        else if (cx < px) moves.push_back('U');
        else if (cy > py) moves.push_back('R');
        else moves.push_back('L');
        
        cx = px;
        cy = py;
    }
    
    reverse(moves.begin(), moves.end());
    
    // Create robust path by adding redundancy
    string result = "";
    for (char c : moves) {
        result += c;
        // Add extra moves to compensate for forgetting with probability p
        // The more likely to forget (higher p), the more redundancy we add
        int extra = (int)(p * 3) + 1;
        for (int i = 0; i < extra && result.length() < 200; i++) {
            result += c;
        }
    }
    
    // Ensure we don't exceed 200 characters
    if (result.length() > 200) {
        result = result.substr(0, 200);
    }
    
    cout << result << endl;
    
    return 0;
}
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
    
    // Simple strategy: repeat the shortest path multiple times
    string path = "";
    
    // Calculate shortest path using BFS
    queue<pair<int, int>> q;
    vector<vector<int>> dist(20, vector<int>(20, -1));
    vector<vector<pair<int, int>>> parent(20, vector<pair<int, int>>(20, {-1, -1}));
    
    q.push({si, sj});
    dist[si][sj] = 0;
    
    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};
    char dirs[] = {'U', 'D', 'L', 'R'};
    
    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop();
        
        for (int d = 0; d < 4; d++) {
            int ni = i + di[d];
            int nj = j + dj[d];
            
            if (ni < 0 || ni >= 20 || nj < 0 || nj >= 20) continue;
            if (dist[ni][nj] != -1) continue;
            
            bool blocked = false;
            if (d == 0 && i > 0 && v[i-1][j] == '1') blocked = true;
            if (d == 1 && i < 19 && v[i][j] == '1') blocked = true;
            if (d == 2 && j > 0 && h[i][j-1] == '1') blocked = true;
            if (d == 3 && j < 19 && h[i][j] == '1') blocked = true;
            
            if (!blocked) {
                dist[ni][nj] = dist[i][j] + 1;
                parent[ni][nj] = {i, j};
                q.push({ni, nj});
            }
        }
    }
    
    // Reconstruct path
    string shortest = "";
    int ci = ti, cj = tj;
    while (ci != si || cj != sj) {
        auto [pi, pj] = parent[ci][cj];
        for (int d = 0; d < 4; d++) {
            if (pi + di[d] == ci && pj + dj[d] == cj) {
                shortest = dirs[d] + shortest;
                break;
            }
        }
        ci = pi;
        cj = pj;
    }
    
    // Repeat the shortest path multiple times to handle forgetting
    int repeat_count = min(200 / (int)shortest.length(), 8);
    if (repeat_count == 0) repeat_count = 1;
    
    for (int i = 0; i < repeat_count && path.length() + shortest.length() <= 200; i++) {
        path += shortest;
    }
    
    cout << path << endl;
    
    return 0;
}
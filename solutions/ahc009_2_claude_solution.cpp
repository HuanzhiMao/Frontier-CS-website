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
    
    // Build adjacency list for the graph
    vector<vector<vector<pair<int, int>>>> adj(20, vector<vector<pair<int, int>>>(20));
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            // Check right
            if (j < 19 && h[i][j] == '0') {
                adj[i][j].push_back({i, j + 1});
            }
            // Check left
            if (j > 0 && h[i][j - 1] == '0') {
                adj[i][j].push_back({i, j - 1});
            }
            // Check down
            if (i < 19 && v[i][j] == '0') {
                adj[i][j].push_back({i + 1, j});
            }
            // Check up
            if (i > 0 && v[i - 1][j] == '0') {
                adj[i][j].push_back({i - 1, j});
            }
        }
    }
    
    // BFS to find shortest path
    queue<pair<int, int>> q;
    vector<vector<int>> dist(20, vector<int>(20, -1));
    vector<vector<pair<int, int>>> parent(20, vector<pair<int, int>>(20, {-1, -1}));
    
    q.push({si, sj});
    dist[si][sj] = 0;
    
    while (!q.empty()) {
        auto [ci, cj] = q.front();
        q.pop();
        
        if (ci == ti && cj == tj) break;
        
        for (auto [ni, nj] : adj[ci][cj]) {
            if (dist[ni][nj] == -1) {
                dist[ni][nj] = dist[ci][cj] + 1;
                parent[ni][nj] = {ci, cj};
                q.push({ni, nj});
            }
        }
    }
    
    // Reconstruct path
    vector<char> path;
    int ci = ti, cj = tj;
    while (parent[ci][cj].first != -1) {
        int pi = parent[ci][cj].first;
        int pj = parent[ci][cj].second;
        
        if (pi == ci - 1) path.push_back('D');
        else if (pi == ci + 1) path.push_back('U');
        else if (pj == cj - 1) path.push_back('R');
        else path.push_back('L');
        
        ci = pi;
        cj = pj;
    }
    
    reverse(path.begin(), path.end());
    
    // Create robust path by repeating the basic path
    string result;
    int repetitions = min(200 / (int)path.size(), 8);
    if (repetitions == 0) repetitions = 1;
    
    for (int rep = 0; rep < repetitions && result.length() + path.size() <= 200; rep++) {
        for (char c : path) {
            result += c;
        }
    }
    
    cout << result << endl;
    
    return 0;
}
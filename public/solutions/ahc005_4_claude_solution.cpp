#include <bits/stdc++.h>
using namespace std;

int N, si, sj;
vector<string> grid;
vector<vector<bool>> visited;
vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
string dirChars = "UDLR";

bool isValid(int i, int j) {
    return i >= 0 && i < N && j >= 0 && j < N && grid[i][j] != '#';
}

vector<pair<int, int>> getVisibleCells(int i, int j) {
    vector<pair<int, int>> visible;
    
    // Horizontal line
    for (int jj = 0; jj < N; jj++) {
        if (grid[i][jj] != '#') {
            visible.push_back({i, jj});
        } else {
            break;
        }
    }
    for (int jj = j - 1; jj >= 0; jj--) {
        if (grid[i][jj] != '#') {
            visible.push_back({i, jj});
        } else {
            break;
        }
    }
    
    // Vertical line
    for (int ii = i + 1; ii < N; ii++) {
        if (grid[ii][j] != '#') {
            visible.push_back({ii, j});
        } else {
            break;
        }
    }
    for (int ii = i - 1; ii >= 0; ii--) {
        if (grid[ii][j] != '#') {
            visible.push_back({ii, j});
        } else {
            break;
        }
    }
    
    return visible;
}

int main() {
    cin >> N >> si >> sj;
    grid.resize(N);
    visited.resize(N, vector<bool>(N, false));
    
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    string result = "";
    int curI = si, curJ = sj;
    
    // Mark visible cells from starting position
    vector<pair<int, int>> visible = getVisibleCells(curI, curJ);
    for (auto& p : visible) {
        visited[p.first][p.second] = true;
    }
    
    // Simple greedy approach: visit cells that maximize new visibility
    while (true) {
        int bestDir = -1;
        int bestNewVisible = 0;
        
        for (int d = 0; d < 4; d++) {
            int ni = curI + directions[d].first;
            int nj = curJ + directions[d].second;
            
            if (!isValid(ni, nj)) continue;
            
            vector<pair<int, int>> newVisible = getVisibleCells(ni, nj);
            int newCount = 0;
            for (auto& p : newVisible) {
                if (!visited[p.first][p.second]) {
                    newCount++;
                }
            }
            
            if (newCount > bestNewVisible) {
                bestNewVisible = newCount;
                bestDir = d;
            }
        }
        
        if (bestDir == -1 || bestNewVisible == 0) break;
        
        curI += directions[bestDir].first;
        curJ += directions[bestDir].second;
        result += dirChars[bestDir];
        
        visible = getVisibleCells(curI, curJ);
        for (auto& p : visible) {
            visited[p.first][p.second] = true;
        }
    }
    
    // Return to start using BFS
    queue<tuple<int, int, string>> q;
    set<pair<int, int>> seen;
    q.push({curI, curJ, ""});
    seen.insert({curI, curJ});
    
    while (!q.empty()) {
        auto [i, j, path] = q.front();
        q.pop();
        
        if (i == si && j == sj) {
            result += path;
            break;
        }
        
        for (int d = 0; d < 4; d++) {
            int ni = i + directions[d].first;
            int nj = j + directions[d].second;
            
            if (isValid(ni, nj) && seen.find({ni, nj}) == seen.end()) {
                seen.insert({ni, nj});
                q.push({ni, nj, path + dirChars[d]});
            }
        }
    }
    
    cout << result << endl;
    return 0;
}
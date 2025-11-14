#include <bits/stdc++.h>
using namespace std;

int n, m, sr, sc, er, ec;
vector<string> grid;
int dx[] = {-1, 1, 0, 0}; // U, D, L, R
int dy[] = {0, 0, -1, 1};
char moves[] = {'U', 'D', 'L', 'R'};

bool isValid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m && grid[x][y] == '1';
}

pair<int, int> makeMove(int x, int y, int dir) {
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    if (isValid(nx, ny)) {
        return {nx, ny};
    }
    return {x, y};
}

int getOpposite(int dir) {
    if (dir == 0) return 1; // U -> D
    if (dir == 1) return 0; // D -> U
    if (dir == 2) return 3; // L -> R
    if (dir == 3) return 2; // R -> L
    return -1;
}

string solve() {
    int totalBlanks = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1') totalBlanks++;
        }
    }
    
    // BFS to find palindromic path
    queue<tuple<int, int, int, int, set<pair<int, int>>, string>> q;
    set<pair<int, int>> visited;
    visited.insert({sr, sc});
    q.push({sr, sc, sr, sc, visited, ""});
    
    map<tuple<int, int, int, int, set<pair<int, int>>>, bool> seen;
    
    while (!q.empty()) {
        auto [x1, y1, x2, y2, vis, path] = q.front();
        q.pop();
        
        if (path.length() > 1000) continue;
        
        if (x1 == er && y1 == ec && vis.size() == totalBlanks) {
            return path;
        }
        
        auto state = make_tuple(x1, y1, x2, y2, vis);
        if (seen.count(state)) continue;
        seen[state] = true;
        
        for (int i = 0; i < 4; i++) {
            auto [nx1, ny1] = makeMove(x1, y1, i);
            auto [nx2, ny2] = makeMove(x2, y2, getOpposite(i));
            
            set<pair<int, int>> newVis = vis;
            newVis.insert({nx1, ny1});
            newVis.insert({nx2, ny2});
            
            string newPath = path + moves[i];
            
            q.push({nx1, ny1, nx2, ny2, newVis, newPath});
        }
    }
    
    return "-1";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m;
    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }
    cin >> sr >> sc >> er >> ec;
    sr--; sc--; er--; ec--; // Convert to 0-indexed
    
    // Special case: if start equals end and only one blank cell
    int totalBlanks = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1') totalBlanks++;
        }
    }
    
    if (sr == er && sc == ec && totalBlanks == 1) {
        cout << "" << endl;
        return 0;
    }
    
    // Use iterative deepening
    for (int maxLen = 0; maxLen <= 1000; maxLen += 2) {
        queue<tuple<int, int, int, int, set<pair<int, int>>, string>> q;
        set<pair<int, int>> visited;
        visited.insert({sr, sc});
        q.push({sr, sc, sr, sc, visited, ""});
        
        while (!q.empty()) {
            auto [x1, y1, x2, y2, vis, path] = q.front();
            q.pop();
            
            if (path.length() == maxLen) {
                if (x1 == er && y1 == ec && vis.size() == totalBlanks) {
                    cout << path << endl;
                    return 0;
                }
                continue;
            }
            
            for (int i = 0; i < 4; i++) {
                auto [nx1, ny1] = makeMove(x1, y1, i);
                auto [nx2, ny2] = makeMove(x2, y2, getOpposite(i));
                
                set<pair<int, int>> newVis = vis;
                newVis.insert({nx1, ny1});
                newVis.insert({nx2, ny2});
                
                string newPath = path + moves[i];
                
                q.push({nx1, ny1, nx2, ny2, newVis, newPath});
            }
        }
    }
    
    cout << "-1" << endl;
    return 0;
}
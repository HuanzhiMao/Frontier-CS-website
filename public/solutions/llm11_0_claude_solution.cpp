#include <bits/stdc++.h>
using namespace std;

int n, m, sr, sc, er, ec;
vector<string> grid;
int dx[] = {-1, 1, 0, 0};
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m;
    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }
    cin >> sr >> sc >> er >> ec;
    sr--; sc--; er--; ec--;
    
    // Count total blank cells
    int totalBlank = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1') totalBlank++;
        }
    }
    
    // BFS to find palindrome sequence
    queue<tuple<int, int, int, int, set<pair<int, int>>, string>> q;
    set<tuple<int, int, int, int, set<pair<int, int>>>> visited;
    
    set<pair<int, int>> initial;
    initial.insert({sr, sc});
    q.push({sr, sc, sr, sc, initial, ""});
    
    while (!q.empty()) {
        auto [r1, c1, r2, c2, vis, path] = q.front();
        q.pop();
        
        if (r1 == er && c1 == ec && vis.size() == totalBlank) {
            cout << path << "\n";
            return 0;
        }
        
        if (path.length() >= 60) continue;
        
        auto state = make_tuple(r1, c1, r2, c2, vis);
        if (visited.count(state)) continue;
        visited.insert(state);
        
        // Try all 4 moves
        for (int dir = 0; dir < 4; dir++) {
            auto [nr1, nc1] = makeMove(r1, c1, dir);
            auto [nr2, nc2] = makeMove(r2, c2, 3 - dir); // opposite direction
            
            set<pair<int, int>> newVis = vis;
            newVis.insert({nr1, nc1});
            newVis.insert({nr2, nc2});
            
            string newPath = path + moves[dir];
            
            q.push({nr1, nc1, nr2, nc2, newVis, newPath});
        }
    }
    
    cout << "-1\n";
    return 0;
}
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

string result = "";
bool found = false;

void dfs(vector<string>& path, int x1, int y1, int x2, int y2, set<pair<int, int>>& visited, int totalBlanks, int depth) {
    if (found) return;
    if (depth > 1000000) return;
    
    if (path.size() % 2 == 0) {
        if (x1 == x2 && y1 == y2) {
            if (visited.size() == totalBlanks && x1 == er && y1 == ec) {
                result = "";
                for (char c : path) result += c;
                found = true;
                return;
            }
        }
    } else {
        if (visited.size() == totalBlanks) {
            bool canReach = false;
            for (int dir = 0; dir < 4; dir++) {
                auto [nx1, ny1] = makeMove(x1, y1, dir);
                auto [nx2, ny2] = makeMove(x2, y2, getOpposite(dir));
                if (nx1 == nx2 && ny1 == ny2 && nx1 == er && ny1 == ec) {
                    canReach = true;
                    path.push_back(moves[dir]);
                    result = "";
                    for (char c : path) result += c;
                    found = true;
                    return;
                }
            }
        }
    }
    
    for (int dir = 0; dir < 4; dir++) {
        auto [nx1, ny1] = makeMove(x1, y1, dir);
        auto [nx2, ny2] = makeMove(x2, y2, getOpposite(dir));
        
        set<pair<int, int>> newVisited = visited;
        newVisited.insert({nx1, ny1});
        newVisited.insert({nx2, ny2});
        
        if (newVisited.size() <= totalBlanks) {
            path.push_back(moves[dir]);
            dfs(path, nx1, ny1, nx2, ny2, newVisited, totalBlanks, depth + 1);
            path.pop_back();
        }
    }
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
    
    vector<string> path;
    set<pair<int, int>> visited;
    visited.insert({sr, sc});
    
    dfs(path, sr, sc, er, ec, visited, totalBlanks, 0);
    
    if (found) {
        cout << result << endl;
    } else {
        cout << "-1" << endl;
    }
    
    return 0;
}
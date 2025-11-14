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
    return 2; // R -> L
}

string solve() {
    int totalBlanks = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1') totalBlanks++;
        }
    }
    
    if (sr == er && sc == ec && totalBlanks == 1) {
        return "";
    }
    
    // Try different palindrome lengths
    for (int len = 1; len <= min(1000000, 2000); len++) {
        // Try all possible palindromes of this length
        function<bool(int, int, int, set<pair<int,int>>&, string&)> dfs = 
        [&](int pos, int x, int y, set<pair<int,int>>& visited, string& path) -> bool {
            if (pos == len) {
                return x == er && y == ec && visited.size() == totalBlanks;
            }
            
            int mid = len / 2;
            if (pos < mid || (len % 2 == 1 && pos == mid)) {
                // First half or middle of palindrome
                for (int dir = 0; dir < 4; dir++) {
                    path[pos] = moves[dir];
                    if (len % 2 == 0 || pos < mid) {
                        path[len - 1 - pos] = moves[getOpposite(dir)];
                    }
                    
                    auto [nx, ny] = makeMove(x, y, dir);
                    visited.insert({nx, ny});
                    
                    if (dfs(pos + 1, nx, ny, visited, path)) {
                        return true;
                    }
                    
                    visited.erase({nx, ny});
                }
            } else {
                // Second half of palindrome - move is already determined
                int moveDir = -1;
                for (int dir = 0; dir < 4; dir++) {
                    if (moves[dir] == path[pos]) {
                        moveDir = dir;
                        break;
                    }
                }
                
                auto [nx, ny] = makeMove(x, y, moveDir);
                visited.insert({nx, ny});
                
                bool result = dfs(pos + 1, nx, ny, visited, path);
                visited.erase({nx, ny});
                return result;
            }
            return false;
        };
        
        string path(len, ' ');
        set<pair<int, int>> visited;
        visited.insert({sr, sc});
        
        if (dfs(0, sr, sc, visited, path)) {
            return path;
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
    
    cout << solve() << "\n";
    
    return 0;
}
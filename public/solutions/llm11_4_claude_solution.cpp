#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<string> grid;
int sr, sc, er, ec;
int dx[] = {-1, 1, 0, 0}; // U, D, L, R
int dy[] = {0, 0, -1, 1};
string moves = "UDLR";
map<char, int> moveToDir = {{'U', 0}, {'D', 1}, {'L', 2}, {'R', 3}};

bool isValid(int x, int y) {
    return x >= 1 && x <= n && y >= 1 && y <= m && grid[x-1][y-1] == '1';
}

pair<int, int> makeMove(int x, int y, int dir) {
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    if (isValid(nx, ny)) {
        return {nx, ny};
    }
    return {x, y};
}

int countBlanks() {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1') count++;
        }
    }
    return count;
}

string solve() {
    int totalBlanks = countBlanks();
    
    // BFS with state (x, y, visited_mask, position_in_palindrome)
    map<tuple<int, int, set<pair<int, int>>, int>, string> dp;
    queue<tuple<int, int, set<pair<int, int>>, string>> q;
    
    set<pair<int, int>> initial;
    initial.insert({sr, sc});
    q.push({sr, sc, initial, ""});
    
    for (int len = 0; len <= min(1000000, totalBlanks * 100); len++) {
        queue<tuple<int, int, set<pair<int, int>>, string>> nextQ;
        
        while (!q.empty()) {
            auto [x, y, visited, path] = q.front();
            q.pop();
            
            if (x == er && y == ec && visited.size() == totalBlanks) {
                return path;
            }
            
            if (path.length() >= 1000) continue;
            
            for (int dir = 0; dir < 4; dir++) {
                string newPath = path + moves[dir];
                
                // Simulate the palindrome path
                int curX = sr, curY = sc;
                set<pair<int, int>> newVisited;
                newVisited.insert({curX, curY});
                
                bool valid = true;
                
                // Forward pass
                for (char c : newPath) {
                    auto [nx, ny] = makeMove(curX, curY, moveToDir[c]);
                    curX = nx;
                    curY = ny;
                    newVisited.insert({curX, curY});
                }
                
                // Backward pass
                for (int i = newPath.length() - 1; i >= 0; i--) {
                    char c = newPath[i];
                    auto [nx, ny] = makeMove(curX, curY, moveToDir[c]);
                    curX = nx;
                    curY = ny;
                    newVisited.insert({curX, curY});
                }
                
                if (valid) {
                    nextQ.push({curX, curY, newVisited, newPath});
                }
            }
        }
        
        q = nextQ;
        if (q.empty()) break;
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
    
    // Special case: if start == end and only one blank cell
    if (sr == er && sc == ec && countBlanks() == 1) {
        cout << "" << endl;
        return 0;
    }
    
    string result = solve();
    cout << result << endl;
    
    return 0;
}
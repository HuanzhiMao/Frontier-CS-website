#include <bits/stdc++.h>
using namespace std;

int N, si, sj;
vector<string> grid;
vector<vector<bool>> visited;

struct Point {
    int i, j;
};

int di[] = {-1, 1, 0, 0};
int dj[] = {0, 0, -1, 1};
char moves[] = {'U', 'D', 'L', 'R'};

bool isValid(int i, int j) {
    return i >= 0 && i < N && j >= 0 && j < N && grid[i][j] != '#';
}

void markVisible(int i, int j, vector<vector<bool>>& seen) {
    if (!isValid(i, j)) return;
    
    // Mark horizontally visible squares
    for (int jj = j; jj >= 0 && grid[i][jj] != '#'; jj--) {
        seen[i][jj] = true;
    }
    for (int jj = j; jj < N && grid[i][jj] != '#'; jj++) {
        seen[i][jj] = true;
    }
    
    // Mark vertically visible squares
    for (int ii = i; ii >= 0 && grid[ii][j] != '#'; ii--) {
        seen[ii][j] = true;
    }
    for (int ii = i; ii < N && grid[ii][j] != '#'; ii++) {
        seen[ii][j] = true;
    }
}

int countVisibleRoads(const vector<vector<bool>>& seen) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != '#' && seen[i][j]) {
                count++;
            }
        }
    }
    return count;
}

string solve() {
    vector<vector<bool>> seen(N, vector<bool>(N, false));
    vector<Point> roadSquares;
    
    // Collect all road squares
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != '#') {
                roadSquares.push_back({i, j});
            }
        }
    }
    
    string result = "";
    int curI = si, curJ = sj;
    
    // Simple greedy approach: visit squares that maximize new visibility
    while (true) {
        markVisible(curI, curJ, seen);
        
        int bestGain = 0;
        int bestDir = -1;
        
        for (int d = 0; d < 4; d++) {
            int ni = curI + di[d];
            int nj = curJ + dj[d];
            
            if (!isValid(ni, nj)) continue;
            
            vector<vector<bool>> tempSeen = seen;
            markVisible(ni, nj, tempSeen);
            
            int newVisible = countVisibleRoads(tempSeen) - countVisibleRoads(seen);
            if (newVisible > bestGain) {
                bestGain = newVisible;
                bestDir = d;
            }
        }
        
        if (bestDir == -1 || bestGain == 0) break;
        
        result += moves[bestDir];
        curI += di[bestDir];
        curJ += dj[bestDir];
    }
    
    // Return to start using BFS
    queue<Point> q;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    vector<vector<char>> parent(N, vector<char>(N, 0));
    
    q.push({curI, curJ});
    dist[curI][curJ] = 0;
    
    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        
        if (cur.i == si && cur.j == sj) break;
        
        for (int d = 0; d < 4; d++) {
            int ni = cur.i + di[d];
            int nj = cur.j + dj[d];
            
            if (isValid(ni, nj) && dist[ni][nj] == -1) {
                dist[ni][nj] = dist[cur.i][cur.j] + 1;
                parent[ni][nj] = moves[d];
                q.push({ni, nj});
            }
        }
    }
    
    // Reconstruct path back to start
    string backPath = "";
    int x = si, y = sj;
    
    while (x != curI || y != curJ) {
        for (int d = 0; d < 4; d++) {
            int px = x - di[d];
            int py = y - dj[d];
            
            if (isValid(px, py) && dist[px][py] == dist[x][y] - 1) {
                backPath = moves[d] + backPath;
                x = px;
                y = py;
                break;
            }
        }
    }
    
    result += backPath;
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> si >> sj;
    grid.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    cout << solve() << endl;
    
    return 0;
}
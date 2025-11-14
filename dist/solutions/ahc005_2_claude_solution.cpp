#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

int N, si, sj;
vector<string> grid;
vector<vector<int>> dist;
vector<vector<Point>> parent;
string dx = "UDLR";
int ddx[] = {-1, 1, 0, 0};
int ddy[] = {0, 0, -1, 1};

bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N && grid[x][y] != '#';
}

vector<Point> getVisible(int x, int y) {
    vector<Point> visible;
    
    // Same row - left and right
    for (int j = y; j >= 0 && grid[x][j] != '#'; j--) {
        visible.push_back(Point(x, j));
    }
    for (int j = y + 1; j < N && grid[x][j] != '#'; j++) {
        visible.push_back(Point(x, j));
    }
    
    // Same column - up and down
    for (int i = x - 1; i >= 0 && grid[i][y] != '#'; i--) {
        visible.push_back(Point(i, y));
    }
    for (int i = x + 1; i < N && grid[i][y] != '#'; i++) {
        visible.push_back(Point(i, y));
    }
    
    return visible;
}

string bfs(int sx, int sy, int tx, int ty) {
    if (sx == tx && sy == ty) return "";
    
    dist.assign(N, vector<int>(N, -1));
    parent.assign(N, vector<Point>(N, Point(-1, -1)));
    
    queue<Point> q;
    q.push(Point(sx, sy));
    dist[sx][sy] = 0;
    
    while (!q.empty()) {
        Point cur = q.front();
        q.pop();
        
        if (cur.x == tx && cur.y == ty) break;
        
        for (int d = 0; d < 4; d++) {
            int nx = cur.x + ddx[d];
            int ny = cur.y + ddy[d];
            
            if (isValid(nx, ny) && dist[nx][ny] == -1) {
                dist[nx][ny] = dist[cur.x][cur.y] + (grid[nx][ny] - '0');
                parent[nx][ny] = cur;
                q.push(Point(nx, ny));
            }
        }
    }
    
    if (dist[tx][ty] == -1) return "";
    
    string path = "";
    int cx = tx, cy = ty;
    
    while (parent[cx][cy].x != -1) {
        Point p = parent[cx][cy];
        for (int d = 0; d < 4; d++) {
            if (p.x + ddx[d] == cx && p.y + ddy[d] == cy) {
                path = dx[d] + path;
                break;
            }
        }
        cx = p.x;
        cy = p.y;
    }
    
    return path;
}

int main() {
    cin >> N >> si >> sj;
    grid.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    // Find all road squares
    vector<Point> roads;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != '#') {
                roads.push_back(Point(i, j));
            }
        }
    }
    
    set<pair<int, int>> seen;
    string result = "";
    int cx = si, cy = sj;
    
    // Add initially visible squares
    vector<Point> initial = getVisible(cx, cy);
    for (auto p : initial) {
        seen.insert({p.x, p.y});
    }
    
    while (seen.size() < roads.size()) {
        int bestScore = -1;
        Point bestTarget(-1, -1);
        
        // Find the road square that would reveal the most unseen squares
        for (auto road : roads) {
            if (seen.count({road.x, road.y})) continue;
            
            vector<Point> visible = getVisible(road.x, road.y);
            int newSeen = 0;
            for (auto p : visible) {
                if (!seen.count({p.x, p.y})) {
                    newSeen++;
                }
            }
            
            if (newSeen > bestScore) {
                bestScore = newSeen;
                bestTarget = road;
            }
        }
        
        if (bestTarget.x == -1) break;
        
        // Move to the best target
        string path = bfs(cx, cy, bestTarget.x, bestTarget.y);
        result += path;
        cx = bestTarget.x;
        cy = bestTarget.y;
        
        // Add newly visible squares
        vector<Point> visible = getVisible(cx, cy);
        for (auto p : visible) {
            seen.insert({p.x, p.y});
        }
    }
    
    // Return to start
    string returnPath = bfs(cx, cy, si, sj);
    result += returnPath;
    
    cout << result << endl;
    
    return 0;
}
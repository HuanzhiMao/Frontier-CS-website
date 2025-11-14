#include <bits/stdc++.h>
using namespace std;

const int N = 30;
vector<vector<int>> pyramid(N);
vector<pair<pair<int,int>, pair<int,int>>> operations;

bool isAdjacent(int x1, int y1, int x2, int y2) {
    if (x1 == x2-1 && y1 == y2-1) return true;
    if (x1 == x2-1 && y1 == y2) return true;
    if (x1 == x2 && y1 == y2-1) return true;
    if (x1 == x2 && y1 == y2+1) return true;
    if (x1 == x2+1 && y1 == y2) return true;
    if (x1 == x2+1 && y1 == y2+1) return true;
    return false;
}

void swapBalls(int x1, int y1, int x2, int y2) {
    swap(pyramid[x1][y1], pyramid[x2][y2]);
    operations.push_back({{x1, y1}, {x2, y2}});
}

vector<pair<int,int>> getNeighbors(int x, int y) {
    vector<pair<int,int>> neighbors;
    vector<pair<int,int>> dirs = {{-1,-1}, {-1,0}, {0,-1}, {0,1}, {1,0}, {1,1}};
    
    for (auto [dx, dy] : dirs) {
        int nx = x + dx, ny = y + dy;
        if (nx >= 0 && nx < N && ny >= 0 && ny <= nx) {
            neighbors.push_back({nx, ny});
        }
    }
    return neighbors;
}

bool bubbleUp(int x, int y, int targetVal) {
    if (pyramid[x][y] == targetVal) return true;
    
    queue<pair<int,int>> q;
    map<pair<int,int>, pair<int,int>> parent;
    set<pair<int,int>> visited;
    
    q.push({x, y});
    visited.insert({x, y});
    
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        
        if (pyramid[cx][cy] == targetVal) {
            vector<pair<int,int>> path;
            pair<int,int> curr = {cx, cy};
            while (curr != make_pair(x, y)) {
                path.push_back(curr);
                curr = parent[curr];
            }
            
            for (int i = path.size() - 1; i >= 0; i--) {
                swapBalls(curr.first, curr.second, path[i].first, path[i].second);
                curr = path[i];
            }
            return true;
        }
        
        for (auto [nx, ny] : getNeighbors(cx, cy)) {
            if (visited.find({nx, ny}) == visited.end()) {
                visited.insert({nx, ny});
                parent[{nx, ny}] = {cx, cy};
                q.push({nx, ny});
            }
        }
    }
    return false;
}

int main() {
    pyramid.resize(N);
    for (int i = 0; i < N; i++) {
        pyramid[i].resize(i + 1);
        for (int j = 0; j <= i; j++) {
            cin >> pyramid[i][j];
        }
    }
    
    for (int x = 0; x < N-1; x++) {
        for (int y = 0; y <= x; y++) {
            int minBelow = min(pyramid[x+1][y], pyramid[x+1][y+1]);
            if (pyramid[x][y] > minBelow) {
                bubbleUp(x, y, minBelow);
            }
        }
    }
    
    for (int iter = 0; iter < 3 && operations.size() < 9000; iter++) {
        for (int x = N-2; x >= 0; x--) {
            for (int y = 0; y <= x; y++) {
                int current = pyramid[x][y];
                int below1 = pyramid[x+1][y];
                int below2 = pyramid[x+1][y+1];
                
                if (current > below1 || current > below2) {
                    vector<pair<int,int>> candidates;
                    for (auto [nx, ny] : getNeighbors(x, y)) {
                        if (pyramid[nx][ny] < current && 
                            pyramid[nx][ny] < below1 && 
                            pyramid[nx][ny] < below2) {
                            candidates.push_back({nx, ny});
                        }
                    }
                    
                    if (!candidates.empty()) {
                        sort(candidates.begin(), candidates.end(), [&](auto a, auto b) {
                            return pyramid[a.first][a.second] < pyramid[b.first][b.second];
                        });
                        
                        auto [bx, by] = candidates[0];
                        swapBalls(x, y, bx, by);
                    }
                }
            }
        }
    }
    
    cout << operations.size() << "\n";
    for (auto op : operations) {
        cout << op.first.first << " " << op.first.second << " " 
             << op.second.first << " " << op.second.second << "\n";
    }
    
    return 0;
}
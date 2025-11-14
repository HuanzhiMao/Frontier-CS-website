#include <bits/stdc++.h>
using namespace std;

int N, si, sj;
vector<string> grid;
vector<vector<bool>> visited_squares;
vector<pair<int,int>> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}};
string dir_chars = "UDLR";

bool is_valid(int i, int j) {
    return i >= 0 && i < N && j >= 0 && j < N && grid[i][j] != '#';
}

void mark_visible(int i, int j) {
    // Mark horizontal visibility
    for (int jj = j; jj >= 0 && grid[i][jj] != '#'; jj--) {
        visited_squares[i][jj] = true;
    }
    for (int jj = j; jj < N && grid[i][jj] != '#'; jj++) {
        visited_squares[i][jj] = true;
    }
    
    // Mark vertical visibility
    for (int ii = i; ii >= 0 && grid[ii][j] != '#'; ii--) {
        visited_squares[ii][j] = true;
    }
    for (int ii = i; ii < N && grid[ii][j] != '#'; ii++) {
        visited_squares[ii][j] = true;
    }
}

int count_visible() {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (visited_squares[i][j]) count++;
        }
    }
    return count;
}

string solve() {
    visited_squares.assign(N, vector<bool>(N, false));
    
    string path = "";
    int ci = si, cj = sj;
    
    mark_visible(ci, cj);
    
    // Simple greedy approach: visit all road squares
    vector<pair<int,int>> road_squares;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] != '#') {
                road_squares.push_back({i, j});
            }
        }
    }
    
    // Sort by Manhattan distance from start
    sort(road_squares.begin(), road_squares.end(), [&](const pair<int,int>& a, const pair<int,int>& b) {
        return abs(a.first - si) + abs(a.second - sj) < abs(b.first - si) + abs(b.second - sj);
    });
    
    set<pair<int,int>> visited_positions;
    
    for (auto& target : road_squares) {
        if (visited_positions.count(target)) continue;
        
        // BFS to find shortest path to target
        queue<pair<pair<int,int>, string>> q;
        set<pair<int,int>> bfs_visited;
        q.push({{ci, cj}, ""});
        bfs_visited.insert({ci, cj});
        
        while (!q.empty()) {
            auto [pos, cur_path] = q.front();
            q.pop();
            
            if (pos == target) {
                path += cur_path;
                ci = pos.first;
                cj = pos.second;
                mark_visible(ci, cj);
                visited_positions.insert({ci, cj});
                break;
            }
            
            for (int d = 0; d < 4; d++) {
                int ni = pos.first + directions[d].first;
                int nj = pos.second + directions[d].second;
                
                if (is_valid(ni, nj) && !bfs_visited.count({ni, nj})) {
                    bfs_visited.insert({ni, nj});
                    q.push({{ni, nj}, cur_path + dir_chars[d]});
                }
            }
        }
    }
    
    // Return to start
    queue<pair<pair<int,int>, string>> q;
    set<pair<int,int>> bfs_visited;
    q.push({{ci, cj}, ""});
    bfs_visited.insert({ci, cj});
    
    while (!q.empty()) {
        auto [pos, cur_path] = q.front();
        q.pop();
        
        if (pos.first == si && pos.second == sj) {
            path += cur_path;
            break;
        }
        
        for (int d = 0; d < 4; d++) {
            int ni = pos.first + directions[d].first;
            int nj = pos.second + directions[d].second;
            
            if (is_valid(ni, nj) && !bfs_visited.count({ni, nj})) {
                bfs_visited.insert({ni, nj});
                q.push({{ni, nj}, cur_path + dir_chars[d]});
            }
        }
    }
    
    return path;
}

int main() {
    cin >> N >> si >> sj;
    grid.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    cout << solve() << endl;
    
    return 0;
}
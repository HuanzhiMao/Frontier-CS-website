#include <bits/stdc++.h>
using namespace std;

int N, si, sj;
vector<string> grid;
vector<vector<bool>> visited;
vector<vector<set<pair<int,int>>>> visible_from;

void compute_visible() {
    visible_from.assign(N, vector<set<pair<int,int>>>(N));
    
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(grid[i][j] == '#') continue;
            
            // Horizontal visibility
            for(int k = 0; k < N; k++) {
                if(grid[i][k] == '#') continue;
                bool can_see = true;
                int start = min(j, k), end = max(j, k);
                for(int l = start; l <= end; l++) {
                    if(grid[i][l] == '#') {
                        can_see = false;
                        break;
                    }
                }
                if(can_see) {
                    visible_from[i][j].insert({i, k});
                }
            }
            
            // Vertical visibility
            for(int k = 0; k < N; k++) {
                if(grid[k][j] == '#') continue;
                bool can_see = true;
                int start = min(i, k), end = max(i, k);
                for(int l = start; l <= end; l++) {
                    if(grid[l][j] == '#') {
                        can_see = false;
                        break;
                    }
                }
                if(can_see) {
                    visible_from[i][j].insert({k, j});
                }
            }
        }
    }
}

vector<pair<int,int>> get_road_squares() {
    vector<pair<int,int>> roads;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(grid[i][j] != '#') {
                roads.push_back({i, j});
            }
        }
    }
    return roads;
}

string solve() {
    compute_visible();
    
    vector<pair<int,int>> roads = get_road_squares();
    set<pair<int,int>> all_roads(roads.begin(), roads.end());
    set<pair<int,int>> seen;
    
    string path = "";
    int ci = si, cj = sj;
    
    // Add initially visible squares
    for(auto& p : visible_from[ci][cj]) {
        seen.insert(p);
    }
    
    while(seen.size() < roads.size()) {
        int best_di = 0, best_dj = 0;
        int best_new = -1;
        
        int di[] = {-1, 1, 0, 0};
        int dj[] = {0, 0, -1, 1};
        string dirs = "UDLR";
        
        for(int d = 0; d < 4; d++) {
            int ni = ci + di[d];
            int nj = cj + dj[d];
            
            if(ni < 0 || ni >= N || nj < 0 || nj >= N || grid[ni][nj] == '#') continue;
            
            int new_count = 0;
            for(auto& p : visible_from[ni][nj]) {
                if(seen.find(p) == seen.end()) {
                    new_count++;
                }
            }
            
            if(new_count > best_new) {
                best_new = new_count;
                best_di = di[d];
                best_dj = dj[d];
            }
        }
        
        if(best_new == -1) break;
        
        ci += best_di;
        cj += best_dj;
        
        if(best_di == -1) path += "U";
        else if(best_di == 1) path += "D";
        else if(best_dj == -1) path += "L";
        else path += "R";
        
        for(auto& p : visible_from[ci][cj]) {
            seen.insert(p);
        }
    }
    
    // Return to start using BFS
    queue<pair<pair<int,int>, string>> q;
    set<pair<int,int>> vis;
    q.push({{ci, cj}, ""});
    vis.insert({ci, cj});
    
    while(!q.empty()) {
        auto curr = q.front();
        q.pop();
        
        int x = curr.first.first;
        int y = curr.first.second;
        string p = curr.second;
        
        if(x == si && y == sj) {
            path += p;
            break;
        }
        
        int di[] = {-1, 1, 0, 0};
        int dj[] = {0, 0, -1, 1};
        string dirs = "UDLR";
        
        for(int d = 0; d < 4; d++) {
            int nx = x + di[d];
            int ny = y + dj[d];
            
            if(nx < 0 || nx >= N || ny < 0 || ny >= N || grid[nx][ny] == '#') continue;
            if(vis.find({nx, ny}) != vis.end()) continue;
            
            vis.insert({nx, ny});
            q.push({{nx, ny}, p + dirs[d]});
        }
    }
    
    return path;
}

int main() {
    cin >> N >> si >> sj;
    grid.resize(N);
    
    for(int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    cout << solve() << endl;
    
    return 0;
}
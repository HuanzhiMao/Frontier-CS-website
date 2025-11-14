#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<string> grid;
    vector<tuple<int, int, int>> ops;
};

int n, m, k;
vector<string> target;
vector<pair<int, int>> presetSizes;
vector<vector<string>> presets;

bool isTarget(const vector<string>& grid) {
    return grid == target;
}

void applyOp(vector<string>& grid, int op, int x, int y) {
    if (op == -4) {
        swap(grid[x-1][y-1], grid[x][y-1]);
    } else if (op == -3) {
        swap(grid[x-1][y-1], grid[x-2][y-1]);
    } else if (op == -2) {
        swap(grid[x-1][y-1], grid[x-1][y-2]);
    } else if (op == -1) {
        swap(grid[x-1][y-1], grid[x-1][y]);
    } else if (op == 0) {
        char temp = grid[x-1][y-1];
        grid[x-1][y-1] = grid[x][y-1];
        grid[x][y-1] = grid[x][y];
        grid[x][y] = grid[x-1][y];
        grid[x-1][y] = temp;
    } else {
        int np = presetSizes[op-1].first;
        int mp = presetSizes[op-1].second;
        for (int i = 0; i < np; i++) {
            for (int j = 0; j < mp; j++) {
                grid[x-1+i][y-1+j] = presets[op-1][i][j];
            }
        }
    }
}

bool canApplyOp(int op, int x, int y) {
    if (op == -4) return x >= 1 && x < n && y >= 1 && y <= m;
    if (op == -3) return x > 1 && x <= n && y >= 1 && y <= m;
    if (op == -2) return x >= 1 && x <= n && y > 1 && y <= m;
    if (op == -1) return x >= 1 && x <= n && y >= 1 && y < m;
    if (op == 0) return x >= 1 && x < n && y >= 1 && y < m;
    if (op >= 1 && op <= k) {
        int np = presetSizes[op-1].first;
        int mp = presetSizes[op-1].second;
        return x >= 1 && x <= n-np+1 && y >= 1 && y <= m-mp+1;
    }
    return false;
}

vector<tuple<int, int, int>> solve(vector<string> initial) {
    queue<State> q;
    set<vector<string>> visited;
    
    q.push({initial, {}});
    visited.insert(initial);
    
    while (!q.empty()) {
        State curr = q.front();
        q.pop();
        
        if (isTarget(curr.grid)) {
            return curr.ops;
        }
        
        if (curr.ops.size() >= 400000) continue;
        
        int presetCount = 0;
        for (auto& op : curr.ops) {
            if (get<0>(op) >= 1) presetCount++;
        }
        
        for (int op = -4; op <= k; op++) {
            if (op == 0) continue;
            if (op >= 1 && presetCount >= 400) continue;
            
            for (int x = 1; x <= n; x++) {
                for (int y = 1; y <= m; y++) {
                    if (!canApplyOp(op, x, y)) continue;
                    
                    vector<string> newGrid = curr.grid;
                    applyOp(newGrid, op, x, y);
                    
                    if (visited.find(newGrid) == visited.end()) {
                        visited.insert(newGrid);
                        vector<tuple<int, int, int>> newOps = curr.ops;
                        newOps.push_back({op, x, y});
                        q.push({newGrid, newOps});
                    }
                }
            }
        }
        
        // Rotation operation
        for (int x = 1; x < n; x++) {
            for (int y = 1; y < m; y++) {
                vector<string> newGrid = curr.grid;
                applyOp(newGrid, 0, x, y);
                
                if (visited.find(newGrid) == visited.end()) {
                    visited.insert(newGrid);
                    vector<tuple<int, int, int>> newOps = curr.ops;
                    newOps.push_back({0, x, y});
                    q.push({newGrid, newOps});
                }
            }
        }
    }
    
    return {};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m >> k;
    
    vector<string> initial(n);
    for (int i = 0; i < n; i++) {
        cin >> initial[i];
    }
    
    target.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> target[i];
    }
    
    presetSizes.resize(k);
    presets.resize(k);
    
    for (int i = 0; i < k; i++) {
        int np, mp;
        cin >> np >> mp;
        presetSizes[i] = {np, mp};
        presets[i].resize(np);
        for (int j = 0; j < np; j++) {
            cin >> presets[i][j];
        }
    }
    
    vector<tuple<int, int, int>> result = solve(initial);
    
    if (result.empty()) {
        cout << -1 << endl;
    } else {
        cout << result.size() << endl;
        for (auto& op : result) {
            cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << endl;
        }
    }
    
    return 0;
}
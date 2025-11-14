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
    } else if (op >= 1 && op <= k) {
        int np = presetSizes[op-1].first;
        int mp = presetSizes[op-1].second;
        for (int i = 0; i < np; i++) {
            for (int j = 0; j < mp; j++) {
                grid[x-1+i][y-1+j] = presets[op-1][i][j];
            }
        }
    }
}

bool solve(vector<string> grid, vector<tuple<int, int, int>> ops, int presetCount) {
    if (ops.size() > 400000) return false;
    if (presetCount > 400) return false;
    
    if (isTarget(grid)) {
        cout << ops.size() << "\n";
        for (auto [op, x, y] : ops) {
            cout << op << " " << x << " " << y << "\n";
        }
        return true;
    }
    
    queue<State> q;
    set<vector<string>> visited;
    
    q.push({grid, ops});
    visited.insert(grid);
    
    while (!q.empty()) {
        State curr = q.front();
        q.pop();
        
        if (curr.ops.size() > 400000) continue;
        
        int currPresetCount = 0;
        for (auto [op, x, y] : curr.ops) {
            if (op >= 1) currPresetCount++;
        }
        if (currPresetCount > 400) continue;
        
        // Try all operations
        vector<tuple<int, int, int>> allOps;
        
        // Switch operations
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (i < n) allOps.push_back({-4, i, j});
                if (i > 1) allOps.push_back({-3, i, j});
                if (j > 1) allOps.push_back({-2, i, j});
                if (j < m) allOps.push_back({-1, i, j});
            }
        }
        
        // Rotate operations
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                allOps.push_back({0, i, j});
            }
        }
        
        // Preset operations
        for (int op = 1; op <= k; op++) {
            int np = presetSizes[op-1].first;
            int mp = presetSizes[op-1].second;
            for (int i = 1; i <= n - np + 1; i++) {
                for (int j = 1; j <= m - mp + 1; j++) {
                    allOps.push_back({op, i, j});
                }
            }
        }
        
        for (auto [op, x, y] : allOps) {
            vector<string> newGrid = curr.grid;
            applyOp(newGrid, op, x, y);
            
            if (isTarget(newGrid)) {
                vector<tuple<int, int, int>> newOps = curr.ops;
                newOps.push_back({op, x, y});
                cout << newOps.size() << "\n";
                for (auto [op2, x2, y2] : newOps) {
                    cout << op2 << " " << x2 << " " << y2 << "\n";
                }
                return true;
            }
            
            if (visited.find(newGrid) == visited.end() && curr.ops.size() < 100) {
                vector<tuple<int, int, int>> newOps = curr.ops;
                newOps.push_back({op, x, y});
                
                int newPresetCount = currPresetCount;
                if (op >= 1) newPresetCount++;
                
                if (newPresetCount <= 400) {
                    visited.insert(newGrid);
                    q.push({newGrid, newOps});
                }
            }
        }
    }
    
    return false;
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
    
    presets.resize(k);
    presetSizes.resize(k);
    
    for (int i = 0; i < k; i++) {
        int np, mp;
        cin >> np >> mp;
        presetSizes[i] = {np, mp};
        presets[i].resize(np);
        for (int j = 0; j < np; j++) {
            cin >> presets[i][j];
        }
    }
    
    if (!solve(initial, {}, 0)) {
        cout << "-1\n";
    }
    
    return 0;
}
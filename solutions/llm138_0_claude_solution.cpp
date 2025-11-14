#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<string> grid;
    vector<tuple<int, int, int>> moves;
    
    State(vector<string> g) : grid(g) {}
    
    bool operator==(const State& other) const {
        return grid == other.grid;
    }
};

struct StateHash {
    size_t operator()(const State& s) const {
        size_t hash = 0;
        for (const string& row : s.grid) {
            for (char c : row) {
                hash = hash * 63 + (c - '0' + 48);
            }
        }
        return hash;
    }
};

int n, m, k;
vector<string> initial, target;
vector<pair<int, int>> presetSizes;
vector<vector<string>> presets;

void applyMove(vector<string>& grid, int op, int x, int y) {
    x--; y--; // Convert to 0-indexed
    
    if (op == -4) {
        swap(grid[x][y], grid[x+1][y]);
    } else if (op == -3) {
        swap(grid[x][y], grid[x-1][y]);
    } else if (op == -2) {
        swap(grid[x][y], grid[x][y-1]);
    } else if (op == -1) {
        swap(grid[x][y], grid[x][y+1]);
    } else if (op == 0) {
        char temp = grid[x][y];
        grid[x][y] = grid[x+1][y];
        grid[x+1][y] = grid[x+1][y+1];
        grid[x+1][y+1] = grid[x][y+1];
        grid[x][y+1] = temp;
    } else if (op >= 1) {
        int presetIdx = op - 1;
        for (int i = 0; i < presetSizes[presetIdx].first; i++) {
            for (int j = 0; j < presetSizes[presetIdx].second; j++) {
                grid[x+i][y+j] = presets[presetIdx][i][j];
            }
        }
    }
}

vector<tuple<int, int, int>> getAllMoves(const vector<string>& grid) {
    vector<tuple<int, int, int>> moves;
    
    // Switch operations
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i < n-1) moves.push_back({-4, i+1, j+1});
            if (i > 0) moves.push_back({-3, i+1, j+1});
            if (j > 0) moves.push_back({-2, i+1, j+1});
            if (j < m-1) moves.push_back({-1, i+1, j+1});
        }
    }
    
    // Rotate operations
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < m-1; j++) {
            moves.push_back({0, i+1, j+1});
        }
    }
    
    // Preset operations
    for (int p = 0; p < k; p++) {
        for (int i = 0; i <= n - presetSizes[p].first; i++) {
            for (int j = 0; j <= m - presetSizes[p].second; j++) {
                moves.push_back({p+1, i+1, j+1});
            }
        }
    }
    
    return moves;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m >> k;
    
    initial.resize(n);
    target.resize(n);
    
    for (int i = 0; i < n; i++) {
        cin >> initial[i];
    }
    
    for (int i = 0; i < n; i++) {
        cin >> target[i];
    }
    
    presetSizes.resize(k);
    presets.resize(k);
    
    for (int i = 0; i < k; i++) {
        cin >> presetSizes[i].first >> presetSizes[i].second;
        presets[i].resize(presetSizes[i].first);
        for (int j = 0; j < presetSizes[i].first; j++) {
            cin >> presets[i][j];
        }
    }
    
    if (initial == target) {
        cout << "0\n";
        return 0;
    }
    
    queue<State> q;
    unordered_set<vector<string>, hash<vector<string>>> visited;
    
    q.push(State(initial));
    visited.insert(initial);
    
    while (!q.empty()) {
        State curr = q.front();
        q.pop();
        
        if (curr.moves.size() > 400000) continue;
        
        vector<tuple<int, int, int>> possibleMoves = getAllMoves(curr.grid);
        
        for (auto move : possibleMoves) {
            int op = get<0>(move);
            int x = get<1>(move);
            int y = get<2>(move);
            
            // Check preset limit
            int presetCount = 0;
            for (auto m : curr.moves) {
                if (get<0>(m) >= 1) presetCount++;
            }
            if (op >= 1 && presetCount >= 400) continue;
            
            vector<string> newGrid = curr.grid;
            applyMove(newGrid, op, x, y);
            
            if (newGrid == target) {
                cout << curr.moves.size() + 1 << "\n";
                for (auto m : curr.moves) {
                    cout << get<0>(m) << " " << get<1>(m) << " " << get<2>(m) << "\n";
                }
                cout << op << " " << x << " " << y << "\n";
                return 0;
            }
            
            if (visited.find(newGrid) == visited.end()) {
                visited.insert(newGrid);
                State newState(newGrid);
                newState.moves = curr.moves;
                newState.moves.push_back({op, x, y});
                q.push(newState);
            }
        }
    }
    
    cout << "-1\n";
    return 0;
}
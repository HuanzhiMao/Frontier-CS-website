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
        for (const auto& row : s.grid) {
            for (char c : row) {
                hash = hash * 63 + c;
            }
        }
        return hash;
    }
};

int n, m, k;
vector<string> target;
vector<pair<int, int>> presetSizes;
vector<vector<string>> presets;

bool isValid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

vector<State> getNextStates(const State& state) {
    vector<State> next;
    
    // Switch operations
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Down
            if (i + 1 < n) {
                State newState = state;
                swap(newState.grid[i][j], newState.grid[i+1][j]);
                newState.moves = state.moves;
                newState.moves.push_back({-4, i+1, j+1});
                next.push_back(newState);
            }
            // Up
            if (i > 0) {
                State newState = state;
                swap(newState.grid[i][j], newState.grid[i-1][j]);
                newState.moves = state.moves;
                newState.moves.push_back({-3, i+1, j+1});
                next.push_back(newState);
            }
            // Left
            if (j > 0) {
                State newState = state;
                swap(newState.grid[i][j], newState.grid[i][j-1]);
                newState.moves = state.moves;
                newState.moves.push_back({-2, i+1, j+1});
                next.push_back(newState);
            }
            // Right
            if (j + 1 < m) {
                State newState = state;
                swap(newState.grid[i][j], newState.grid[i][j+1]);
                newState.moves = state.moves;
                newState.moves.push_back({-1, i+1, j+1});
                next.push_back(newState);
            }
        }
    }
    
    // Rotate operations
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < m-1; j++) {
            State newState = state;
            char temp = newState.grid[i][j];
            newState.grid[i][j] = newState.grid[i+1][j];
            newState.grid[i+1][j] = newState.grid[i+1][j+1];
            newState.grid[i+1][j+1] = newState.grid[i][j+1];
            newState.grid[i][j+1] = temp;
            newState.moves = state.moves;
            newState.moves.push_back({0, i+1, j+1});
            next.push_back(newState);
        }
    }
    
    // Preset operations
    for (int preset = 0; preset < k; preset++) {
        int np = presetSizes[preset].first;
        int mp = presetSizes[preset].second;
        
        for (int i = 0; i <= n - np; i++) {
            for (int j = 0; j <= m - mp; j++) {
                State newState = state;
                for (int di = 0; di < np; di++) {
                    for (int dj = 0; dj < mp; dj++) {
                        newState.grid[i + di][j + dj] = presets[preset][di][dj];
                    }
                }
                newState.moves = state.moves;
                newState.moves.push_back({preset + 1, i+1, j+1});
                next.push_back(newState);
            }
        }
    }
    
    return next;
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
        cin >> presetSizes[i].first >> presetSizes[i].second;
        presets[i].resize(presetSizes[i].first);
        for (int j = 0; j < presetSizes[i].first; j++) {
            cin >> presets[i][j];
        }
    }
    
    State initialState(initial);
    State targetState(target);
    
    if (initialState == targetState) {
        cout << "0\n";
        return 0;
    }
    
    queue<State> q;
    unordered_set<vector<string>> visited;
    
    q.push(initialState);
    visited.insert(initial);
    
    while (!q.empty()) {
        State current = q.front();
        q.pop();
        
        if (current.moves.size() > 400000) continue;
        
        vector<State> nextStates = getNextStates(current);
        
        for (const State& next : nextStates) {
            if (next.grid == target) {
                cout << next.moves.size() << "\n";
                for (const auto& move : next.moves) {
                    cout << get<0>(move) << " " << get<1>(move) << " " << get<2>(move) << "\n";
                }
                return 0;
            }
            
            if (visited.find(next.grid) == visited.end() && next.moves.size() < 400000) {
                visited.insert(next.grid);
                q.push(next);
            }
        }
    }
    
    cout << "-1\n";
    return 0;
}
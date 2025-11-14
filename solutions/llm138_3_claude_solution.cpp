#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<string> grid;
    vector<tuple<int, int, int>> operations;
    
    State(vector<string> g) : grid(g) {}
    
    bool operator==(const State& other) const {
        return grid == other.grid;
    }
};

struct StateHash {
    size_t operator()(const State& s) const {
        size_t h = 0;
        for (const string& row : s.grid) {
            for (char c : row) {
                h = h * 63 + (c - '0' + 10);
            }
        }
        return h;
    }
};

int n, m, k;
vector<string> initial, target;
vector<pair<int, int>> preset_sizes;
vector<vector<string>> presets;

vector<State> getNextStates(const State& curr) {
    vector<State> next;
    
    // Switch operations
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Down
            if (i + 1 < n) {
                State ns = curr;
                swap(ns.grid[i][j], ns.grid[i+1][j]);
                ns.operations = curr.operations;
                ns.operations.push_back({-4, i+1, j+1});
                next.push_back(ns);
            }
            // Up
            if (i - 1 >= 0) {
                State ns = curr;
                swap(ns.grid[i][j], ns.grid[i-1][j]);
                ns.operations = curr.operations;
                ns.operations.push_back({-3, i+1, j+1});
                next.push_back(ns);
            }
            // Left
            if (j - 1 >= 0) {
                State ns = curr;
                swap(ns.grid[i][j], ns.grid[i][j-1]);
                ns.operations = curr.operations;
                ns.operations.push_back({-2, i+1, j+1});
                next.push_back(ns);
            }
            // Right
            if (j + 1 < m) {
                State ns = curr;
                swap(ns.grid[i][j], ns.grid[i][j+1]);
                ns.operations = curr.operations;
                ns.operations.push_back({-1, i+1, j+1});
                next.push_back(ns);
            }
        }
    }
    
    // Rotate operations
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < m-1; j++) {
            State ns = curr;
            char temp = ns.grid[i][j];
            ns.grid[i][j] = ns.grid[i+1][j];
            ns.grid[i+1][j] = ns.grid[i+1][j+1];
            ns.grid[i+1][j+1] = ns.grid[i][j+1];
            ns.grid[i][j+1] = temp;
            ns.operations = curr.operations;
            ns.operations.push_back({0, i+1, j+1});
            next.push_back(ns);
        }
    }
    
    // Preset operations
    int preset_count = 0;
    for (const auto& op : curr.operations) {
        if (get<0>(op) >= 1) preset_count++;
    }
    
    if (preset_count < 400) {
        for (int p = 0; p < k; p++) {
            int np = preset_sizes[p].first;
            int mp = preset_sizes[p].second;
            
            for (int i = 0; i <= n - np; i++) {
                for (int j = 0; j <= m - mp; j++) {
                    State ns = curr;
                    for (int di = 0; di < np; di++) {
                        for (int dj = 0; dj < mp; dj++) {
                            ns.grid[i+di][j+dj] = presets[p][di][dj];
                        }
                    }
                    ns.operations = curr.operations;
                    ns.operations.push_back({p+1, i+1, j+1});
                    next.push_back(ns);
                }
            }
        }
    }
    
    return next;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> n >> m >> k;
    
    initial.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> initial[i];
    }
    
    target.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> target[i];
    }
    
    preset_sizes.resize(k);
    presets.resize(k);
    
    for (int i = 0; i < k; i++) {
        cin >> preset_sizes[i].first >> preset_sizes[i].second;
        presets[i].resize(preset_sizes[i].first);
        for (int j = 0; j < preset_sizes[i].first; j++) {
            cin >> presets[i][j];
        }
    }
    
    State start(initial);
    State goal(target);
    
    if (start == goal) {
        cout << "0\n";
        return 0;
    }
    
    queue<State> q;
    unordered_set<string> visited;
    
    q.push(start);
    string start_str = "";
    for (const string& row : start.grid) start_str += row;
    visited.insert(start_str);
    
    while (!q.empty()) {
        State curr = q.front();
        q.pop();
        
        if (curr.operations.size() >= 400000) continue;
        
        vector<State> next_states = getNextStates(curr);
        
        for (const State& ns : next_states) {
            if (ns == goal) {
                cout << ns.operations.size() << "\n";
                for (const auto& op : ns.operations) {
                    cout << get<0>(op) << " " << get<1>(op) << " " << get<2>(op) << "\n";
                }
                return 0;
            }
            
            string ns_str = "";
            for (const string& row : ns.grid) ns_str += row;
            
            if (visited.find(ns_str) == visited.end()) {
                visited.insert(ns_str);
                q.push(ns);
            }
        }
    }
    
    cout << "-1\n";
    return 0;
}
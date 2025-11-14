#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<vector<int>> board;
    vector<pair<int, int>> vehicles; // position of each vehicle
    vector<bool> isHorizontal;
    vector<int> length;
    
    bool operator<(const State& other) const {
        return board < other.board;
    }
};

int dx[] = {-1, 1, 0, 0}; // U, D, L, R
int dy[] = {0, 0, -1, 1};
char dir[] = {'U', 'D', 'L', 'R'};

bool isValid(int x, int y) {
    return x >= 0 && x < 6 && y >= 0 && y < 6;
}

bool canMove(const State& state, int vehicleId, int direction) {
    int x = state.vehicles[vehicleId].first;
    int y = state.vehicles[vehicleId].second;
    int len = state.length[vehicleId];
    bool horizontal = state.isHorizontal[vehicleId];
    
    if (horizontal && (direction == 0 || direction == 1)) return false;
    if (!horizontal && (direction == 2 || direction == 3)) return false;
    
    int nx = x + dx[direction];
    int ny = y + dy[direction];
    
    if (direction == 2) { // Left
        if (!isValid(nx, ny)) return false;
        return state.board[nx][ny] == 0;
    } else if (direction == 3) { // Right
        ny = y + len;
        if (!isValid(nx, ny)) return false;
        return state.board[nx][ny] == 0;
    } else if (direction == 0) { // Up
        if (!isValid(nx, ny)) return false;
        return state.board[nx][ny] == 0;
    } else { // Down
        nx = x + len;
        if (!isValid(nx, ny)) return false;
        return state.board[nx][ny] == 0;
    }
}

State makeMove(State state, int vehicleId, int direction) {
    int x = state.vehicles[vehicleId].first;
    int y = state.vehicles[vehicleId].second;
    int len = state.length[vehicleId];
    bool horizontal = state.isHorizontal[vehicleId];
    
    // Clear current position
    if (horizontal) {
        for (int i = 0; i < len; i++) {
            state.board[x][y + i] = 0;
        }
    } else {
        for (int i = 0; i < len; i++) {
            state.board[x + i][y] = 0;
        }
    }
    
    // Update position
    state.vehicles[vehicleId].first += dx[direction];
    state.vehicles[vehicleId].second += dy[direction];
    
    int nx = state.vehicles[vehicleId].first;
    int ny = state.vehicles[vehicleId].second;
    
    // Set new position
    if (horizontal) {
        for (int i = 0; i < len; i++) {
            state.board[nx][ny + i] = vehicleId;
        }
    } else {
        for (int i = 0; i < len; i++) {
            state.board[nx + i][ny] = vehicleId;
        }
    }
    
    return state;
}

bool isSolved(const State& state) {
    int redX = state.vehicles[1].first;
    int redY = state.vehicles[1].second;
    int redLen = state.length[1];
    
    return redY + redLen == 6;
}

int bfs(const State& initial) {
    if (isSolved(initial)) return 0;
    
    queue<pair<State, int>> q;
    set<State> visited;
    
    q.push({initial, 0});
    visited.insert(initial);
    
    while (!q.empty()) {
        auto [current, steps] = q.front();
        q.pop();
        
        for (int v = 1; v < current.vehicles.size(); v++) {
            for (int d = 0; d < 4; d++) {
                if (canMove(current, v, d)) {
                    State next = makeMove(current, v, d);
                    
                    if (visited.find(next) == visited.end()) {
                        visited.insert(next);
                        
                        if (isSolved(next)) {
                            return steps + 1;
                        }
                        
                        q.push({next, steps + 1});
                    }
                }
            }
        }
    }
    
    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<vector<int>> board(6, vector<int>(6));
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> board[i][j];
        }
    }
    
    // Parse vehicles
    map<int, vector<pair<int, int>>> vehiclePos;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] != 0) {
                vehiclePos[board[i][j]].push_back({i, j});
            }
        }
    }
    
    State initial;
    initial.board = board;
    initial.vehicles.resize(vehiclePos.size() + 1);
    initial.isHorizontal.resize(vehiclePos.size() + 1);
    initial.length.resize(vehiclePos.size() + 1);
    
    for (auto& [id, positions] : vehiclePos) {
        sort(positions.begin(), positions.end());
        initial.vehicles[id] = positions[0];
        initial.length[id] = positions.size();
        
        if (positions.size() > 1) {
            initial.isHorizontal[id] = (positions[0].first == positions[1].first);
        }
    }
    
    int originalSteps = bfs(initial);
    
    int maxSteps = originalSteps;
    vector<pair<int, int>> bestMoves;
    
    // Try different sequences of moves
    for (int v = 1; v < initial.vehicles.size(); v++) {
        for (int d = 0; d < 4; d++) {
            if (canMove(initial, v, d)) {
                State next = makeMove(initial, v, d);
                int steps = bfs(next);
                
                if (steps > maxSteps) {
                    maxSteps = steps;
                    bestMoves = {{v, d}};
                }
            }
        }
    }
    
    cout << maxSteps << " " << bestMoves.size() << "\n";
    for (auto [v, d] : bestMoves) {
        cout << v << " " << dir[d] << "\n";
    }
    
    return 0;
}
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

struct Move {
    int vehicle;
    char direction;
};

int dx[] = {-1, 1, 0, 0}; // U, D, L, R
int dy[] = {0, 0, -1, 1};
char dirs[] = {'U', 'D', 'L', 'R'};

bool isValid(int x, int y) {
    return x >= 0 && x < 6 && y >= 0 && y < 6;
}

bool canMove(const State& state, int vehicleId, int dir) {
    if (vehicleId < 1 || vehicleId >= state.vehicles.size()) return false;
    
    int x = state.vehicles[vehicleId].first;
    int y = state.vehicles[vehicleId].second;
    bool horizontal = state.isHorizontal[vehicleId];
    int len = state.length[vehicleId];
    
    // Check if direction is valid for vehicle orientation
    if (horizontal && (dir == 0 || dir == 1)) return false; // horizontal can't move up/down
    if (!horizontal && (dir == 2 || dir == 3)) return false; // vertical can't move left/right
    
    vector<pair<int, int>> newPositions;
    
    if (horizontal) {
        int ny = y + dy[dir];
        for (int i = 0; i < len; i++) {
            if (!isValid(x, ny + i)) return false;
            newPositions.push_back({x, ny + i});
        }
    } else {
        int nx = x + dx[dir];
        for (int i = 0; i < len; i++) {
            if (!isValid(nx + i, y)) return false;
            newPositions.push_back({nx + i, y});
        }
    }
    
    // Check if new positions are free (except current vehicle positions)
    for (auto& pos : newPositions) {
        if (state.board[pos.first][pos.second] != 0 && 
            state.board[pos.first][pos.second] != vehicleId) {
            return false;
        }
    }
    
    return true;
}

State makeMove(const State& state, int vehicleId, int dir) {
    State newState = state;
    
    int x = state.vehicles[vehicleId].first;
    int y = state.vehicles[vehicleId].second;
    bool horizontal = state.isHorizontal[vehicleId];
    int len = state.length[vehicleId];
    
    // Clear current position
    if (horizontal) {
        for (int i = 0; i < len; i++) {
            newState.board[x][y + i] = 0;
        }
    } else {
        for (int i = 0; i < len; i++) {
            newState.board[x + i][y] = 0;
        }
    }
    
    // Update vehicle position
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    newState.vehicles[vehicleId] = {nx, ny};
    
    // Place vehicle at new position
    if (horizontal) {
        for (int i = 0; i < len; i++) {
            newState.board[nx][ny + i] = vehicleId;
        }
    } else {
        for (int i = 0; i < len; i++) {
            newState.board[nx + i][ny] = vehicleId;
        }
    }
    
    return newState;
}

bool isSolved(const State& state) {
    // Red car (id=1) needs to reach the exit at right end of row 3
    int redCarY = state.vehicles[1].second;
    int redCarLen = state.length[1];
    return redCarY + redCarLen > 5; // part of car is beyond the board
}

int solve(const State& initialState) {
    queue<pair<State, int>> q;
    set<State> visited;
    
    q.push({initialState, 0});
    visited.insert(initialState);
    
    while (!q.empty()) {
        auto [currentState, steps] = q.front();
        q.pop();
        
        if (isSolved(currentState)) {
            return steps;
        }
        
        // Try all possible moves
        for (int vehicleId = 1; vehicleId < currentState.vehicles.size(); vehicleId++) {
            for (int dir = 0; dir < 4; dir++) {
                if (canMove(currentState, vehicleId, dir)) {
                    State newState = makeMove(currentState, vehicleId, dir);
                    if (visited.find(newState) == visited.end()) {
                        visited.insert(newState);
                        q.push({newState, steps + 1});
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
    
    State initialState;
    initialState.board.resize(6, vector<int>(6));
    
    int maxId = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> initialState.board[i][j];
            maxId = max(maxId, initialState.board[i][j]);
        }
    }
    
    // Initialize vehicle data
    initialState.vehicles.resize(maxId + 1);
    initialState.isHorizontal.resize(maxId + 1);
    initialState.length.resize(maxId + 1);
    
    for (int id = 1; id <= maxId; id++) {
        bool found = false;
        for (int i = 0; i < 6 && !found; i++) {
            for (int j = 0; j < 6; j++) {
                if (initialState.board[i][j] == id) {
                    initialState.vehicles[id] = {i, j};
                    
                    // Determine orientation and length
                    int len = 1;
                    bool horizontal = false;
                    
                    // Check horizontal
                    while (j + len < 6 && initialState.board[i][j + len] == id) {
                        len++;
                        horizontal = true;
                    }
                    
                    // Check vertical if not horizontal
                    if (!horizontal) {
                        len = 1;
                        while (i + len < 6 && initialState.board[i + len][j] == id) {
                            len++;
                        }
                    }
                    
                    initialState.isHorizontal[id] = horizontal;
                    initialState.length[id] = len;
                    found = true;
                    break;
                }
            }
        }
    }
    
    int originalSteps = solve(initialState);
    
    // Try a simple move to increase difficulty
    vector<Move> moves;
    int bestSteps = originalSteps;
    
    // Try moving red car left if possible
    if (canMove(initialState, 1, 2)) { // Move left
        State newState = makeMove(initialState, 1, 2);
        int steps = solve(newState);
        if (steps > bestSteps) {
            bestSteps = steps;
            moves = {{1, 'L'}};
        }
    }
    
    cout << bestSteps << " " << moves.size() << "\n";
    for (auto& move : moves) {
        cout << move.vehicle << " " << move.direction << "\n";
    }
    
    return 0;
}
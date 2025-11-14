#include <bits/stdc++.h>
using namespace std;

struct State {
    vector<vector<int>> board;
    vector<pair<int, int>> vehicles; // position and orientation info
    
    bool operator<(const State& other) const {
        return board < other.board;
    }
    
    bool operator==(const State& other) const {
        return board == other.board;
    }
};

struct Vehicle {
    int id;
    int row, col;
    int length;
    bool horizontal;
};

vector<Vehicle> parseVehicles(const vector<vector<int>>& board) {
    vector<Vehicle> vehicles;
    set<int> seen;
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] != 0 && seen.find(board[i][j]) == seen.end()) {
                Vehicle v;
                v.id = board[i][j];
                v.row = i;
                v.col = j;
                seen.insert(v.id);
                
                // Determine orientation and length
                if (j + 1 < 6 && board[i][j + 1] == v.id) {
                    v.horizontal = true;
                    v.length = 2;
                    if (j + 2 < 6 && board[i][j + 2] == v.id) {
                        v.length = 3;
                    }
                } else {
                    v.horizontal = false;
                    v.length = 2;
                    if (i + 2 < 6 && board[i + 2][j] == v.id) {
                        v.length = 3;
                    }
                }
                
                vehicles.push_back(v);
            }
        }
    }
    
    return vehicles;
}

vector<vector<int>> moveVehicle(vector<vector<int>> board, const Vehicle& v, char direction) {
    // Clear vehicle from current position
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == v.id) {
                board[i][j] = 0;
            }
        }
    }
    
    int newRow = v.row, newCol = v.col;
    
    if (direction == 'U') newRow--;
    else if (direction == 'D') newRow++;
    else if (direction == 'L') newCol--;
    else if (direction == 'R') newCol++;
    
    // Place vehicle at new position
    if (v.horizontal) {
        for (int j = 0; j < v.length; j++) {
            board[newRow][newCol + j] = v.id;
        }
    } else {
        for (int i = 0; i < v.length; i++) {
            board[newRow + i][newCol] = v.id;
        }
    }
    
    return board;
}

bool canMove(const vector<vector<int>>& board, const Vehicle& v, char direction) {
    int newRow = v.row, newCol = v.col;
    
    if (direction == 'U') newRow--;
    else if (direction == 'D') newRow++;
    else if (direction == 'L') newCol--;
    else if (direction == 'R') newCol++;
    
    // Check bounds
    if (v.horizontal) {
        if (newRow < 0 || newRow >= 6 || newCol < 0 || newCol + v.length - 1 >= 6) {
            return false;
        }
        // Check for obstacles
        for (int j = 0; j < v.length; j++) {
            if (board[newRow][newCol + j] != 0 && board[newRow][newCol + j] != v.id) {
                return false;
            }
        }
    } else {
        if (newRow < 0 || newRow + v.length - 1 >= 6 || newCol < 0 || newCol >= 6) {
            return false;
        }
        // Check for obstacles
        for (int i = 0; i < v.length; i++) {
            if (board[newRow + i][newCol] != 0 && board[newRow + i][newCol] != v.id) {
                return false;
            }
        }
    }
    
    return true;
}

bool isSolved(const vector<vector<int>>& board) {
    // Red car (id=1) should be at positions (2,4) and (2,5) to exit
    return board[2][4] == 1 && board[2][5] == 1;
}

int bfs(const vector<vector<int>>& board) {
    if (isSolved(board)) return 0;
    
    queue<pair<vector<vector<int>>, int>> q;
    set<vector<vector<int>>> visited;
    
    q.push({board, 0});
    visited.insert(board);
    
    while (!q.empty()) {
        auto [currentBoard, steps] = q.front();
        q.pop();
        
        vector<Vehicle> vehicles = parseVehicles(currentBoard);
        
        for (const Vehicle& v : vehicles) {
            vector<char> directions;
            if (v.horizontal) {
                directions = {'L', 'R'};
            } else {
                directions = {'U', 'D'};
            }
            
            for (char dir : directions) {
                if (canMove(currentBoard, v, dir)) {
                    vector<vector<int>> newBoard = moveVehicle(currentBoard, v, dir);
                    
                    if (isSolved(newBoard)) {
                        return steps + 1;
                    }
                    
                    if (visited.find(newBoard) == visited.end()) {
                        visited.insert(newBoard);
                        q.push({newBoard, steps + 1});
                    }
                }
            }
        }
    }
    
    return -1; // No solution found
}

int main() {
    vector<vector<int>> board(6, vector<int>(6));
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> board[i][j];
        }
    }
    
    // For simplicity, we'll try a few basic moves and pick the one that maximizes steps
    int maxSteps = bfs(board);
    vector<pair<int, char>> bestMoves;
    
    vector<Vehicle> vehicles = parseVehicles(board);
    
    // Try single moves
    for (const Vehicle& v : vehicles) {
        vector<char> directions;
        if (v.horizontal) {
            directions = {'L', 'R'};
        } else {
            directions = {'U', 'D'};
        }
        
        for (char dir : directions) {
            if (canMove(board, v, dir)) {
                vector<vector<int>> newBoard = moveVehicle(board, v, dir);
                int steps = bfs(newBoard);
                if (steps > maxSteps) {
                    maxSteps = steps;
                    bestMoves = {{v.id, dir}};
                }
            }
        }
    }
    
    if (bestMoves.empty()) {
        maxSteps = bfs(board);
    }
    
    cout << maxSteps << " " << bestMoves.size() << "\n";
    for (auto [id, dir] : bestMoves) {
        cout << id << " " << dir << "\n";
    }
    
    return 0;
}
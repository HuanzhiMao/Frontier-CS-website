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
            if (board[i][j] > 0 && seen.find(board[i][j]) == seen.end()) {
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

bool canMove(const vector<vector<int>>& board, const Vehicle& v, char direction) {
    if (v.horizontal) {
        if (direction == 'L') {
            return v.col > 0 && board[v.row][v.col - 1] == 0;
        } else if (direction == 'R') {
            return v.col + v.length < 6 && board[v.row][v.col + v.length] == 0;
        }
    } else {
        if (direction == 'U') {
            return v.row > 0 && board[v.row - 1][v.col] == 0;
        } else if (direction == 'D') {
            return v.row + v.length < 6 && board[v.row + v.length][v.col] == 0;
        }
    }
    return false;
}

vector<vector<int>> moveVehicle(const vector<vector<int>>& board, const Vehicle& v, char direction) {
    vector<vector<int>> newBoard = board;
    
    // Clear old position
    for (int i = 0; i < v.length; i++) {
        if (v.horizontal) {
            newBoard[v.row][v.col + i] = 0;
        } else {
            newBoard[v.row + i][v.col] = 0;
        }
    }
    
    // Set new position
    int newRow = v.row, newCol = v.col;
    if (direction == 'U') newRow--;
    else if (direction == 'D') newRow++;
    else if (direction == 'L') newCol--;
    else if (direction == 'R') newCol++;
    
    for (int i = 0; i < v.length; i++) {
        if (v.horizontal) {
            newBoard[newRow][newCol + i] = v.id;
        } else {
            newBoard[newRow + i][newCol] = v.id;
        }
    }
    
    return newBoard;
}

bool isSolved(const vector<vector<int>>& board) {
    // Check if red car (id=1) can reach the exit at row 2, col 5
    for (int j = 0; j < 6; j++) {
        if (board[2][j] == 1) {
            // Find the rightmost position of the red car
            int rightmost = j;
            while (rightmost + 1 < 6 && board[2][rightmost + 1] == 1) {
                rightmost++;
            }
            // Check if there's a clear path to the exit
            bool canExit = true;
            for (int k = rightmost + 1; k < 6; k++) {
                if (board[2][k] != 0) {
                    canExit = false;
                    break;
                }
            }
            if (canExit) return true;
            break;
        }
    }
    return false;
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
                    
                    if (visited.find(newBoard) == visited.end()) {
                        visited.insert(newBoard);
                        
                        if (isSolved(newBoard)) {
                            return steps + 1;
                        }
                        
                        q.push({newBoard, steps + 1});
                    }
                }
            }
        }
    }
    
    return -1;
}

int main() {
    vector<vector<int>> board(6, vector<int>(6));
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> board[i][j];
        }
    }
    
    int originalSteps = bfs(board);
    
    // Try to find a move that increases the minimum steps
    vector<Vehicle> vehicles = parseVehicles(board);
    int maxSteps = originalSteps;
    vector<pair<int, char>> bestMoves;
    
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
        cout << originalSteps << " 0" << endl;
    } else {
        cout << maxSteps << " " << bestMoves.size() << endl;
        for (auto [id, dir] : bestMoves) {
            cout << id << " " << dir << endl;
        }
    }
    
    return 0;
}
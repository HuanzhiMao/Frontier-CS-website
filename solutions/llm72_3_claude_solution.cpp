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

bool isSolved(const vector<vector<int>>& board) {
    // Check if red car (id=1) can exit from row 2
    for (int j = 0; j < 6; j++) {
        if (board[2][j] == 1) {
            // Check if path to exit is clear
            bool canExit = true;
            for (int k = j; k < 6; k++) {
                if (board[2][k] != 1 && board[2][k] != 0) {
                    canExit = false;
                    break;
                }
            }
            if (canExit) {
                // Find rightmost position of red car
                int rightmost = j;
                while (rightmost + 1 < 6 && board[2][rightmost + 1] == 1) {
                    rightmost++;
                }
                return rightmost >= 5; // Can exit
            }
            break;
        }
    }
    return false;
}

int bfs(const vector<vector<int>>& initial) {
    queue<pair<vector<vector<int>>, int>> q;
    set<vector<vector<int>>> visited;
    
    q.push({initial, 0});
    visited.insert(initial);
    
    while (!q.empty()) {
        auto [board, steps] = q.front();
        q.pop();
        
        if (isSolved(board)) {
            return steps;
        }
        
        vector<Vehicle> vehicles = parseVehicles(board);
        
        for (const Vehicle& v : vehicles) {
            vector<vector<int>> newBoard = board;
            
            // Clear current position
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 6; j++) {
                    if (newBoard[i][j] == v.id) {
                        newBoard[i][j] = 0;
                    }
                }
            }
            
            // Try moving in both directions
            vector<pair<int, int>> directions;
            if (v.horizontal) {
                directions = {{0, -1}, {0, 1}}; // left, right
            } else {
                directions = {{-1, 0}, {1, 0}}; // up, down
            }
            
            for (auto [dr, dc] : directions) {
                int newRow = v.row + dr;
                int newCol = v.col + dc;
                
                // Check bounds
                bool valid = true;
                if (v.horizontal) {
                    if (newCol < 0 || newCol + v.length - 1 >= 6) valid = false;
                } else {
                    if (newRow < 0 || newRow + v.length - 1 >= 6) valid = false;
                }
                
                if (!valid) continue;
                
                // Check collision
                bool collision = false;
                for (int k = 0; k < v.length; k++) {
                    int checkRow = newRow + (v.horizontal ? 0 : k);
                    int checkCol = newCol + (v.horizontal ? k : 0);
                    if (newBoard[checkRow][checkCol] != 0) {
                        collision = true;
                        break;
                    }
                }
                
                if (!collision) {
                    // Place vehicle at new position
                    for (int k = 0; k < v.length; k++) {
                        int placeRow = newRow + (v.horizontal ? 0 : k);
                        int placeCol = newCol + (v.horizontal ? k : 0);
                        newBoard[placeRow][placeCol] = v.id;
                    }
                    
                    if (visited.find(newBoard) == visited.end()) {
                        visited.insert(newBoard);
                        q.push({newBoard, steps + 1});
                    }
                }
            }
            
            // Restore original board
            for (int k = 0; k < v.length; k++) {
                int placeRow = v.row + (v.horizontal ? 0 : k);
                int placeCol = v.col + (v.horizontal ? k : 0);
                newBoard[placeRow][placeCol] = v.id;
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
    
    // Find red car position
    int redRow = 2, redCol = -1;
    for (int j = 0; j < 6; j++) {
        if (board[2][j] == 1) {
            redCol = j;
            break;
        }
    }
    
    // Try moving red car left to maximize steps
    vector<vector<int>> newBoard = board;
    int moveSteps = 0;
    char moveDir = 'L';
    
    if (redCol > 0 && board[2][redCol - 1] == 0) {
        // Move red car left
        newBoard[2][redCol - 1] = 1;
        newBoard[2][redCol + 1] = 0;
        moveSteps = 1;
    } else {
        moveSteps = 0;
    }
    
    int minSteps = bfs(newBoard);
    
    cout << minSteps << " " << moveSteps << endl;
    if (moveSteps > 0) {
        cout << "1 " << moveDir << endl;
    }
    
    return 0;
}
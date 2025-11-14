#include <bits/stdc++.h>
using namespace std;

int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool inBounds(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 14;
}

bool canRead(vector<string>& grid, string target, int x, int y, int pos, vector<vector<bool>>& visited) {
    if (pos == target.length()) return true;
    if (!inBounds(x, y)) return false;
    if (grid[x][y] != target[pos]) return false;
    
    for (int d = 0; d < 8; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (canRead(grid, target, nx, ny, pos + 1, visited)) {
            return true;
        }
    }
    return false;
}

bool canReadNumber(vector<string>& grid, int num) {
    string target = to_string(num);
    vector<vector<bool>> visited(8, vector<bool>(14, false));
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            if (canRead(grid, target, i, j, 0, visited)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    vector<string> grid(8, string(14, '0'));
    
    // Place digits strategically
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            grid[i][j] = '0' + ((i + j) % 10);
        }
    }
    
    // Add specific patterns for better coverage
    grid[0][0] = '1'; grid[0][1] = '2'; grid[0][2] = '3';
    grid[1][0] = '4'; grid[1][1] = '5'; grid[1][2] = '6';
    grid[2][0] = '7'; grid[2][1] = '8'; grid[2][2] = '9';
    
    // Create more concentrated digit areas
    for (int i = 3; i < 6; i++) {
        for (int j = 10; j < 14; j++) {
            grid[i][j] = '0' + ((i * j) % 10);
        }
    }
    
    // Add sequential patterns
    grid[6][0] = '1'; grid[6][1] = '0'; grid[6][2] = '1';
    grid[7][0] = '1'; grid[7][1] = '1'; grid[7][2] = '1';
    
    // Fill remaining with varied digits
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            if (grid[i][j] == '0' + ((i + j) % 10)) {
                grid[i][j] = '0' + ((i * 3 + j * 7 + 1) % 10);
            }
        }
    }
    
    for (int i = 0; i < 8; i++) {
        cout << grid[i] << "\n";
    }
    
    return 0;
}
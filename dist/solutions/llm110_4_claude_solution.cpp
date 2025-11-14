#include <bits/stdc++.h>
using namespace std;

int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool dfs(vector<string>& grid, string target, int pos, int x, int y, int prevX, int prevY) {
    if (pos == target.length()) return true;
    
    for (int d = 0; d < 8; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 14 && !(nx == prevX && ny == prevY)) {
            if (grid[nx][ny] == target[pos]) {
                if (dfs(grid, target, pos + 1, nx, ny, x, y)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool canRead(vector<string>& grid, int num) {
    string target = to_string(num);
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            if (grid[i][j] == target[0]) {
                if (target.length() == 1 || dfs(grid, target, 1, i, j, -1, -1)) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    vector<string> grid(8, string(14, '0'));
    
    // Place digits strategically
    // First row with all digits 0-9 and some repetitions
    grid[0] = "12345678901234";
    grid[1] = "23456789012345";
    grid[2] = "34567890123456";
    grid[3] = "45678901234567";
    grid[4] = "56789012345678";
    grid[5] = "67890123456789";
    grid[6] = "78901234567890";
    grid[7] = "89012345678901";
    
    // Try to optimize for higher numbers
    int maxScore = 0;
    vector<string> bestGrid = grid;
    
    // Random optimization with time limit
    auto start = chrono::high_resolution_clock::now();
    
    while (true) {
        auto now = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(now - start).count() > 50000) break;
        
        // Make small random changes
        vector<string> testGrid = bestGrid;
        int randRow = rand() % 8;
        int randCol = rand() % 14;
        testGrid[randRow][randCol] = '0' + (rand() % 10);
        
        // Test score
        int score = 0;
        for (int i = 1; i <= min(200, maxScore + 50); i++) {
            if (canRead(testGrid, i)) {
                score = i;
            } else {
                break;
            }
        }
        
        if (score > maxScore) {
            maxScore = score;
            bestGrid = testGrid;
        }
    }
    
    for (const string& row : bestGrid) {
        cout << row << "\n";
    }
    
    return 0;
}
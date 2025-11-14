#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
using namespace std;

int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool isValid(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 14;
}

bool canRead(vector<vector<int>>& grid, string target) {
    int n = target.length();
    if (n == 0) return false;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            if (grid[i][j] == target[0] - '0') {
                vector<vector<vector<bool>>> visited(8, vector<vector<bool>>(14, vector<bool>(n, false)));
                queue<tuple<int, int, int>> q;
                q.push({i, j, 0});
                visited[i][j][0] = true;
                
                while (!q.empty()) {
                    auto [x, y, pos] = q.front();
                    q.pop();
                    
                    if (pos == n - 1) return true;
                    
                    for (int d = 0; d < 8; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        
                        if (isValid(nx, ny) && grid[nx][ny] == target[pos + 1] - '0' && !visited[nx][ny][pos + 1]) {
                            visited[nx][ny][pos + 1] = true;
                            q.push({nx, ny, pos + 1});
                        }
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    vector<vector<int>> grid(8, vector<int>(14));
    
    // Initialize with a pattern that allows many small numbers
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            grid[i][j] = (i + j) % 10;
        }
    }
    
    // Add specific patterns for common digits
    for (int i = 0; i < 8; i++) {
        grid[i][0] = 1;
        grid[i][1] = 2;
        grid[i][2] = 3;
    }
    
    for (int j = 0; j < 14; j++) {
        grid[0][j] = j % 10;
        grid[1][j] = (j + 1) % 10;
    }
    
    // Fill with repeating patterns to maximize connectivity
    for (int i = 2; i < 8; i++) {
        for (int j = 3; j < 14; j++) {
            grid[i][j] = (i * j + i + j) % 10;
        }
    }
    
    // Add zeros for easier path construction
    for (int i = 4; i < 6; i++) {
        for (int j = 6; j < 10; j++) {
            grid[i][j] = 0;
        }
    }
    
    // Print the grid
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            cout << grid[i][j];
        }
        cout << "\n";
    }
    
    return 0;
}
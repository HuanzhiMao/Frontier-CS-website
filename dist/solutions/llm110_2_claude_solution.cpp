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

bool canRead(vector<string>& grid, string target) {
    int n = target.length();
    if (n == 0) return false;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            if (grid[i][j] == target[0]) {
                vector<vector<vector<bool>>> visited(8, vector<vector<bool>>(14, vector<bool>(n, false)));
                queue<tuple<int, int, int>> q;
                q.push({i, j, 0});
                
                while (!q.empty()) {
                    auto [x, y, pos] = q.front();
                    q.pop();
                    
                    if (pos == n - 1) return true;
                    
                    if (visited[x][y][pos]) continue;
                    visited[x][y][pos] = true;
                    
                    for (int d = 0; d < 8; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        
                        if (isValid(nx, ny) && grid[nx][ny] == target[pos + 1] && !visited[nx][ny][pos + 1]) {
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
    vector<string> grid(8, string(14, '0'));
    
    // Create a pattern that allows reading many consecutive numbers
    string pattern = "0123456789";
    
    // Fill first few rows with repeating digits to make small numbers easy
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 14; j++) {
            grid[i][j] = '0' + ((i * 14 + j) % 10);
        }
    }
    
    // Add specific patterns for common digit sequences
    grid[3] = "12345678901234";
    grid[4] = "23456789012345";
    grid[5] = "34567890123456";
    grid[6] = "45678901234567";
    grid[7] = "56789012345678";
    
    // Output the grid
    for (int i = 0; i < 8; i++) {
        cout << grid[i] << "\n";
    }
    
    return 0;
}
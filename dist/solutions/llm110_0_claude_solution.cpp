#include <bits/stdc++.h>
using namespace std;

int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

char grid[8][14];
bool found;
string target;

bool isValid(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 14;
}

void dfs(int x, int y, int pos, int prevX, int prevY) {
    if (found) return;
    if (pos == target.length()) {
        found = true;
        return;
    }
    
    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (isValid(nx, ny) && (nx != prevX || ny != prevY) && 
            grid[nx][ny] == target[pos]) {
            dfs(nx, ny, pos + 1, x, y);
        }
    }
}

bool canRead(int num) {
    target = to_string(num);
    found = false;
    
    for (int i = 0; i < 8 && !found; i++) {
        for (int j = 0; j < 14 && !found; j++) {
            if (grid[i][j] == target[0]) {
                dfs(i, j, 1, -1, -1);
            }
        }
    }
    
    return found;
}

int main() {
    // Initialize grid with strategic pattern
    string pattern[] = {
        "12345678901234",
        "23456789012345", 
        "34567890123456",
        "45678901234567",
        "56789012345678",
        "67890123456789",
        "78901234567890",
        "89012345678901"
    };
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            grid[i][j] = pattern[i][j];
        }
    }
    
    // Try to improve by testing and modifying
    int maxScore = 0;
    for (int test = 1; test <= 200; test++) {
        if (!canRead(test)) {
            maxScore = test - 1;
            break;
        }
        maxScore = test;
    }
    
    // If we can improve, try some modifications
    if (maxScore < 150) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 14; j++) {
                char original = grid[i][j];
                for (char c = '0'; c <= '9'; c++) {
                    grid[i][j] = c;
                    int newScore = 0;
                    for (int test = 1; test <= maxScore + 20; test++) {
                        if (!canRead(test)) {
                            newScore = test - 1;
                            break;
                        }
                        newScore = test;
                    }
                    if (newScore > maxScore) {
                        maxScore = newScore;
                        original = c;
                    }
                }
                grid[i][j] = original;
            }
        }
    }
    
    // Print the final grid
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            cout << grid[i][j];
        }
        cout << "\n";
    }
    
    return 0;
}
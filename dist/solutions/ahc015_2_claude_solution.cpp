#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    vector<int> flavors(100);
    for (int i = 0; i < 100; i++) {
        cin >> flavors[i];
    }
    
    vector<vector<int>> grid(10, vector<int>(10, 0));
    vector<char> directions = {'F', 'B', 'L', 'R'};
    
    for (int t = 0; t < 100; t++) {
        int p;
        cin >> p;
        
        // Find the p-th empty cell
        int count = 0;
        int row = -1, col = -1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (grid[i][j] == 0) {
                    count++;
                    if (count == p) {
                        row = i;
                        col = j;
                        break;
                    }
                }
            }
            if (row != -1) break;
        }
        
        // Place the candy
        grid[row][col] = flavors[t];
        
        // Choose direction based on candy flavor and position
        char chosen_dir;
        
        if (flavors[t] == 1) {
            // Try to move flavor 1 to top-left corner
            if (row > 5) chosen_dir = 'F';
            else if (col > 5) chosen_dir = 'L';
            else chosen_dir = 'F';
        } else if (flavors[t] == 2) {
            // Try to move flavor 2 to top-right corner
            if (row > 5) chosen_dir = 'F';
            else if (col < 4) chosen_dir = 'R';
            else chosen_dir = 'R';
        } else {
            // Try to move flavor 3 to bottom
            if (row < 4) chosen_dir = 'B';
            else if (col > 5) chosen_dir = 'L';
            else chosen_dir = 'B';
        }
        
        cout << chosen_dir << endl;
        cout.flush();
        
        // Simulate the tilt
        if (chosen_dir == 'F') {
            for (int j = 0; j < 10; j++) {
                vector<int> column;
                for (int i = 0; i < 10; i++) {
                    if (grid[i][j] != 0) {
                        column.push_back(grid[i][j]);
                    }
                    grid[i][j] = 0;
                }
                for (int i = 0; i < column.size(); i++) {
                    grid[i][j] = column[i];
                }
            }
        } else if (chosen_dir == 'B') {
            for (int j = 0; j < 10; j++) {
                vector<int> column;
                for (int i = 0; i < 10; i++) {
                    if (grid[i][j] != 0) {
                        column.push_back(grid[i][j]);
                    }
                    grid[i][j] = 0;
                }
                for (int i = 0; i < column.size(); i++) {
                    grid[9 - column.size() + 1 + i][j] = column[i];
                }
            }
        } else if (chosen_dir == 'L') {
            for (int i = 0; i < 10; i++) {
                vector<int> row_candies;
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j] != 0) {
                        row_candies.push_back(grid[i][j]);
                    }
                    grid[i][j] = 0;
                }
                for (int j = 0; j < row_candies.size(); j++) {
                    grid[i][j] = row_candies[j];
                }
            }
        } else { // 'R'
            for (int i = 0; i < 10; i++) {
                vector<int> row_candies;
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j] != 0) {
                        row_candies.push_back(grid[i][j]);
                    }
                    grid[i][j] = 0;
                }
                for (int j = 0; j < row_candies.size(); j++) {
                    grid[i][9 - row_candies.size() + 1 + j] = row_candies[j];
                }
            }
        }
    }
    
    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
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
        int empty_count = 0;
        int place_row = -1, place_col = -1;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (grid[i][j] == 0) {
                    empty_count++;
                    if (empty_count == p) {
                        place_row = i;
                        place_col = j;
                        break;
                    }
                }
            }
            if (place_row != -1) break;
        }
        
        grid[place_row][place_col] = flavors[t];
        
        // Simple strategy: alternate between directions to spread candies
        char chosen_dir = directions[t % 4];
        
        // Apply the tilt
        if (chosen_dir == 'F') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 0;
                for (int i = 0; i < 10; i++) {
                    if (grid[i][j] != 0) {
                        if (write_pos != i) {
                            grid[write_pos][j] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos++;
                    }
                }
            }
        } else if (chosen_dir == 'B') {
            for (int j = 0; j < 10; j++) {
                int write_pos = 9;
                for (int i = 9; i >= 0; i--) {
                    if (grid[i][j] != 0) {
                        if (write_pos != i) {
                            grid[write_pos][j] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos--;
                    }
                }
            }
        } else if (chosen_dir == 'L') {
            for (int i = 0; i < 10; i++) {
                int write_pos = 0;
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j] != 0) {
                        if (write_pos != j) {
                            grid[i][write_pos] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos++;
                    }
                }
            }
        } else if (chosen_dir == 'R') {
            for (int i = 0; i < 10; i++) {
                int write_pos = 9;
                for (int j = 9; j >= 0; j--) {
                    if (grid[i][j] != 0) {
                        if (write_pos != j) {
                            grid[i][write_pos] = grid[i][j];
                            grid[i][j] = 0;
                        }
                        write_pos--;
                    }
                }
            }
        }
        
        cout << chosen_dir << endl;
        cout.flush();
    }
    
    return 0;
}
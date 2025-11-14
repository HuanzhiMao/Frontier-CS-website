#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int task;
    cin >> task;
    
    vector<string> grid(12, string(12, ' '));
    
    if (task == 0) {
        // Small task - using digits 0-3
        grid[0] = "3   2   111 ";
        grid[1] = "32 31  2   1";
        grid[2] = "3 2 1  2   1";
        grid[3] = "3 2 1  2111 ";
        grid[4] = "3 2 1  2    ";
        grid[5] = "3   1  2    ";
        grid[6] = "            ";
        grid[7] = "1  3   11111";
        grid[8] = "1 3      1  ";
        grid[9] = "13   1 3 1  ";
        grid[10] = "1 3  1 3 1  ";
        grid[11] = "1  3 111 1  ";
    } else {
        // Large task - using digits 1-3
        grid[0] = "3   2   111 ";
        grid[1] = "32 31  2   1";
        grid[2] = "3 2 1  2   1";
        grid[3] = "3 2 1  2111 ";
        grid[4] = "3 2 1  2    ";
        grid[5] = "3   1  2    ";
        grid[6] = "            ";
        grid[7] = "1  3   11111";
        grid[8] = "1 3      1  ";
        grid[9] = "13   1 3 1  ";
        grid[10] = "1 3  1 3 1  ";
        grid[11] = "1  3 111 1  ";
    }
    
    for (int i = 0; i < 12; i++) {
        cout << grid[i] << "\n";
    }
    
    return 0;
}
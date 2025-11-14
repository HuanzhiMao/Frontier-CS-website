#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int task;
    cin >> task;
    
    vector<string> grid(12, string(12, ' '));
    
    if (task == 0) {
        // Small task - can use 0,1,2,3
        grid[0] = "3   2   131 ";
        grid[1] = "31 32  3   2";
        grid[2] = "3 2 3  1   2";
        grid[3] = "3 2 1  2132 ";
        grid[4] = "3 2 1  3    ";
        grid[5] = "3   1  2    ";
        grid[6] = "            ";
        grid[7] = "2  1   31323";
        grid[8] = "2 1      3  ";
        grid[9] = "21   3 2 1  ";
        grid[10] = "2 3  1 2 3  ";
        grid[11] = "2  1 313 1  ";
    } else {
        // Large task - can use 1,2,3
        grid[0] = "3   2   131 ";
        grid[1] = "31 32  3   2";
        grid[2] = "3 2 3  1   2";
        grid[3] = "3 2 1  2132 ";
        grid[4] = "3 2 1  3    ";
        grid[5] = "3   1  2    ";
        grid[6] = "            ";
        grid[7] = "2  1   31323";
        grid[8] = "2 1      3  ";
        grid[9] = "21   3 2 1  ";
        grid[10] = "2 3  1 2 3  ";
        grid[11] = "2  1 313 1  ";
    }
    
    for (int i = 0; i < 12; i++) {
        cout << grid[i] << endl;
    }
    
    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long x;
    cin >> x;

    int n_max = 300;
    vector<vector<int>> grid(n_max + 1, vector<int>(n_max + 1, 0));

    long long current_val = 1;
    int current_row = 1;

    // Find the largest power of 2 less than or equal to x
    while ((current_val << 1) <= x) {
        current_val <<= 1;
        current_row++;
    }

    // Create the path for the largest power of 2
    for (int i = 1; i < current_row; ++i) {
        grid[i][i] = 1;
        grid[i][i + 1] = 1;
    }
    grid[current_row][current_row] = 1;
    
    // The main path now has current_val ways to reach (current_row, current_row)

    long long remainder = x - current_val;

    // Add smaller powers of 2 based on the remaining value
    for (int i = current_row - 1; i >= 1; --i) {
        if ((remainder >> (i - 1)) & 1) {
            grid[current_row][i] = 1;
            remainder -= (1LL << (i - 1));
            // Extend the main path downwards to merge this new branch
            current_row++;
            grid[current_row][i] = 1;
        }
    }
    
    // Fill the rest of the path for the accumulator
    for (int i = 1; i <= current_row; ++i) {
        for (int j = 1; j < i; ++j) {
            if(grid[i][j] == 1) {
                 for(int k=j; k < i; ++k) {
                     grid[i][k] = 1;
                 }
            }
        }
    }

    cout << current_row << endl;
    for (int i = 1; i <= current_row; ++i) {
        for (int j = 1; j <= current_row; ++j) {
            cout << grid[i][j] << (j == current_row ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}